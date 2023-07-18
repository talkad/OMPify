import os
from tqdm import tqdm
import json

hpcorpus_dir = '/tier2/bgu/HPCorpus_preprocess'  # '/tier2/bgu/HPCorpus'
dirs  = ['Fortran', 'c', 'cpp']

dirs  = ['c']

for lang in dirs:
    total_funcs = 0

    dir_path = os.path.join(hpcorpus_dir, lang)

    for js_file in tqdm(os.listdir(dir_path)):
        js_path = os.path.join(dir_path, js_file)

        with open(js_path, 'r') as f:
            for line in f:
                total_funcs += 1

    print(f'lang {lang} num of funcs {total_funcs}')


# lang Fortran num of funcs 359272   |  282122    =  78.52%
# lang c num of funcs 87817591
# lang cpp num of funcs 68233984

