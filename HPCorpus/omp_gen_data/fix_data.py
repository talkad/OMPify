import json
import re
import os

def parse_openmp_pragma(pragma):
    pragma = f'{pragma} '
    pattern = r'(\w+(\s*\(.*?\)|\s))'
    matches = re.findall(pattern, pragma)
    clauses = []
    
    for match in matches:
        clause = match[0].strip()

        if '(' in clause:
            clause = clause[:clause.find('(')].strip()
            args = match[1].strip()[1:-1]
            clauses.append((clause, args))
        else:
            clauses.append((clause, ''))
    
    return clauses


    
def get_pragma( pragma):
    line = pragma.lstrip().lower()
    clauses = parse_openmp_pragma(line)

    rel_pragma = sorted(clauses, key=lambda x: x[0])                                                       # sort the pragma
    rel_pragma = list(map(lambda clause: f"{clause[0]}({' '.join(sorted(re.split('[ ,]', clause[1])))})" if clause[1] else clause[0] , rel_pragma))                      # convert to string
    
    return rel_pragma



dir = '/home/1010/talkad/Downloads/OMP_Dataset/fortran'
with open(os.path.join(dir, 'total_uniq.jsonl'), 'r') as f, open(os.path.join(dir, 'total_uniq_new.jsonl'), 'w') as f_new:
    dataset = []

    for line in f:
        js = json.loads(line.strip())
        js['pragma'] = ' '.join(js['pragma'] )#get_pragma(js['pragma'])

        dataset.append(js)

    for sample in dataset:
        f_new.write(json.dumps(sample) + '\n')
