import os
import json
from tqdm import tqdm
from concurrent.futures.thread import ThreadPoolExecutor
import logging

import preprocess


logging.basicConfig(filename='clean_c.log', format='%(asctime)s - %(levelname)s - %(name)s -   %(message)s',datefmt='%d/%m/%Y %H:%M:%S',level=logging.INFO)
logger = logging.getLogger(__name__)


def remove_dups(js_files):
    '''
    remove duplicates from HPCorpus, using the hashed value
    '''
    hpcorpus_dir = '/tier2/bgu/HPCorpus_preprocess'
    save_dir = '/tier2/bgu/HPCorpus_preprocess'
    langs = ['c']

    lang_dir = os.path.join(hpcorpus_dir, langs[0])
    hash_memory = []

    for js_file in js_files:
        js_dir = os.path.join(lang_dir, js_file)

        logger.info(f'starts {js_dir}')
        dataset = []
        

        with open(os.path.join(js_dir), 'r') as f:
            for line in f:

                js = json.loads(line.strip())
                hash = js['hash']
                
                if hash in hash_memory:
                    continue

                hash_memory.append(hash)
                dataset.append(js)   

        # write the dataset into json
        with open(os.path.join(save_dir, langs[0], f"{preprocess.get_filename(js_file)}.jsonl"), 'w') as data_f:
            for sample in dataset:
                data_f.write(json.dumps(sample) + '\n')

        logger.info(f'end {js_dir}')
                    


def extract_hash(dir, js_file):
    '''
    create csv files create the hash and its location
    '''
    dataset = []
    hpcorpus_dir = dir
    save_dir = hpcorpus_dir+'/hash'
    langs = ['c']

    lang_dir =  hpcorpus_dir #os.path.join(hpcorpus_dir, langs[0])

    js_dir = os.path.join(lang_dir, js_file)

    logger.info(f'starts {js_dir}')  

    with open(os.path.join(js_dir), 'r') as f:
        for idx, line in enumerate(f):
            try:
                js = json.loads(line.strip())
            except Exception:
                continue
            hash = js['hash']

            dataset.append(f'{idx},{hash}')   

    # write the dataset into json
    with open(os.path.join(save_dir, langs[0], f"{preprocess.get_filename(js_file)}.csv"), 'w') as data_f:
        for sample in dataset:
            data_f.write(sample + '\n')

    logger.info(f'end {js_dir}')


def unite_csv(dir):
    hpcorpus_dir = dir+'/hash'
    langs = ['c']
    lang_dir = os.path.join(hpcorpus_dir, langs[0])

    with open(os.path.join(lang_dir, 'total.csv'), 'w') as total_f:

        for csv_file in tqdm(os.listdir(lang_dir)):
            print(csv_file)
            if 'total' in csv_file:
                continue

            csv_dir = os.path.join(lang_dir, csv_file)

            with open(csv_dir, 'r') as f:
                for line in f:
                    total_f.write(f'{csv_file},{line}' + '\n')


def extract_uniq_samples(dir):
    hpcorpus_dir = dir
    lang = 'c'

    lang_dir = hpcorpus_dir # os.path.join(hpcorpus_dir, lang)

    with open(os.path.join(lang_dir, f'total_uniq_{lang}.csv'), 'r') as f:
        uniq_content = f.read().split('\n')
        # print(uniq_content)
    
    with open(os.path.join(lang_dir, 'total_uniq.jsonl'), 'a') as total_f:
        for js_file in tqdm(os.listdir(lang_dir)):
            print(js_file)

            if not js_file.startswith('batch_'):
                continue
            
            uniq_content_file = [content.split(',') for content in uniq_content if content.startswith(preprocess.get_filename(js_file))]

            # print(uniq_content_file)
            # if len(uniq_content_file):
            #     print(uniq_content_file)

            with open(os.path.join(lang_dir, js_file), 'r') as js_f:
                samples = js_f.read().split('\n')

            for content in uniq_content_file:
                # print(samples[int(content[1])])
                try:
                    total_f.write(samples[int(content[1])] + '\n')
                except:
                    print('failed', content[1])
                    



    



# hpcorpus_dir = '/home/1010/talkad/Downloads/OMP_Dataset/c/source'
# samples = os.listdir(hpcorpus_dir)
# for sample in tqdm(samples):
#     if not sample.startswith('batch'):
#         continue
#     print(sample)
#     extract_hash(hpcorpus_dir, sample)



# unite_csv(hpcorpus_dir)


# extract_uniq_samples(hpcorpus_dir)







dir = '/home/1010/talkad/Downloads/OMP_Dataset/c/replaced'

with open(os.path.join(dir, 'total.jsonl'), 'w') as f:
    for file in tqdm(os.listdir(dir)):

        if file.startswith('batch_'):
            # print(file)
            with open(os.path.join(dir, file), 'r') as f_batch:

                for idx, l in enumerate(f_batch):
                    if l.startswith('e'):
                        print(file)
                        print(idx)
                    f.write(l)


hash = []
with open(os.path.join(dir, 'total_uniq.jsonl'), 'w') as f, open(os.path.join(dir, 'total.jsonl'), 'r') as f_all:
    for idx, l in enumerate(f_all):
        # print(idx)
        js = json.loads(l.strip())

        if js['hash'] not in hash:
            f.write(l)
            hash.append(js['hash'])
