import logging
from typing import Union, Tuple

import enums
from models.bart import BartForClassificationAndGeneration
from data.vocab import Vocab

from CompCoder.downstream_tasks.pragma_gen import run_translation

logger = logging.getLogger(__name__)


def train(
        args,
        trained_model: Union[BartForClassificationAndGeneration, str] = None,
        trained_vocab: Union[Tuple[Vocab, Vocab, Vocab], str] = None):
    """
    Fine-tuning from given pre-trained model and vocabs, or training from scratch.

    Args:
        args (argparse.Namespace): Arguments
        trained_model (Union[BartForClassificationAndGeneration, str]): Optional,
            instance or directory of ``BartForClassificationAndGeneration``, must given when ``only_test`` is True
        trained_vocab (Union[Tuple[Vocab, Vocab, Vocab], str]): Optional, Tuple of instances or directory of three
            vocabularies, must given when ``only_test`` is True

    """
    task = args.task.lower()
    assert task in enums.ALL_DOWNSTREAM_TASKS, f'Downstream task {task} is not supported.'

    if args.train_from_scratch:
        args.trained_model = None
        args.trained_vocab = None
        trained_model = None
        trained_vocab = None

    if trained_model is None and args.trained_model is not None:
        trained_model = args.trained_model
    assert not args.only_test or isinstance(trained_model, str) or \
           isinstance(trained_model, BartForClassificationAndGeneration), \
           f'The model type is not supported, expect Bart model or string of model dir, got {type(trained_model)}'

    if trained_vocab is None and args.trained_vocab is not None:
        trained_vocab = args.trained_vocab
    assert not args.only_test or isinstance(trained_vocab, str) or isinstance(trained_vocab, tuple), \
        f'The vocab type is not supported, expect tuple or string of path, got {type(trained_vocab)}'

    logger.info('*' * 100)
    if trained_model:
        logger.info('Fine-tuning from pre-trained model and vocab')
        if isinstance(trained_model, str):
            logger.info(f'Model dir: {trained_model}')
        if isinstance(trained_vocab, str):
            logger.info(f'Vocab dir: {trained_vocab}')
    else:
        logger.info('Training from scratch')

    # start downstream task
    if task == enums.TASK_PRAGMA:
        run_pragma_gen(args=args,
                        trained_model=trained_model,
                        trained_vocab=trained_vocab,
                        only_test=args.only_test)