import json
import pickle
import random
import os
from tqdm import tqdm
import random

def save(split_name, save_path, data):
    with open(os.path.join(save_path, split_name), 'wb') as f:
        pickle.dump(data, f)

db_paths = ['/home/talkad/Downloads/thesis/data_gathering_script/samples/sample_c.json']

# ['/home/talkad/Downloads/thesis/data_gathering_script/samples/sample_c.json',
#             '/home/talkad/Downloads/thesis/data_gathering_script/samples/sample_cpp.json']

dataset =[]
prefix = '../../../LIGHTBITS_SHARE/c_loops/'

for path in db_paths:
    with open(path, 'r') as f:
        data = json.load(f)

    l = list(data.items())
    random.shuffle(l)
    data = dict(l)

    keys = list(data.keys())
    train = keys[:int(len(keys)*0.9)]
    test = keys[int(len(keys)*0.9):]

    for idx in tqdm(train):
        sample_path = data[idx]
        sample_path_original = os.path.join('/home/talkad', 'LIGHTBITS_SHARE/c_loops', sample_path[len(prefix):])

        sample_path_updated = os.path.join('/home/talkad', 'LIGHTBITS_SHARE/c_replaced', sample_path[len(prefix):])

        with open(os.path.join(sample_path_updated, 'code.c'), 'r') as f:
            code = f.read()

        with open(os.path.join(sample_path_original, 'code.c'), 'r') as f:
            code_original = f.read()

        try:
            with open(os.path.join(sample_path_updated, 'pragma.pickle'), 'rb') as f:
                label = 1
        except FileNotFoundError:
            label = 0
        
        dataset.append((code, label))
        dataset.append((code_original, label))

    random.shuffle(dataset)
    save('train.pkl', '.', dataset)

    dataset = []
    for idx in tqdm(test):
        sample_path = data[idx]
        sample_path_updated = os.path.join('/home/talkad', 'LIGHTBITS_SHARE/c_loops', sample_path[len(prefix):])

        with open(os.path.join(sample_path_updated, 'code.c'), 'r') as f:
            code = f.read()

        try:
            with open(os.path.join(sample_path_updated, 'pragma.pickle'), 'rb') as f:
                label = 1
        except FileNotFoundError:
            label = 0
        
        dataset.append((code, label))
    save('test.pkl', '.', dataset)





# total_samples = len(dataset)
# random.shuffle(dataset)

# # save('train', '.', dataset[:int(total_samples*0.8)])
# # save('valid', '.', dataset[int(total_samples*0.8):int(total_samples*0.9)])
# # save('test', '.', dataset[int(total_samples*0.9):])

# save('train', '.', dataset[:int(total_samples*0.9)])
# save('test', '.', dataset[int(total_samples*0.9):])


# # print(dataset)


