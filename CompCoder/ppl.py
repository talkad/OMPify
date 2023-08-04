import pygments 
from pygments.lexers import get_lexer_by_name

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


def get_lex_count(texts,lang,show_lex=True):
	lexer = get_lexer_by_name(lang)
	if show_lex:
		print(f'the count is made with: {lexer}')
	pygments_len=sum(len(list(pygments.lex(t, lexer))) for t in texts)
	lex_vocab = sum(len(v) for v in lexer.tokens.values())
	return pygments_len*lex_vocab



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

    print(get_lex_count(dataset.source_tokens, 'fortran'))


    config = BartConfig.from_pretrained('/home/1010/talkad/OMPify/outputs/wed_pre_train_fortran_mass_20230802_235207/models/mass/config.json')
    model = BartForClassificationAndGeneration(config)
    model.load_state_dict(torch.load('/home/1010/talkad/OMPify/outputs/wed_pre_train_fortran_mass_20230802_235207/models/mass/pytorch_model.bin'))


    # 1249955746
    # dataset len = 56024

    # 4.482078679927921e-05
    # 1.00010933053841




    # 1.1264629782033304
    # >>> a = 171900 / 1249955746
    # >>> a
    # 0.00013752486882043582
    # >>> b = a * 7003
    # >>> b
    # 0.9630866563495121
    # >>> np.exp(b)
    # 2.619770337136881

    lex = 1249955746
    loss = 171900

    num_batch = 7003
    ppl = np.exp(loss * num_batch / lex)

    # mass = 164003.098 -> 2.506390038531169
    # mass_cap = 178776.58927870198 -> 2.7226711097930782

    # mass_tokom = 13812.332727938026 -> 1.0804579213791219
    # mass_cap_tokom = 13817.63995899079 -> 1.0804900484993762




	
