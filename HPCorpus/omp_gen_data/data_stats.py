from tqdm import tqdm
import json
import os

lang = 'fortran'

rel_constrcuts = ['do' if lang == 'fortran' else 'for', 'sections', 'task', 'target', 'teams']
rel_clauses = ['shared', 'private', 'firstprivate', 'lastprivate', 'reduction', 'map', 'simd', 'end', '_']

data_dir = '/home/1010/talkad/Downloads/OMP_Dataset/fortran'

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


# pragma = 'do private(a,b) firstprivate(d) reduce'
# clauses = pragma.split()
# clauses_key = [clause[:clause.find('(')] if '(' in clause else clause for clause in clauses]
# print(clauses_key)

# {'do': {'shared': 76, 'private': 415, 'firstprivate': 6, 'lastprivate': 5, 'reduction': 55, 'map': 1, 'simd': 5, 'end': 0, '_': 879}, 
# 'sections': {'shared': 0, 'private': 1, 'firstprivate': 4, 'lastprivate': 4, 'reduction': 1, 'map': 0, 'simd': 0, 'end': 0, '_': 12}, 
# 'task': {'shared': 12, 'private': 6, 'firstprivate': 9, 'lastprivate': 0, 'reduction': 1, 'map': 0, 'simd': 0, 'end': 0, '_': 47}, 
# 'target': {'shared': 0, 'private': 0, 'firstprivate': 0, 'lastprivate': 0, 'reduction': 1, 'map': 41, 'simd': 0, 'end': 0, '_': 64},
#  'teams': {'shared': 0, 'private': 0, 'firstprivate': 0, 'lastprivate': 0, 'reduction': 1, 'map': 1, 'simd': 0, 'end': 0, '_': 6}}

