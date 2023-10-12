
import torch

from typing import List
import itertools

from data.vocab import Vocab
import enums

from .tokompiler import Tokompiler


def collate_fn(batch, args, task, code_vocab, ast_vocab, dfg_vocab):
    """
    Data collator function.

    Args:
        batch (list):
        args (argparse.Namespace):
        task (str):
        code_vocab (Vocab):
        nl_vocab (Vocab):
        ast_vocab (Vocab):

    Returns:
        dict: Model inputs

    """

    model_inputs = {}
    # cap
    if task == enums.TASK_CODE_AST_PREDICTION:
        code_raw, ast_raw, is_ast = map(list, zip(*batch))

        model_inputs['input_ids'], model_inputs['attention_mask'] = get_concat_batch_inputs(
            code_raw=code_raw,
            code_vocab=code_vocab,
            max_code_len=args.max_code_len,
            ast_raw=ast_raw,
            ast_vocab=ast_vocab,
            max_ast_len=args.max_ast_len,
            dfg_raw=[],
            dfg_vocab=dfg_vocab,
            max_dfg_len=args.max_dfg_len,
            no_ast=args.no_ast,
            no_dfg=args.no_dfg
        )
        model_inputs['labels'] = torch.tensor(is_ast, dtype=torch.long)
    # mass
    elif task == enums.TASK_MASS:
        code_raw, ast_raw, target_raw = map(list, zip(*batch))

        model_inputs['input_ids'], model_inputs['attention_mask'] = get_concat_batch_inputs(
            code_raw=code_raw,
            code_vocab=code_vocab,
            max_code_len=args.max_code_len,
            ast_raw=ast_raw,
            ast_vocab=ast_vocab,
            max_ast_len=args.max_ast_len,
            dfg_raw=[],
            dfg_vocab=dfg_vocab,
            max_dfg_len=args.max_dfg_len,
            no_ast=args.no_ast,
            no_dfg=args.no_dfg
        )
        model_inputs['decoder_input_ids'], model_inputs['decoder_attention_mask'] = get_batch_inputs(
            batch=target_raw,
            vocab=code_vocab,
            processor=Vocab.sos_processor,
            max_len=int(args.mass_mask_ratio * args.max_code_len)
        )
        model_inputs['labels'], _ = get_batch_inputs(batch=target_raw,
                                                     vocab=code_vocab,
                                                     processor=Vocab.eos_processor,
                                                     max_len=int(args.mass_mask_ratio * args.max_code_len))

    # pragma generation
    elif task == enums.TASK_PRAGMA:
        code_raw, target_raw = map(list, zip(*batch))

        model_inputs['input_ids'], model_inputs['attention_mask'] = get_concat_batch_inputs(
            code_raw=code_raw,
            code_vocab=code_vocab,
            max_code_len=args.max_code_len,
            ast_raw=None,
            ast_vocab=ast_vocab,
            max_ast_len=args.max_ast_len,
            dfg_raw=[],
            dfg_vocab=dfg_vocab,
            max_dfg_len=args.max_dfg_len,
            no_ast=args.no_ast,
            no_dfg=args.no_dfg
        )

        model_inputs['decoder_input_ids'], model_inputs['decoder_attention_mask'] = get_batch_inputs(
            batch=target_raw,
            vocab=code_vocab,
            processor=Vocab.sos_processor,
            tokom_processor='sos',
            max_len=args.max_code_len
        )

        model_inputs['labels'], _ = get_batch_inputs(batch=target_raw,
                                                     vocab=code_vocab,
                                                     processor=Vocab.eos_processor,
                                                     tokom_processor='eos',
                                                     max_len=args.max_code_len)

    return model_inputs


def get_batch_inputs(batch: List[str], vocab: Vocab, processor=None, tokom_processor=None, max_len=None):
    """
    Encode the given batch to input to the model.

    Args:
        batch (list[str]): Batch of sequence,
            each sequence is represented by a string or list of tokens
        vocab (Vocab): Vocab of the batch
        processor (tokenizers.processors.PostProcessor): Optional, post-processor method
        max_len (int): Optional, the maximum length of each sequence

    Returns:
        (torch.LongTensor, torch.LongTensor): Tensor of batch and mask, [B, T]

    """
    # set post processor
    if isinstance(vocab.tokenizer, Tokompiler):
        vocab.tokenizer.post_processor = tokom_processor
    else:
        vocab.tokenizer.post_processor = processor
    # set truncation
    if max_len:
        vocab.tokenizer.enable_truncation(max_length=max_len)
    else:
        vocab.tokenizer.no_truncation()
    # encode batch
    inputs, padding_mask = vocab.encode_batch(batch, pad=True, max_length=max_len)
    # to tensor
    inputs = torch.tensor(inputs, dtype=torch.long)
    padding_mask = torch.tensor(padding_mask, dtype=torch.long)

    return inputs, padding_mask


def get_concat_batch_inputs(code_raw, code_vocab, max_code_len,
                            ast_raw, ast_vocab, max_ast_len,
                            dfg_raw, dfg_vocab, max_dfg_len,
                            no_ast=False, no_dfg=False):
    """
    Return the concat tensor and mask for input.

    Args:
        code_raw:
        code_vocab:
        max_code_len:
        ast_raw:
        ast_vocab:
        max_ast_len:
        dfg_raw:
        dfg_vocab:
        max_dfg_len:
        no_ast:
        no_dfg:

    Returns:
        (torch.Tensor, torch.Tensor):
            - Concat inputs
            - concat attention mask

    """
    # import pdb ; pdb.set_trace()
    code_inputs, code_padding_mask = get_batch_inputs(batch=code_raw,
                                                      vocab=code_vocab,
                                                      processor=Vocab.sep_processor,
                                                      max_len=max_code_len)
    
    if not no_ast:
        ast_inputs, ast_padding_mask = get_batch_inputs(batch=ast_raw,
                                                        vocab=ast_vocab,
                                                        processor=Vocab.eos_processor if no_dfg else Vocab.sep_processor,
                                                        max_len=max_ast_len)
    else:
        ast_inputs, ast_padding_mask = None, None

    if not no_dfg:
        dfg_inputs, dfg_padding_mask = get_batch_inputs(batch=dfg_raw,
                                                      vocab=dfg_vocab,
                                                      processor=Vocab.eos_processor,
                                                      max_len=max_dfg_len)
    else:
        dfg_inputs, dfg_padding_mask = None, None

    inputs = torch.cat([inputs for inputs in [code_inputs, ast_inputs, dfg_inputs] if inputs is not None], dim=-1)
    padding_mask = torch.cat([mask for mask in [code_padding_mask, ast_padding_mask, dfg_padding_mask]
                              if mask is not None], dim=-1)

    return inputs, padding_mask


def pad_batch(batch, pad_value=0):
    """
    Pad a list of sequence to a padded 2d tensor.

    Args:
        batch (list[list[int]]): List of sequence
        pad_value (int): Optional, fill value, default to 0.

    Returns:
        torch.Tensor: Padded tensor. [B, T].

    """
    batch = list(zip(*itertools.zip_longest(*batch, fillvalue=pad_value)))
    return torch.tensor([list(b) for b in batch]).long()
