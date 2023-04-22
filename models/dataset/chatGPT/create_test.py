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


db_path = '/home/talkad/LIGHTBITS_SHARE/chatGPT_DB/DB/database'

with open(f'test.jsonl', 'w') as dataset:
    for folder in tqdm(os.listdir(db_path)):
        is_exist, is_reduction, is_private = 0, 0, 0

        try:
            with open(os.path.join(db_path, folder, 'pragma.c')) as f:
                pragma = f.read()

                if reduction_reg.search(pragma):
                    is_reduction = 1

                if private_reg.search(pragma):
                    is_private = 1
                is_exist = 1
        except:
            pass

        with open(os.path.join(db_path, folder, 'code.c'), 'r') as f:
            code = f.read()
            code = code_clean(code)

        dataset.write(json.dumps({'code': code, 
                                        'pragma': pragma, 
                                        'exist': is_exist, 
                                        'private' :is_private, 
                                        'reduction': is_reduction, 
                                        'path': ''}) + '\n')