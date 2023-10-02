from transformers import BartConfig, Seq2SeqTrainingArguments, EarlyStoppingCallback, \
    IntervalStrategy, SchedulerType

import logging
from typing import Union, Tuple
import os
import torch
import enums
from models.bart import BartForClassificationAndGeneration
from data.vocab import Vocab, load_vocab, init_vocab
from data.dataset import init_dataset
from utils.general import count_params, human_format, layer_wise_parameters
from eval.metrics import bleu, meteor, rouge_l, avg_ir_metrics, accuracy_for_sequence
from utils.callbacks import LogStateCallBack
from utils.trainer import CodeTrainer

logger = logging.getLogger(__name__)


def run_pragma_gen(
        args,
        trained_model: Union[BartForClassificationAndGeneration, str] = None,
        trained_vocab: Union[Tuple[Vocab, Vocab, Vocab], str] = None,
        only_test=False):
    """
    Fine-tuning from given pre-trained model and vocabs, or training from scratch.

    Args:
        args (argparse.Namespace): Arguments
        trained_model (Union[BartForClassificationAndGeneration, str]): Optional,
            instance or directory of ``BartForClassificationAndGeneration``, must given when ``only_test`` is True
        trained_vocab (Union[Tuple[Vocab, Vocab, Vocab], str]): Optional, Tuple of instances or directory of three
            vocabularies, must given when ``only_test`` is True
        only_test (bool): True when only need to test, default to False

    """
    logger.info('-' * 100)
    logger.info(f'OpenMP pragma generation')
    # --------------------------------------------------
    # datasets
    # --------------------------------------------------
    logger.info('-' * 100)
    logger.info('Loading datasets')
    datasets = dict()
    splits = ['test'] if only_test else ['train',  'test']
    for split in splits:
        datasets[split] = init_dataset(args=args,
                                       mode=enums.TRAINING_MODE_FINE_TUNE,
                                       task=enums.TASK_PRAGMA,
                                       language=args.langs,
                                       split=split)
        logger.info(f'The size of {split} set: {len(datasets[split])}')
    if args.train_subset_ratio and 'train' in datasets:
        datasets['train'] = datasets['train'].subset(args.train_subset_ratio)
        logger.info(f'The train is trimmed to subset due to the argument: train_subset_ratio={args.train_subset_ratio}')
        logger.info('The size of trimmed train set: {}'.format(len(datasets['train'])))
    logger.info('Datasets loaded successfully')

      # --------------------------------------------------
    # vocabs
    # --------------------------------------------------
    logger.info('-' * 100)
    logger.info('Loading vocabularies from files')
    
    if args.no_replaced:
        code_vocab = load_vocab(vocab_root=trained_vocab, name='code.bpe.50000.None') #name=args.code_vocab_name)
    else:
        code_vocab = load_vocab(vocab_root=trained_vocab, name=args.replaced_code_vocab_name)

    ast_vocab = load_vocab(vocab_root=trained_vocab, name='ast.word.None.50000') #name=args.ast_vocab_name)
        
    logger.info(f'The size of code vocabulary: {len(code_vocab)}')
    logger.info(f'The size of ast vocabulary: {len(ast_vocab)}')
    logger.info('Vocabularies built successfully')


    # --------------------------------------------------
    # model
    # --------------------------------------------------

    logger.info('Loading the model from file')
    config = BartConfig.from_pretrained('/home/talkad/shared/models/fortran_mass/models/mass/config.json')
    model = BartForClassificationAndGeneration(config, mode=enums.MODEL_MODE_GEN)
    model.load_state_dict(torch.load('/home/talkad/shared/models/fortran_mass/models/mass/pytorch_model.bin'))
    
    # log model statistic
    logger.info('Trainable parameters: {}'.format(human_format(count_params(model))))
    table = layer_wise_parameters(model)
    logger.debug('Layer-wised trainable parameters:\n{}'.format(table))
    logger.info('Model built successfully')


    # --------------------------------------------------
    # trainer
    # --------------------------------------------------
    logger.info('-' * 100)
    logger.info('Initializing the running configurations')

    def decode_preds(preds):
        preds, labels = preds
        decoded_preds = code_vocab.decode_batch(preds)
        decoded_labels = code_vocab.decode_batch(labels)
        return decoded_labels, decoded_preds

    # compute metrics
    def compute_test_metrics(eval_preds):
        decoded_labels, decoded_preds = decode_preds(eval_preds)
        print(decoded_labels, decoded_preds)
        return 1

        result = {'references': decoded_labels, 'candidates': decoded_preds}
        refs = [ref.strip().split() for ref in decoded_labels]
        cans = [can.strip().split() for can in decoded_preds]
        result.update(bleu(references=refs, candidates=cans))
        try:
            result.update(meteor(references=refs, candidates=cans))
        except Exception:
            pass
        result.update(rouge_l(references=refs, candidates=cans))
        result.update(avg_ir_metrics(references=refs, candidates=cans))
        result.update(accuracy_for_sequence(references=refs, candidates=cans))
        return result

    training_args = Seq2SeqTrainingArguments(output_dir=os.path.join(args.checkpoint_root, enums.TASK_PRAGMA),
                                             overwrite_output_dir=True,
                                             do_train=True,
                                             do_eval=True,
                                             do_predict=True,
                                             evaluation_strategy=IntervalStrategy.EPOCH,
                                             prediction_loss_only=False,
                                             per_device_train_batch_size=args.batch_size,
                                             per_device_eval_batch_size=args.eval_batch_size,
                                             gradient_accumulation_steps=args.gradient_accumulation_steps,
                                             learning_rate=args.learning_rate,
                                             weight_decay=args.lr_decay_rate,
                                             max_grad_norm=args.grad_clipping_norm,
                                             num_train_epochs=args.n_epoch,
                                             lr_scheduler_type=SchedulerType.LINEAR,
                                             warmup_steps=args.warmup_steps,
                                             logging_dir=os.path.join(args.tensor_board_root, enums.TASK_PRAGMA),
                                             logging_strategy=IntervalStrategy.STEPS,
                                             logging_steps=args.logging_steps,
                                             save_strategy=IntervalStrategy.EPOCH,
                                             save_total_limit=5,
                                             seed=args.random_seed,
                                             fp16=args.fp16,
                                             dataloader_drop_last=False,
                                             run_name=args.model_name,
                                             load_best_model_at_end=True,
                                            #  metric_for_best_model='accuracy',
                                             greater_is_better=True,
                                             ignore_data_skip=False,
                                             label_smoothing_factor=args.label_smoothing,
                                             report_to=['tensorboard'],
                                             dataloader_pin_memory=True,
                                             predict_with_generate=True)

    trainer = CodeTrainer(main_args=args,
                          code_vocab=code_vocab,
                          ast_vocab=ast_vocab,
                          dfg_vocab=None,
                          task=enums.TASK_PRAGMA,
                          model=model,
                          args=training_args,
                          data_collator=None,
                          train_dataset=datasets['train'] if 'train' in datasets else None,
                          eval_dataset=datasets['test'] if 'test' in datasets else None,
                          tokenizer=None,
                          model_init=None,
                          compute_metrics=compute_test_metrics,
                          callbacks=[
                              EarlyStoppingCallback(early_stopping_patience=args.early_stop_patience),
                              LogStateCallBack()])
    logger.info('Running configurations initialized successfully')

    # --------------------------------------------------
    # train
    # --------------------------------------------------
    if not only_test:
        logger.info('-' * 100)
        logger.info('Start training')
        # import pdb; pdb.set_trace()

        train_result = trainer.train()
        logger.info('Training finished')
        trainer.save_model(args.model_root)
        trainer.save_state()
        metrics = train_result.metrics
        trainer.log_metrics(split='train', metrics=metrics)
        trainer.save_metrics(split='train', metrics=metrics)


    # --------------------------------------------------
    # test
    # --------------------------------------------------
    logger.info('-' * 100)
    logger.info('Initializing the running configurations')

    collate_func=lambda batch: collate_fn(batch,
                                    args=args,
                                    task=enums.TASK_PRAGMA,
                                    code_vocab=code_vocab,
                                    ast_vocab=ast_vocab,
                                    dfg_vocab=None)

    total_loss = 0
    total_tokens = 0
    for idx, data in enumerate(dataset):
        if idx == 200:
            break

        inputs = collate_func([data])
        labels = inputs['labels']
        labels_amount = labels.nonzero().size(0)
        output = model(**inputs)

        total_tokens += labels_amount*2
        total_loss += output.loss.item()

    print(total_loss/total_tokens)
    print(np.exp(total_loss/total_tokens))
    print('='*50)
