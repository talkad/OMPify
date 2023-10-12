import torch.utils.data
from torch.utils.data.dataset import Dataset

import os
import random
import logging
import pickle
import random

import enums
from .data_utils import load_dataset_from_dir, parse_for_pragma_gen
from data.vocab import Vocab

logger = logging.getLogger(__name__)


class CodeDataset(Dataset):

    def __init__(self, args, dataset_name, mode, task=None, language=None, split=None):
        """
        Initialization definition.

        Args:
            args (argparse.Namespace): Arguments
            dataset_name (str): Name of the dataset
            mode (str): Training mode, ``pre_train`` or ``fine_tune``
            task (str): Dataset mode, support pre-training tasks: ['cap', 'mass', 'mnp'],
                and downstream fine-tuning task: ['translation'],
            language (str): Only for downstream fine-tuning
            split (str): Only for downstream fine-tuning, support ['train', 'valid', 'test', 'codebase']

        """
        super(CodeDataset, self).__init__()
        self.args = args
        self.dataset_name = dataset_name
        self.task = task
        self.mode = mode
        self.split = split
        self.paths = {}

        # dataset dir for files, all files in this dir meeting the filename will be used as dataset files
        self.dataset_dir = os.path.join(args.dataset_root, self.mode)

        # load pre-training dataset
        if self.mode == 'pre_train':
            self.paths, self.languages, self.sources, self.source_tokens, self.replaced, self.replaced_tokens, self.asts = load_dataset_from_dir(args, dataset_dir=self.dataset_dir)
            self.size = len(self.sources)
        # load fine-tuning dataset
        else:
            assert split
            logger.info(f'  Loading {split} set')
            self.dataset_dir = os.path.join(self.dataset_dir, task)

            # pragma generation
            if task == enums.TASK_PRAGMA:
                self.source_tokens, self.pragma_tokens = parse_for_pragma_gen(
                    dataset_path=args.pragma_dataset_path,
                    lang=language,
                    split=split,
                    is_replaced=not args.no_replaced
                    )

                assert len(self.source_tokens) == len(self.pragma_tokens)
                self.size = len(self.source_tokens)

    def get_codes(self):
        # selection of code representation
        if self.args.no_replaced:
            codes = self.source_tokens
        else:
            codes = self.replaced_tokens

        return codes

    def __getitem__(self, index):            

        # cap
        if self.task == enums.TASK_CODE_AST_PREDICTION:
            codes = self.get_codes()
            is_ast = random.random() < 0.5
            if is_ast:
                return codes[index], self.asts[index], 1
            else:
                other_ast = self.asts[random.randint(0, self.size - 1)]
                while other_ast == self.asts[index]:
                    other_ast = self.asts[random.randint(0, self.size - 1)]
                return codes[index], other_ast, 0
        # mass
        elif self.task == enums.TASK_MASS:
            codes = self.get_codes()
            code_tokens = codes[index].split()
            mask_len = int(self.args.mass_mask_ratio * len(code_tokens))
            mask_start = random.randint(0, len(code_tokens) - mask_len)
            mask_tokens = code_tokens[mask_start: mask_start + mask_len]
            input_tokens = code_tokens[:mask_start] + [Vocab.MSK_TOKEN] + code_tokens[mask_start + mask_len:]
            return ' '.join(input_tokens), self.asts[index], ' '.join(mask_tokens)

        # pragma generation
        elif self.task == enums.TASK_PRAGMA:
            return self.source_tokens[index],  self.pragma_tokens[index]
        
    def __len__(self):
        return self.size

    def set_task(self, task):
        self.task = task

    def save(self):
        """Save to binary pickle file"""
        path = os.path.join(self.args.dataset_save_dir, f'{self.dataset_name}.pk')
        with open(path, mode='wb') as f:
            pickle.dump(self, f)
        logger.info(f'Dataset saved to {path}')

    def subset(self, ratio):
        """
        Return a subset of self.

        Args:
            ratio (float): The ratio of size, must greater than 0 and less than/equal to 1

        Returns:
            Dataset: the subset

        """
        assert 0 < ratio <= 1, f'The subset ratio supposed to be 0 < ratio <= 1, but got ratio={ratio}'
        if ratio == 1:
            return self
        indices = random.sample(range(self.size), int(self.size * ratio))
        return torch.utils.data.Subset(self, indices)


def init_dataset(args, mode, task=None, language=None, split=None, load_if_saved=True) -> CodeDataset:
    """
    Find dataset, if the dataset is saved, load and return, else initialize and return.

    Args:
        args (argparse.Namespace): Arguments
        mode (str): Training mode, ``pre_train`` or ``fine_tune``
        task (str): Dataset mode, support pre-training tasks: ['cap', 'mass', 'mnp'],
            and downstream fine-tuning task: ['summarization', 'translation'],
            future support ['completion', 'search', 'clone', 'summarization', 'translation']
        language (str): Only for downstream fine-tuning
        split (str): Only for downstream fine-tuning, support ['train', 'valid', 'test', 'codebase(only for search)']
        load_if_saved (bool): Whether to load the saved instance if it exists, default to True

    Returns:
        CodeDataset: Loaded or initialized dataset

    """
    # name = 'fortran'
    # path = os.path.join(args.dataset_save_dir, f'{name}.pk')

    # with open(path, mode='rb') as f:
    #     obj = pickle.load(f)
    # assert isinstance(obj, CodeDataset)
    # obj.args = args
    # logger.info(f'Dataset instance loaded from: {path}')
    # print_paths(obj.paths)
    # return obj

    name = '.'.join([sub_name for sub_name in [mode, task, language, split] if sub_name is not None])
    if load_if_saved:
        path = os.path.join(args.dataset_save_dir, f'{name}.pk')
        if os.path.exists(path) and os.path.isfile(path):
            logger.info(f'Trying to load saved binary pickle file from: {path}')
            with open(path, mode='rb') as f:
                obj = pickle.load(f)
            assert isinstance(obj, CodeDataset)
            obj.args = args
            logger.info(f'Dataset instance loaded from: {path}')
            print_paths(obj.paths)
            return obj

    dataset = CodeDataset(args=args,
                          dataset_name=name,
                          mode=mode,
                          task=task,
                          language=language,
                          split=split)
    # dataset.save()
    return dataset


def print_paths(paths):
    """
    Print paths.

    Args:
        paths (dict): Dict mapping path group to path string or list of path strings.

    """
    logger.info('Dataset loaded from these files:')
    for key, value in paths.items():
        if isinstance(value, list):
            for v in value:
                logger.info(f'  {key}: {v}')
        else:
            logger.info(f'  {key}: {value}')

