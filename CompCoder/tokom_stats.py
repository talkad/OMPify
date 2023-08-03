
import logging

import enums
from data.dataset import init_dataset
from data.vocab import  init_vocab, load_vocab
import sys
from tqdm import tqdm
import data.asts.convert_representation as cr
import data.asts.parse_tools as parse_tools
from data.asts.tokenizer import Tokompiler, TokenizerBPE



logger = logging.getLogger(__name__)


def stats(args):

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
    # trained_vocab = None
    # logger.info('-' * 100)
    # if trained_vocab:
    #     logger.info('Loading vocabularies from files')

    #     if args.no_replaced:
    #         code_vocab = load_vocab(vocab_root=trained_vocab, name='code.bpe.50000.None') #name=args.code_vocab_name)
    #     else:
    #         code_vocab = load_vocab(vocab_root=trained_vocab, name=args.replaced_code_vocab_name)

    #     ast_vocab = load_vocab(vocab_root=trained_vocab, name='ast.word.None.50000') #name=args.ast_vocab_name)
    # else:
    #     logger.info('Building vocabularies')

        # if args.no_replaced:

        # code vocab
        # code_vocab = init_vocab(vocab_save_dir=args.vocab_save_dir,
        #                         name=args.code_vocab_name,
        #                         method=args.code_tokenize_method,
        #                         vocab_size=args.code_vocab_size,
        #                         datasets=list(map(lambda code: code.split(), dataset.source_tokens)),
        #                         ignore_case=True,
        #                         save_root=args.vocab_root)

    # --------------------------------------------------
    tokenizer = TokenizerBPE()
#     sample="""
# function calculate_pi
# (max, seed) result(pi)
# implicit none
# integer, intent(in) :: max, seed
# real(8) :: pi
# real(8) :: area, x, y
# integer :: i
# external :: drand48
# integer :: pi_count

# pi_count = 0
# call seed48(seed)

# do i = 1, max
# x = drand48() * 2 - 1
# y = drand48() * 2 - 1
# if (x * x + y * y < 1) then
# pi_count = pi_count + 1
# end if
# area = 4.0 * real(pi_count) / real(i)
# end do

# pi = 4.0 * real(pi_count) / real(max)
# end function
# """
#     tokens = tokenizer.tokenize(sample, lang='fortran')
#     print(tokens)

    total_length = 0
    for sample in tqdm(dataset.source_tokens[:10**4]):
        # print(sample)
        tokens = tokenizer.tokenize(sample, lang='fortran')
        # print(tokens)
        total_length += len(tokens)
        # break

    print(total_length/10**4)

    total = 0
    total_length = 0
    for sample in tqdm(dataset.source_tokens[:10**4]):
        # print(sample)
        try:
            xsbt = cr.code2xsbt(sample)
        except:
            continue
        total += 1
        # print(xsbt)
        total_length += len(xsbt.split())
        # break


    print(total_length/total)



    tokenizer = Tokompiler('/home/1010/talkad/OMPify/CompCoder/data/asts/vocabs/tokenizer_vocab/vocab.txt')
    total_length = 0
    for sample in tqdm(dataset.replaced_tokens[:10**4]):
        # print(sample)
        # tokens = tokenizer.tokenize(sample, lang='fortran')
        tokens = sample.split()
        total_length += len(tokens)
        # print(tokens)
        # break

    print(total_length/10**4)  


    total_length = 0
    total = 0
    for sample in tqdm(dataset.source_tokens[:10**4]):
        # print(sample)
        try:
            ast = cr.code2ast(sample)
        except:
            continue

        total += 1
        # print(ast)
        total_length += len(ast.split())
        # break

    print(total, total_length/total)