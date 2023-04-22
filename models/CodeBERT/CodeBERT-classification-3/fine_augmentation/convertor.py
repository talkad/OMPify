import json
import pickle
import random
import os
from tqdm import tqdm

def save(split_name, save_path, data):
    with open(os.path.join(save_path, split_name), 'w') as f:
        for path in data:
            f.write(path+'\n')

db_paths = ['/home/talkad/Downloads/thesis/data_gathering_script/samples/sample_c.json']


dataset =[]

for path in db_paths:
    with open(path, 'r') as f:
        data = json.load(f)

    for _, sample_path in tqdm(data.items()):
        path = os.path.join('/home/talkad', sample_path[9:])

        dataset.append(path)


total_samples = len(dataset)
random.shuffle(dataset)

save('train.jsonl', '.', dataset[:int(total_samples*0.9)])
save('test.jsonl', '.', dataset[int(total_samples*0.9):])
save('valid.jsonl', '.', dataset[int(total_samples*0.9):])



# print(dataset)


