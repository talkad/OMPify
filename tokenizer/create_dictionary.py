from tokenizer import Tokompiler
import os
import json
from tqdm import tqdm



def get_code(sample):
    
    code = sample['code'].split('\n')

    for idx, line in enumerate(code):
        if len(line) == 0 or line.endswith(';'):
            continue

        return '\n'.join(code[idx:])
    
    return ''
     

def count_tokens(lang):
    result = {}
    tokenizer = Tokompiler()

    dir = os.path.join('/tier2/bgu/HPCorpus', lang)

    for js_file in tqdm(os.listdir(dir)[:10]):
        js_path = os.path.join(dir, js_file)

        with open(js_path, 'r') as f:
            
            for sample in tqdm(f):
                js = json.loads(sample.strip())
                code = get_code(js)

                 
                tokens = tokenizer.tokenize(code, lang='fortran')

                for token in tokens:
                    if token not in result:
                        result[token] = 0

                    result[token] += 1

    with open("fortran_vocab.json", "w") as f:
        sorted_result = {k: v for k, v in sorted(result.items(), key=lambda item: item[1])}
        json.dump(sorted_result, f, indent=4, separators=(',', ': '))


def create_vocab():
    
    with open('vocabs/token_occurences/total.json', 'r') as f, open('tokompiler_vocab.json', 'w') as vocab:
        js = json.loads(f.read())

        js_vocab = {key:idx for idx, key in enumerate(js)}
        json.dump(js_vocab, vocab, indent=4, separators=(',', ': '))


# Amount of each token     
# count_tokens('Fortran')

# c cpp 146744 (one file each)
# fortran (10 files)


create_vocab()