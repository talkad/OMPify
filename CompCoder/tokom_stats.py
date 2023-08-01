
import logging

import enums
from data.dataset import init_dataset
from data.vocab import  init_vocab, load_vocab
import sys
from tqdm import tqdm

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
    trained_vocab = None
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

        # if args.no_replaced:

        # code vocab
        code_vocab = init_vocab(vocab_save_dir=args.vocab_save_dir,
                                name=args.code_vocab_name,
                                method=args.code_tokenize_method,
                                vocab_size=args.code_vocab_size,
                                datasets=list(map(lambda code: code.split(), dataset.source_tokens)),
                                ignore_case=True,
                                save_root=args.vocab_root)
        # print(code_vocab.tokenizer.get_vocab()['[UNK]'])
        # print(code_vocab.tokenizer.get_vocab()['[PAD]'])
        # print(code_vocab.tokenizer.get_vocab()['[SEP]'])
    # else:
    #     # replaced code vocab
        # code_vocab = init_vocab(vocab_save_dir=args.vocab_save_dir,
        #                         name=args.replaced_code_vocab_name,
        #                         method='comp',
        #                         datasets=[],
        #                         ignore_case=True,
        #                         save_root=args.vocab_root)
            
    # --------------------------------------------------

    total_length = 0
    token_length = 0
    unk_counter = 0 
    for sample in tqdm(dataset.source_tokens[:10**5]):
        total_length+= len(sample)
        encoded = code_vocab.encode_batch([sample])[0][0]
        token_length+= len(encoded)
        unk_counter += code_vocab.encode_batch([sample])[0][0].count(4)


    print(total_length/10**5)
    print(token_length/10**5)
    print(unk_counter/(token_length))

    # tokom
    # 4700.80695
    # 630.46137
    # 0.020634920634920634

    # source
    # 1679.91665
    # 512.74274
    # 7.411123948824707e-07

    # source

    # print(len(dataset.source_tokens), sys.getsizeof(dataset.source_tokens))
    # print(len(dataset.replaced_tokens), sys.getsizeof(dataset.replaced_tokens))


  