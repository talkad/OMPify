import torch.utils.data
from transformers import BartConfig, Seq2SeqTrainingArguments, IntervalStrategy, SchedulerType, TrainingArguments
import pickle

import logging
import os
from typing import Union, Tuple

import enums
from data.dataset import init_dataset
from data.vocab import Vocab, init_vocab, load_vocab
from utils.general import count_params, human_format, layer_wise_parameters
from utils.trainer import CodeTrainer, CodeCLSTrainer
from utils.callbacks import LogStateCallBack
from models.bart import BartForClassificationAndGeneration


logger = logging.getLogger(__name__)


def pre_train(args,
              trained_model: Union[BartForClassificationAndGeneration, str] = None,
              trained_vocab: Union[Tuple[Vocab, Vocab, Vocab], str] = None):
    tasks = args.pre_train_tasks

    if tasks is None:
        logger.warning('Was specified for pre-training, but got pre-training tasks to None, '
                       'will default to {}'.format(','.join(enums.PRE_TRAIN_TASKS)))
        tasks = enums.PRE_TRAIN_TASKS
    else:
        supported_tasks = []
        for task in tasks.split(','):
            task = task.strip().lower()
            if task in enums.PRE_TRAIN_TASKS:
                supported_tasks.append(task)
            else:
                logger.warning(f'Pre-training task {task} is not supported and will be ignored.')
        tasks = supported_tasks

    assert not trained_model or \
        isinstance(trained_model, str) or \
        isinstance(trained_model, BartForClassificationAndGeneration), \
        f'The model type is not supported, expect Bart model or string of model dir, got {type(trained_model)}'

    if trained_vocab is None and args.trained_vocab is not None:
        trained_vocab = args.trained_vocab
    assert not trained_vocab or isinstance(trained_vocab, str), \
        f'The vocab type is not supported, expect string of vocab dir, got {type(trained_vocab)}'

    logger.info('*' * 100)
    logger.info('Initializing pre-training environments')
    logger.info(f'Pre-training tasks: {args.pre_train_tasks}')

    # --------------------------------------------------
    # datasets
    # --------------------------------------------------
    logger.info('-' * 100)
    logger.info('Loading and parsing datasets')
    dataset = init_dataset(args=args, mode=enums.TRAINING_MODE_PRE_TRAIN)
    logger.info(f'The size of pre_training set: {len(dataset)}')
    if args.pre_train_subset_ratio:
        dataset = dataset.subset(args.pre_train_subset_ratio)
        logger.info(f'The pre-train dataset is trimmed to subset due to the argument: '
                    f'train_subset_ratio={args.pre_train_subset_ratio}')
        logger.info('The size of trimmed pre-train set: {}'.format(len(dataset)))
    logger.info('Datasets loaded and parsed successfully')


    # --------------------------------------------------
    # vocabs
    # --------------------------------------------------
    logger.info('-' * 100)
    if trained_vocab:
        logger.info('Loading vocabularies from files')

        if args.no_replaced:
            code_vocab = load_vocab(vocab_root=trained_vocab, name='code.bpe.50000.None') #name=args.code_vocab_name)
        else:
            code_vocab = load_vocab(vocab_root=trained_vocab, name=args.replaced_code_vocab_name)

        ast_vocab = load_vocab(vocab_root=trained_vocab, name='ast.word.None.50000') #name=args.ast_vocab_name)
    else:
        logger.info('Building vocabularies')

        if args.no_replaced:

            # code vocab
            code_vocab = init_vocab(vocab_save_dir=args.vocab_save_dir,
                                    name=args.code_vocab_name,
                                    method=args.code_tokenize_method,
                                    vocab_size=args.code_vocab_size,
                                    datasets=list(map(lambda code: code.split(), dataset.source_tokens)),
                                    ignore_case=True,
                                    save_root=args.vocab_root)
        else:
            # replaced code vocab
            code_vocab = init_vocab(vocab_save_dir=args.vocab_save_dir,
                                    name=args.replaced_code_vocab_name,
                                    method='comp',
                                    datasets=[],
                                    ignore_case=True,
                                    save_root=args.vocab_root)
            
        # ast vocab
        ast_vocab = init_vocab(vocab_save_dir=args.vocab_save_dir,
                               name=args.ast_vocab_name,
                               method='word',
                               datasets=list(map(lambda ast: ast.split(), dataset.asts)),
                               save_root=args.vocab_root,
                               index_offset=len(code_vocab))
        
        # # dfg vocab
        # dfg_vocab = init_vocab(vocab_save_dir=args.vocab_save_dir,
        #                        name=args.ast_vocab_name,
        #                        method='comp',
        #                        datasets=[dataset.dfg],
        #                        save_root=args.vocab_root,
        #                        index_offset=len(code_vocab)+len(ast_vocab))
        
    logger.info(f'The size of code vocabulary: {len(code_vocab)}')
    logger.info(f'The size of ast vocabulary: {len(ast_vocab)}')
    # logger.info(f'The size of ast vocabulary: {len(dfg_vocab)}')
    logger.info('Vocabularies built successfully')


    # --------------------------------------------------
    # Model
    # --------------------------------------------------
    logger.info('-' * 100)
    logger.info('Building model')
    config = BartConfig(vocab_size=len(code_vocab) + len(ast_vocab),
                        max_position_embeddings=512,
                        encoder_layers=args.n_layer,
                        encoder_ffn_dim=args.d_ff,
                        encoder_attention_heads=args.n_head,
                        decoder_layers=args.n_layer,
                        decoder_ffn_dim=args.d_ff,
                        decoder_attention_heads=args.n_head,
                        activation_function='gelu',
                        d_model=args.d_model,
                        dropout=args.dropout,
                        use_cache=True,
                        pad_token_id=Vocab.START_VOCAB.index(Vocab.PAD_TOKEN),
                        bos_token_id=Vocab.START_VOCAB.index(Vocab.SOS_TOKEN),
                        eos_token_id=Vocab.START_VOCAB.index(Vocab.EOS_TOKEN),
                        is_encoder_decoder=True,
                        decoder_start_token_id=Vocab.START_VOCAB.index(Vocab.SOS_TOKEN),
                        forced_eos_token_id=Vocab.START_VOCAB.index(Vocab.EOS_TOKEN),
                        max_length=100,  # limit decoder output
                        min_length=1,
                        num_beams=args.beam_width,
                        num_labels=2)
    model = BartForClassificationAndGeneration(config)

    # config = BartConfig.from_pretrained('/home/1010/talkad/OMPify/outputs/wed_pre_train_fortran_tokom_cap_mass_20230802_180945/models/mass/config.json')
    # model = BartForClassificationAndGeneration(config)
    # model.load_state_dict(torch.load('/home/1010/talkad/OMPify/outputs/wed_pre_train_fortran_tokom_cap_mass_20230802_180945/models/mass/pytorch_model.bin'))

    # log model statistic
    logger.info('Model trainable parameters: {}'.format(human_format(count_params(model))))
    table = layer_wise_parameters(model)
    logger.debug('Layer-wised trainable parameters:\n{}'.format(table))
    logger.info('Model built successfully')


    # --------------------------------------------------
    # pre-train
    # --------------------------------------------------
    for task in tasks:
        logger.info('-' * 100)
        logger.info(f'Pre-training task: {task.upper()}')

        if isinstance(dataset, torch.utils.data.Subset):
            dataset.dataset.set_task(task)
        else:
            dataset.set_task(task)

        if task == enums.TASK_CODE_AST_PREDICTION:
            # set model mode
            logger.info('-' * 100)
            model.set_model_mode(enums.MODEL_MODE_CLS)
            # --------------------------------------------------
            # trainer
            # --------------------------------------------------
            logger.info('-' * 100)
            logger.info('Initializing the running configurations')
            training_args = TrainingArguments(output_dir=os.path.join(args.pre_train_output_root, task),
                                              overwrite_output_dir=True,
                                              do_train=True,
                                              per_device_train_batch_size=args.batch_size,
                                              gradient_accumulation_steps=1,
                                              learning_rate=args.learning_rate,
                                              weight_decay=args.lr_decay_rate,
                                              max_grad_norm=args.grad_clipping_norm,
                                              num_train_epochs=args.n_epoch,
                                              lr_scheduler_type=SchedulerType.LINEAR,
                                              warmup_steps=args.warmup_steps,
                                              logging_dir=os.path.join(args.tensor_board_root, task),
                                              logging_strategy=IntervalStrategy.STEPS,
                                              logging_steps=args.logging_steps,
                                              save_strategy=IntervalStrategy.NO,
                                              seed=args.random_seed,
                                              fp16=args.fp16,
                                              dataloader_drop_last=False,
                                              run_name=args.model_name,
                                              load_best_model_at_end=True,
                                              ignore_data_skip=False,
                                              label_smoothing_factor=args.label_smoothing,
                                              report_to=['tensorboard'],
                                              dataloader_pin_memory=True)
            trainer = CodeCLSTrainer(main_args=args,
                                     code_vocab=code_vocab,
                                     ast_vocab=ast_vocab,
                                     dfg_vocab=None,
                                     task=task,
                                     model=model,
                                     args=training_args,
                                     data_collator=None,
                                     train_dataset=dataset,
                                     tokenizer=None,
                                     model_init=None,
                                     compute_metrics=None,
                                     callbacks=[LogStateCallBack()])
            logger.info('Running configurations initialized successfully')

            # --------------------------------------------------
            # train
            # --------------------------------------------------
            logger.info('-' * 100)
            logger.info(f'Start pre-training task: {task}')
            cap_result = trainer.train()
            logger.info(f'Pre-training task {task} finished')
            trainer.save_model(os.path.join(args.model_root, task))

        elif task == enums.TASK_MASS:
            # set model mode
            logger.info('-' * 100)
            model.set_model_mode(enums.MODEL_MODE_GEN)
            # --------------------------------------------------
            # trainer
            # --------------------------------------------------
            logger.info('-' * 100)
            logger.info('Initializing the running configurations')
            training_args = Seq2SeqTrainingArguments(output_dir=os.path.join(args.pre_train_output_root, task),
                                                     overwrite_output_dir=True,
                                                     do_train=True,
                                                     per_device_train_batch_size=args.batch_size,
                                                     gradient_accumulation_steps=1,
                                                     learning_rate=args.learning_rate,
                                                     weight_decay=args.lr_decay_rate,
                                                     max_grad_norm=args.grad_clipping_norm,
                                                     num_train_epochs=args.n_epoch,
                                                     lr_scheduler_type=SchedulerType.LINEAR,
                                                     warmup_steps=args.warmup_steps,
                                                     logging_dir=os.path.join(args.tensor_board_root, task),
                                                     logging_strategy=IntervalStrategy.STEPS,
                                                     logging_steps=args.logging_steps,
                                                     save_strategy=IntervalStrategy.NO,
                                                     seed=args.random_seed,
                                                     fp16=args.fp16,
                                                     dataloader_drop_last=False,
                                                     run_name=args.model_name,
                                                     load_best_model_at_end=True,
                                                     ignore_data_skip=False,
                                                     label_smoothing_factor=args.label_smoothing,
                                                     report_to=['tensorboard'],
                                                     dataloader_pin_memory=True)
            trainer = CodeTrainer(main_args=args,
                                  code_vocab=code_vocab,
                                  ast_vocab=ast_vocab,
                                  dfg_vocab=None,
                                  task=task,
                                  model=model,
                                  args=training_args,
                                  data_collator=None,
                                  train_dataset=dataset,
                                  tokenizer=code_vocab,
                                  model_init=None,
                                  compute_metrics=torch.nn.CrossEntropyLoss(reduction='sum'),
                                  callbacks=[LogStateCallBack()])
            
            # #### DEBUG #####
            # print(code_vocab.token2id)
            # print('-'*100)
            # loader = trainer.get_train_dataloader()
            
            # print(trainer.train_dataset[0])
            # for batch in loader:
            #     print(batch['input_ids'][0])
            #     break

            # print(trainer.train_dataset[1])
            # for batch in loader:
            #     print(batch['input_ids'][1])
            #     break
            # return
            # #### DEBUG #####
            
            logger.info('Running configurations initialized successfully')

            # --------------------------------------------------
            # train
            # --------------------------------------------------
            logger.info('-' * 100)
            logger.info(f'Start pre-training task: {task}')
            # model device
            logger.info('Device: {}'.format(next(model.parameters()).device))
            mass_result = trainer.train()
            logger.info(f'Pre-training task {task} finished')
            trainer.save_model(os.path.join(args.model_root, task))

    logger.info('Pre-training finished')

    return model, (code_vocab, ast_vocab, None)
