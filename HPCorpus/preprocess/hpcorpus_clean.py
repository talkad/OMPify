import os
import json
from tqdm import tqdm
import preprocess
from concurrent.futures.thread import ThreadPoolExecutor
import logging

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
                    


def extract_hash(js_file):
    '''
    create csv files create the hash and its location
    '''
    dataset = []
    hpcorpus_dir = '/tier2/bgu/HPCorpus_preprocess'
    save_dir = '/tier2/bgu/hash'
    langs = ['Fortran']

    lang_dir = os.path.join(hpcorpus_dir, langs[0])

    js_dir = os.path.join(lang_dir, js_file)

    logger.info(f'starts {js_dir}')  

    with open(os.path.join(js_dir), 'r') as f:
        for idx, line in enumerate(f):

            js = json.loads(line.strip())
            hash = js['hash']

            dataset.append(f'{idx},{hash}')   

    # write the dataset into json
    with open(os.path.join(save_dir, langs[0], f"{preprocess.get_filename(js_file)}.csv"), 'w') as data_f:
        for sample in dataset:
            data_f.write(sample + '\n')

    logger.info(f'end {js_dir}')


def unite_csv():
    hpcorpus_dir = '/tier2/bgu/hash'
    langs = ['cpp']
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


def extract_uniq_samples():
    hpcorpus_dir = '/tier2/bgu/HPCorpus_preprocess'
    lang = 'cpp'

    lang_dir = os.path.join(hpcorpus_dir, lang)

    with open(os.path.join(lang_dir, f'total_uniq_{lang}.csv'), 'r') as f:
        uniq_content = f.read().split('\n')
        # print(uniq_content)
    
    with open(os.path.join(lang_dir, 'total_uniq.jsonl'), 'w') as total_f:
        for js_file in tqdm(os.listdir(lang_dir)):
            
            if 'total' in js_file:
                continue

            uniq_content_file = [content.split(',') for content in uniq_content if content.startswith(preprocess.get_filename(js_file))]
            # if len(uniq_content_file):
            #     print(uniq_content_file)

            with open(os.path.join(lang_dir, js_file), 'r') as js_f:
                samples = js_f.read().split('\n')

            for content in uniq_content_file:
                # print(samples[int(content[1])])
                total_f.write(samples[int(content[1])] + '\n')
                    



    


# hpcorpus_dir = '/tier2/bgu/HPCorpus_preprocess/cpp'
# samples = os.listdir(hpcorpus_dir)[:100]
# step_size = 5
# for i in range(0, len(samples), step_size):
#     elements_to_process = samples[i:i+step_size]

#     remove_dups(elements_to_process)



# hpcorpus_dir = '/tier2/bgu/HPCorpus_preprocess/Fortran'
# samples = os.listdir(hpcorpus_dir)
# for sample in samples:
#     extract_hash(sample)



# unite_csv()


extract_uniq_samples()