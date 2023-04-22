import torch.nn as nn
import torch
from torch.optim import AdamW
from transformers import BartConfig, get_scheduler, SchedulerType
from torch.utils.data.dataloader import DataLoader
from accelerate import Accelerator

import logging
from typing import Union, Tuple
import os
import math
from tqdm import tqdm
import pickle 

from sklearn.metrics import precision_score
from sklearn.metrics import recall_score
from sklearn.metrics import f1_score
from sklearn.metrics import accuracy_score
import numpy as np

import enums
from models.bart import BartForClassificationAndGeneration
from data.vocab import Vocab, load_vocab, init_vocab
from data.dataset import init_dataset
from utils.general import count_params, human_format, layer_wise_parameters
from data.data_collator import collate_fn
from utils.early_stopping import EarlyStopping

from downstream_tasks.augmentation.replace import create_sample
from downstream_tasks.augmentation.code2ast import code2xsbt

import json

logger = logging.getLogger(__name__)


def create_samples(data_path, update_ratio=0):
    dataset = []
    with open(data_path, 'r') as f:
        for line in tqdm(f):
            path=json.loads(line.strip())['path']
            code, label = create_sample(path,update_ratio)

            xsbt =  code2xsbt(code, enums.LANG_C)
            dataset.append((code, xsbt, label))

    return dataset


def run_pragma_trainer(
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
    logger.info(f'Code search on language: {args.search_language}')
    # accelerator
    accelerator = Accelerator()
    # --------------------------------------------------
    # datasets
    # --------------------------------------------------
    logger.info('-' * 100)
    logger.info('Loading datasets')
    datasets = dict()
    splits = ['train', 'valid']
    for split in splits:    
        datasets[split] = create_samples(os.path.join('/home/talkad/Desktop/pragformer_projects/a_code/data_creator/json_c_cpp', f'{split}.jsonl'))
                    
        logger.info(f'The size of {split} set: {len(datasets[split])}')
        logger.info(datasets[split][2])

    logger.info('Datasets loaded successfully')

    # --------------------------------------------------
    # vocabs
    # --------------------------------------------------
    logger.info('-' * 100)
    if trained_vocab:
        if isinstance(trained_vocab, tuple):
            logger.info('Vocabularies are passed through parameter')
            assert len(trained_vocab) == 3
            code_vocab, ast_vocab, nl_vocab = trained_vocab
        else:
            logger.info('Loading vocabularies from files')
            code_vocab = load_vocab(vocab_root=trained_vocab, name=args.code_vocab_name)
            ast_vocab = load_vocab(vocab_root=trained_vocab, name=args.ast_vocab_name)
            nl_vocab = load_vocab(vocab_root=trained_vocab, name=args.nl_vocab_name)

    logger.info(f'The size of code vocabulary: {len(code_vocab)}')
    logger.info(f'The size of nl vocabulary: {len(nl_vocab)}')
    logger.info(f'The size of ast vocabulary: {len(ast_vocab)}')
    logger.info('Vocabularies built successfully')

    # --------------------------------------------------
    # model
    # --------------------------------------------------
    logger.info('-' * 100)
    if trained_model:
        if isinstance(trained_model, BartForClassificationAndGeneration):
            logger.info('Model is passed through parameter')
            model = trained_model
        else:
            logger.info('Loading the model from file')
            logger.info(trained_model)
            config = BartConfig.from_json_file(os.path.join(trained_model, 'config.json'))
            model = BartForClassificationAndGeneration.from_pretrained(os.path.join(trained_model, 'pytorch_model.bin'),
                                                                       config=config)
    else:
        logger.info('Building the model')
        config = BartConfig(vocab_size=len(code_vocab) + len(ast_vocab) + len(nl_vocab),
                            max_position_embeddings=1024,
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
                            max_length=args.max_code_len,
                            min_length=1,
                            num_beams=args.beam_width,
                            num_labels=2)
        model = BartForClassificationAndGeneration(config, mode=enums.MODEL_MODE_SEARCH)

    model.set_model_mode(enums.MODEL_MODE_CLS)  # change to classification
    # log model statistic
    logger.info('Trainable parameters: {}'.format(human_format(count_params(model))))
    table = layer_wise_parameters(model)
    logger.debug('Layer-wised trainable parameters:\n{}'.format(table))
    logger.info('Model built successfully')

    # --------------------------------------------------
    # trainer
    # --------------------------------------------------
    if not only_test:
        logger.info('-' * 100)
        logger.info('Initializing the running configurations')
        gradient_accumulation_steps = 1

        # dataloader
        dataloader = DataLoader(dataset=datasets['train'],
                                batch_size=args.batch_size,
                                collate_fn=lambda batch: collate_fn(batch,
                                                                    args=args,
                                                                    task=enums.TASK_PRAGMA,
                                                                    code_vocab=code_vocab,
                                                                    nl_vocab=nl_vocab,
                                                                    ast_vocab=ast_vocab))

        valid_dataloader = DataLoader(dataset=datasets['valid'],
                                      batch_size=args.eval_batch_size,
                                      collate_fn=lambda batch: collate_fn(batch,
                                                                          args=args,
                                                                          task=enums.TASK_PRAGMA,
                                                                          code_vocab=code_vocab,
                                                                          nl_vocab=nl_vocab,
                                                                          ast_vocab=ast_vocab))
        # optimizer
        no_decay = ["bias", "LayerNorm.weight"]
        optimizer_grouped_parameters = [
            {
                "params": [p for n, p in model.named_parameters() if not any(nd in n for nd in no_decay)],
                "weight_decay": args.lr_decay_rate,
            },
            {
                "params": [p for n, p in model.named_parameters() if any(nd in n for nd in no_decay)],
                "weight_decay": 0.0,
            },
        ]
        optimizer = AdamW(optimizer_grouped_parameters, lr=args.learning_rate)
        # Prepare everything with `accelerator`
        model, optimizer, dataloader, valid_dataloader = accelerator.prepare(
            model, optimizer, dataloader, valid_dataloader
        )
        # Scheduler and math around the number of training steps.
        num_update_steps_per_epoch = math.ceil(len(dataloader) / gradient_accumulation_steps)
        max_train_steps = args.n_epoch * num_update_steps_per_epoch

        lr_scheduler = get_scheduler(name=SchedulerType.LINEAR,
                                     optimizer=optimizer,
                                     num_warmup_steps=args.warmup_steps,
                                     num_training_steps=max_train_steps)
        # early stopping
        early_stopping = EarlyStopping(patience=args.early_stop_patience, higher_better=True)
        logger.info('Running configurations initialized successfully')

        # --------------------------------------------------
        # train
        # --------------------------------------------------
        logger.info('-' * 100)
        logger.info('Start training')

        total_batch_size = args.batch_size * accelerator.num_processes * gradient_accumulation_steps

        logger.info("***** Running training *****")
        # logger.info(f"  Num examples = {len(datasets['train'])}")
        logger.info(f"  Num Epochs = {args.n_epoch}")
        logger.info(f"  Instantaneous batch size per device = {args.batch_size}")
        logger.info(f"  Total train batch size (w. parallel, distributed & accumulation) = {total_batch_size}")
        logger.info(f"  Gradient Accumulation steps = {gradient_accumulation_steps}")
        logger.info(f"  Total optimization steps = {max_train_steps}")

        progress_bar = tqdm(range(max_train_steps))
        completed_steps = 0

        ratios = [0, 0.05, 0.15, 0.3, 0.4]

        for epoch in range(args.n_epoch):

            ratio = ratios[epoch] if epoch<len(ratios) else 0.45
            # ratio = 1

            datasets['train'] = create_samples('/home/talkad/Desktop/pragformer_projects/a_code/data_creator/json_c_cpp/train.jsonl', update_ratio=ratio)
            split = 'train'
            logger.info(f'The size of {split} set: {len(datasets[split])}')
            logger.info(datasets[split][2])
            
            dataloader = DataLoader(dataset=datasets['train'],
                                batch_size=args.batch_size,
                                collate_fn=lambda batch: collate_fn(batch,
                                                                    args=args,
                                                                    task=enums.TASK_PRAGMA,
                                                                    code_vocab=code_vocab,
                                                                    nl_vocab=nl_vocab,
                                                                    ast_vocab=ast_vocab))
            model, optimizer, dataloader, valid_dataloader = accelerator.prepare(
                model, optimizer, dataloader, valid_dataloader
                )

            model.train()
            for step, batch in enumerate(dataloader):
                loss = model(**batch)

                loss = loss.loss # TAL

                loss = loss / gradient_accumulation_steps
                accelerator.backward(loss)

                if step % args.log_state_every == 0 and step != 0:
                    logger.info('loss: {:.4f}'.format(loss.item()))

                if step % gradient_accumulation_steps == 0 or step == len(dataloader) - 1:
                    # Gradient clipping
                    if args.grad_clipping_norm is not None and args.grad_clipping_norm > 0:

                        if hasattr(optimizer, "clip_grad_norm"):
                            # Some optimizers (like the sharded optimizer) have a specific way to do gradient clipping
                            optimizer.clip_grad_norm(args.grad_clipping_norm)
                        elif hasattr(model, "clip_grad_norm_"):
                            # Some models (like FullyShardedDDP) have a specific way to do gradient clipping
                            model.clip_grad_norm_(args.grad_clipping_norm)
                        else:
                            # Revert to normal clipping otherwise, handling Apex or full precision
                            nn.utils.clip_grad_norm_(model.parameters(), args.grad_clipping_norm)

                    optimizer.step()
                    lr_scheduler.step()
                    optimizer.zero_grad()
                    progress_bar.update(1)
                    completed_steps += 1

                if completed_steps >= max_train_steps:
                    break


            model.eval()
            logger.info("***** Running evaluation *****")
            correct = 0
            total = 0
            logits_list=[] 
            labels_list=[]
            for step, batch in tqdm(enumerate(valid_dataloader)):
                loss = model(**batch)

                logits = loss.logits
                labels = batch['labels']
                preds = torch.argmax(logits, dim=1)
                correct += (preds == labels).float().sum()
                total += len(preds)

                logits_list.append(preds.cpu().numpy())
                labels_list.append(labels.cpu().numpy())

            # print('vsss', len(valid_dataloader), total)
            logger.info('accuracy: ' + str(correct.item()/total))

            preds=np.concatenate(logits_list,0)
            labels=np.concatenate(labels_list,0)

            recall=recall_score(labels, preds)
            precision=precision_score(labels, preds)   
            f1=f1_score(labels, preds)        
            accuracy=accuracy_score(labels, preds)

            result = {
                "eval_recall": float(recall),
                "eval_precision": float(precision),
                "eval_f1": float(f1),
                "eval_acc": float(accuracy)
                }

            logger.info("***** Eval results *****")
            for key in sorted(result.keys()):
                logger.info("  %s = %s", key, str(round(result[key],4)))