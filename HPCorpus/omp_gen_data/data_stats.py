from tqdm import tqdm
import json
import os

lang = 'cpp'

rel_clauses = ['private', 'reduction', 'simd', 'total']

data_dir = '/home/talkad/LIGHTBITS_SHARE/CompCoder_datasets/OMP_Dataset'

counter = {clause:0 for clause in rel_clauses}

for file in [os.path.join(data_dir, lang, 'source', 'total.jsonl')]:
    
    with open(os.path.join(data_dir, file), 'r') as f:
        for line in tqdm(f):
            js = json.loads(line.strip())

            clauses = js['pragma']
            
            counter['total'] += 1

            for clause in rel_clauses[:-1]:
                if clause in clauses:
                    counter[clause] += 1

    print(counter)


# fortran: {'private': 1867, 'reduction': 180, 'simd': 16, 'total': 3857}
# c: {'private': 4410, 'reduction': 748, 'simd': 355, 'total': 11336}
# cpp: {'private': 2237, 'reduction': 1453, 'simd': 353, 'total': 19091}