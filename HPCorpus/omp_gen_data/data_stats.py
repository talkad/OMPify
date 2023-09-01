from tqdm import tqdm
import json
import os

lang = 'fortran'

rel_constrcuts = ['do' if lang == 'fortran' else 'for']
rel_clauses = ['private', 'reduction', 'simd', '_']

data_dir = '/home/1010/talkad/Downloads/OMP_Dataset/fortran/source'

counter = {construct:{clause:0 for clause in rel_clauses} for construct in rel_constrcuts}

for file in [os.path.join(data_dir, 'total_uniq.jsonl')]:
    
    with open(os.path.join(data_dir, file), 'r') as f:
        for line in tqdm(f):
            js = json.loads(line.strip())

            clauses = js['pragma'].split()
            clauses_key = [clause[:clause.find('(')] if '(' in clause else clause for clause in clauses]

            for k,v in counter.items():
                if k in clauses_key:
                    counter[k]['_'] += 1

                for vv in v:
                    if k in clauses_key and vv in clauses_key:
                        counter[k][vv] += 1

    print(counter)


# fortran: {'do': {'private': 422, 'reduction': 51, 'simd': 5, '_': 1062}}
# c: {'for': {'private': 1035, 'reduction': 216, 'simd': 122, '_': 3351}}
# cpp: {'for': {'private': 509, 'reduction': 431, 'simd': 171, '_': 5220}}