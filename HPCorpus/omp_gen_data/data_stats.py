from tqdm import tqdm
import json
import os

lang = 'c'
rel_clauses = ['private', 'reduction', 'target', 'total']

data_dir = '/home/talkad/LIGHTBITS_SHARE/OMP_Dataset'

counter = {clause:0 for clause in rel_clauses}

for file in [os.path.join(data_dir, lang, 'source', 'total_c_unique.jsonl')]:
    
    with open(os.path.join(data_dir, file), 'r') as f:
        for line in tqdm(f):
            js = json.loads(line.strip())

            pragma = js['pragma']

            # if all([not clause in pragma for clause in ['target', 'teams', 'distribute']]):           
            counter['total'] += 1

            for clause in rel_clauses[:-1]:
                if clause in pragma:
                    counter[clause] += 1

    print(counter)


# fortran: {'private': 1867, 'reduction': 180, 'simd': 16, 'total': 3857}
# c: {'private': 4410, 'reduction': 748, 'simd': 355, 'total': 11336}
# cpp: {'private': 2237, 'reduction': 1453, 'simd': 353, 'total': 19091}



# HPCorpus new - just cpu
# c {'private': 3526, 'reduction': 713, 'target': 100, 'total': 8764}
# cpp {'private': 2122, 'reduction': 1424, 'target': 230, 'total': 18151}