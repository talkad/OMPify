import json
from tqdm import tqdm

file_path = '/home/1010/talkad/OMPify/CompCoder/pretrain_c.jsonl'

with open(file_path, 'r') as f, open('hashes.txt', 'w') as output_f:
    for line in tqdm(f):
        js = json.loads(line.strip())

        output_f.write(js['replaced_hash'] + '\n')


# original lines - 20529149
# replaced uniq lines - 20523017
