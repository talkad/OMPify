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
                    

hpcorpus_dir = '/tier2/bgu/HPCorpus_preprocess/c'
samples = os.listdir(hpcorpus_dir)[300:]
step_size = 5
for i in range(0, len(samples), step_size):
    elements_to_process = samples[i:i+step_size]

    remove_dups(elements_to_process)


# with ThreadPoolExecutor(max_workers=50) as executor:
#     executor.map(remove_dups, samples)



