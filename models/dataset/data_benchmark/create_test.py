import pickle
import json
from tqdm import tqdm
import re
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


db_paths = {'/home/talkad/Downloads/thesis/data_gathering_script/samples/sample_spec.json': 'test_spec.jsonl',
            '/home/talkad/Downloads/thesis/data_gathering_script/samples/sample_polybench.json': 'test_poly.jsonl',
            '/home/talkad/Downloads/thesis/data_gathering_script/samples/sample_nas.json': 'test_nas.jsonl'}

for path in db_paths:
    with open(db_paths[path], 'w') as dataset:
        
        with open(path, 'r') as f:
            data = json.load(f)

        for _, sample_path in tqdm(data.items()):
            path = os.path.join('/home/talkad', sample_path[9:])
            pragma = ''
            is_exist, is_private, is_reduction = 0, 0, 0

            with open(os.path.join(path, 'code.c'), 'r') as f:
                code = f.read()

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
            
            dataset.write(json.dumps({'code': code, 
                                        'pragma': pragma, 
                                        'exist': is_exist, 
                                        'private' :is_private, 
                                        'reduction': is_reduction, 
                                        'path': path}) + '\n')
