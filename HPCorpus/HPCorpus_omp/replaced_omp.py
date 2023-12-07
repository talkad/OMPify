import sys
sys.path.append('..')
from parse_tools import parse
import json
import re
import convert_representation as cr

def parse_openmp_pragma(pragma):
    '''
    parse OpenMP pragma into meaningful representation

    Parameters:
        pragma: str - string indicating the pragma
    Result:
        A list containing tuples representing the clause and their arguments

    Example:
        input: '#pragma omp for private  (a,b,c) lastprivate(d) schedule(static:8)'
        output: [('pragma', ''), ('omp', ''), ('for', ''), ('private', 'a,b,c'), ('lastprivate', 'd'), ('schedule', 'static:8')]

    '''
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


def replace_pragma(pragma, mappings):
    result = ''
    try:
        clauses = parse_openmp_pragma(pragma)
        replaced_pragma = []

        for clause,vars in clauses:
            try:
                if not vars:
                    replaced_pragma.append((clause,vars))
                elif clause in ['reduction', 'map']:
                    func = vars[:vars.find(':')]
                    func_vars = vars[vars.find(':')+1:].split()

                    func_vars = ' '.join(sorted([mappings[var] for var in func_vars]))
                    replaced_pragma.append((clause,f'{func}:{func_vars}'))
                else:
                    func_vars = vars.split(',')

                    func_vars = ' '.join(sorted([mappings[var] for var in func_vars]))
                    replaced_pragma.append((clause,func_vars))
            except:
                pass

        result = '#'+' '.join(clause[0] if not clause[1] else f'{clause[0]}({clause[1]})' for clause in replaced_pragma)
    except Exception as e:
        print(e)
        pass

    return result
        

def is_omp_pragma(pragma):
    '''
    Returns true if the given pragma is omp
    '''
    line = pragma.lstrip().lower()
    clauses = [clause for clause, _ in parse_openmp_pragma(line)]

    return 'omp' in clauses

def replace_code(code):
    node = parse(code, lang='c')
    code, mappings = cr.generate_replaced(node)
    buf = []

    for line in code.split('\n'):
        if is_omp_pragma(line):
            buf.append(replace_pragma(line, mappings))
        else:
            buf.append(line)

    return '\n'.join(buf)



hpcorpus_dir = '/tier2/bgu/HPCorpus_omp.jsonl'
hpcorpus_omp_dir = '/tier2/bgu/HPCorpus_omp_replaced.jsonl'


with open(hpcorpus_dir, 'r') as f, open(hpcorpus_omp_dir, 'w') as out:
    for idx, line in enumerate(f):
        js = json.loads(line.strip())
        code = js['code']

        code = replace_code(code)
        out.write(json.dumps({'code': code}) + '\n')

        if idx % 10**4 == 0:
            print(f'iteration: {idx}')


