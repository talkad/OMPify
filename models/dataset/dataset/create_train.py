import pickle
import json
from tqdm import tqdm
import re
import random
import os

reduction_reg = re.compile(r'reduction\s*\(')
private_reg = re.compile(r'private\s*\(')


def code_clean(code):
    
    updated_code = ''

    for idx, line in enumerate(code.split('\n')):
        l = line.lstrip().lower()
        if l.startswith('#pragma') and 'omp' in l:
            continue

        updated_code += line + '\n'

    return updated_code

db_paths = {'/home/talkad/Downloads/thesis/data_gathering_script/database_creator/samples/sample_c.json': 'c', 
            '/home/talkad/Downloads/thesis/data_gathering_script/database_creator/samples/sample_cpp.json': 'cpp'}

test_json_paths = ['/home/talkad/Downloads/thesis/data_gathering_script/models/dataset/data_benchmark/test_nas.jsonl',
                    '/home/talkad/Downloads/thesis/data_gathering_script/models/dataset/data_benchmark/test_poly.jsonl',
                    '/home/talkad/Downloads/thesis/data_gathering_script/models/dataset/data_benchmark/test_spec.jsonl']

# load test
memory = []

for test_json_path in tqdm(test_json_paths):
    with open(test_json_path, 'r') as test:
        test_samples = [json.loads(line.strip())['code'] for line in test]
        print(f'size {len(test_samples)}')
        memory += test_samples


dataset = []

for db_path in db_paths:
    with open(db_path, 'r') as f:
        data = json.load(f)

    for _, sample_path in tqdm(data.items()):
        path = os.path.join('/home/talkad', sample_path[9:])
        pragma = ''
        is_exist, is_private, is_reduction = 0, 0, 0

        ### NO LEAKS ###
        test_sample = lambda path: 'spec' in path or 'polybench' in path or 'nas' in path
        if any([test_sample(sub_path) for sub_path in path.lower().split('/')]):
            continue

        with open(os.path.join(path, f'code.{db_paths[db_path]}'), 'r') as f:
            code = f.read()
            code = code_clean(code)
            
            ### NO LEAKS ###
            if code in memory:
                continue

        try:
            with open(os.path.join(path, 'pragma.pickle'), 'rb') as f:
                pragma = pickle.load(f).string
                is_exist = 1

                if reduction_reg.search(pragma):
                    is_reduction = 1

                if private_reg.search(pragma):
                    is_private = 1
        except FileNotFoundError:
            pass

        dataset.append({'code': code, 
                                    'pragma': pragma, 
                                    'exist': is_exist, 
                                    'private' :is_private, 
                                    'reduction': is_reduction, 
                                    'path': path})

total_samples = len(dataset)
random.shuffle(dataset)

with open(f'train.jsonl', 'w') as train:
    for sample in dataset[:int(total_samples*0.8)]:
        train.write(json.dumps(sample) + '\n')

with open(f'valid.jsonl', 'w') as train:
    for sample in dataset[int(total_samples*0.8):int(total_samples*0.9)]:
        train.write(json.dumps(sample) + '\n')

with open(f'test.jsonl', 'w') as train:
    for sample in dataset[int(total_samples*0.9):]:
        train.write(json.dumps(sample) + '\n')
