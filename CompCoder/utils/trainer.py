
from torch.utils.data.dataloader import DataLoader
from torch.utils.data.dataset import Dataset
# from transformers import Seq2SeqTrainer, Trainer
import transformers
from transformers.trainer import *
import torch
import argparse
from typing import Optional

from data.data_collator import collate_fn


class CodeTrainer(transformers.Seq2SeqTrainer):

    def __init__(self, main_args: argparse.Namespace, code_vocab, ast_vocab, dfg_vocab, task, **kwargs):
        bullshit = torch.nn.Parameter(torch.ones(1))
        kwargs['model'].register_parameter('bullshit', param=bullshit)
        super(CodeTrainer, self).__init__(**kwargs)
        self.main_args = main_args
        self.code_vocab = code_vocab
        self.ast_vocab = ast_vocab
        self.dfg_vocab = dfg_vocab
        self.task = task

        self.bullshit = bullshit


    def compute_loss(self, model, inputs, return_outputs=False):
        """
        How the loss is computed by Trainer. By default, all models return the loss in the first element.

        Subclass and override for custom behavior.
        """
        #(['input_ids', 'attention_mask', 'decoder_input_ids', 'decoder_attention_mask', 'labels']
        # print(model)
        with torch.no_grad():
            labels=inputs.pop('labels')
            logits = model(**inputs).logits
            assert logits.shape[1]==inputs['decoder_input_ids'].shape[1]
            assert labels.dtype==torch.int64
            loss_fn=torch.nn.CrossEntropyLoss(reduction='sum')
            mask=inputs['decoder_attention_mask']
            # loss=loss_fn( logits[mask].view([-1,50152]),labels[mask].view(-1))
            loss=loss_fn( logits[mask].view([-1,1334]),labels[mask].view(-1))
        #print(inputs.keys())
        # loss.grad_fn=lambda:None

        #print(inputs)
        # print("Im hereeeee xxxxxxxxxxxxxxxxxxxxxxxxxx")
        return loss*self.bullshit
    

    def get_train_dataloader(self) -> DataLoader:
        """
        Returns the training :class:`~torch.utils.data.DataLoader`.

        Will use no sampler if :obj:`self.train_dataset` does not implement :obj:`__len__`, a random sampler (adapted
        to distributed training if necessary) otherwise.

        Subclass and override this method if you want to inject some custom behavior.
        """

        return DataLoader(dataset=self.train_dataset,
                          batch_size=self.main_args.batch_size,
                          shuffle=True,
                          collate_fn=lambda batch: collate_fn(batch,
                                                              args=self.main_args,
                                                              task=self.task,
                                                              code_vocab=self.code_vocab,
                                                              ast_vocab=self.ast_vocab,
                                                              dfg_vocab=self.dfg_vocab))

    def get_eval_dataloader(self, eval_dataset: Optional[Dataset] = None) -> DataLoader:
        if eval_dataset:
            self.eval_dataset = eval_dataset
        return DataLoader(dataset=self.eval_dataset,
                          batch_size=self.main_args.eval_batch_size,
                          collate_fn=lambda batch: collate_fn(batch,
                                                              args=self.main_args,
                                                              task=self.task,
                                                              code_vocab=self.code_vocab,
                                                              ast_vocab=self.ast_vocab,
                                                              dfg_vocab=self.dfg_vocab))

    def get_test_dataloader(self, test_dataset: Dataset) -> DataLoader:
        return DataLoader(dataset=test_dataset,
                          batch_size=self.main_args.eval_batch_size,
                          collate_fn=lambda batch: collate_fn(batch,
                                                              args=self.main_args,
                                                              task=self.task,
                                                              code_vocab=self.code_vocab,
                                                              ast_vocab=self.ast_vocab,
                                                              dfg_vocab=self.dfg_vocab))

    def set_task(self, task):
        self.task = task


class CodeCLSTrainer(transformers.Trainer):

    def __init__(self, main_args: argparse.Namespace, code_vocab, ast_vocab, dfg_vocab, task, **kwargs):
        super(CodeCLSTrainer, self).__init__(**kwargs)
        self.main_args = main_args
        self.code_vocab = code_vocab
        self.ast_vocab = ast_vocab
        self.dfg_vocab = dfg_vocab
        self.task = task

    def get_train_dataloader(self) -> DataLoader:
        """
        Returns the training :class:`~torch.utils.data.DataLoader`.

        Will use no sampler if :obj:`self.train_dataset` does not implement :obj:`__len__`, a random sampler (adapted
        to distributed training if necessary) otherwise.

        Subclass and override this method if you want to inject some custom behavior.
        """

        return DataLoader(dataset=self.train_dataset,
                          batch_size=self.main_args.batch_size,
                          shuffle=True,
                          collate_fn=lambda batch: collate_fn(batch,
                                                              args=self.main_args,
                                                              task=self.task,
                                                              code_vocab=self.code_vocab,
                                                              ast_vocab=self.ast_vocab,
                                                              dfg_vocab=self.dfg_vocab))

    def get_eval_dataloader(self, eval_dataset: Optional[Dataset] = None) -> DataLoader:
        if eval_dataset:
            self.eval_dataset = eval_dataset
        return DataLoader(dataset=self.eval_dataset,
                          batch_size=self.main_args.eval_batch_size,
                          collate_fn=lambda batch: collate_fn(batch,
                                                              args=self.main_args,
                                                              task=self.task,
                                                              code_vocab=self.code_vocab,
                                                              ast_vocab=self.ast_vocab,
                                                              dfg_vocab=self.dfg_vocab))

    def get_test_dataloader(self, test_dataset: Dataset) -> DataLoader:
        return DataLoader(dataset=test_dataset,
                          batch_size=self.main_args.eval_batch_size,
                          collate_fn=lambda batch: collate_fn(batch,
                                                              args=self.main_args,
                                                              task=self.task,
                                                              code_vocab=self.code_vocab,
                                                             ast_vocab=self.ast_vocab,
                                                              dfg_vocab=self.dfg_vocab))

    def set_task(self, task):
        self.task = task
