import json


def count_pragma_omp(code):
    counter = 0

    for line in code.split('\n'):
        line_clean = line.strip().lower()

        if line_clean.startswith('#pragma') and ' omp' in line_clean and ' for' in line_clean:
            counter+=1

    return counter


def connect_lines(code):
    '''
    concatenate c lines of code ends with '\'
    '''
    code_buf = []

    for line in code.split('\n'):
        if code_buf and code_buf[-1].rstrip().endswith('\\'):
            code_buf[-1] = code_buf[-1].rstrip()[:-1] + ' ' + line
        else:
            code_buf.append(line)

    code_buf = list(map(lambda line: line.replace('\t', ' ') if line.strip().startswith('#pragma') else line, code_buf))
    return '\n'.join(code_buf)
        

def remove_comp_dir(code):
    code_buf = []

    for line in code.split('\n'):
        if any([line.startswith(comp) for comp in ['#if', '#ifdef', '#ifndef', '#elif', '#else', '#endif']]):
            continue

        code_buf.append(f'{line} ')

    return '\n'.join(code_buf)


hpcorpus_dir = '/tier2/bgu/HPCorpus_final/fine_tune/c/dataset_c.jsonl'
hpcorpus_omp_dir = '/tier2/bgu/HPCorpus_omp.jsonl'

samples = 0

with open(hpcorpus_dir, 'r') as f, open(hpcorpus_omp_dir, 'w') as out:
    for idx, line in enumerate(f):
        js = json.loads(line.strip())
        code = js['code']

        if count_pragma_omp(code) > 0:
            code = remove_comp_dir(connect_lines(code))
            out.write(json.dumps({'code': code}) + '\n')
            samples += 1

        if idx % 10**4 == 0:
            print(f'iteration: {idx}, samples = {samples}')


