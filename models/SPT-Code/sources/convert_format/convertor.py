import json
import pickle
import random
import os
from tqdm import tqdm

def save(split_name, save_path, data):
    with open(os.path.join(save_path, split_name), 'wb') as f:
        pickle.dump(data, f)

db_paths = ['/home/talkad/Downloads/thesis/data_gathering_script/samples/sample_c.json']

# ['/home/talkad/Downloads/thesis/data_gathering_script/samples/sample_c.json',
#             '/home/talkad/Downloads/thesis/data_gathering_script/samples/sample_cpp.json']

dataset =[]

for path in db_paths:
    with open(path, 'r') as f:
        data = json.load(f)

    for _, sample_path in tqdm(data.items()):
        sample_path_updated = os.path.join('/home/talkad', sample_path[9:])

        with open(os.path.join(sample_path_updated, 'code.c'), 'r') as f:
            code = f.read()

        try:
            with open(os.path.join(sample_path_updated, 'pragma.pickle'), 'rb') as f:
                label = 1
        except FileNotFoundError:
            label = 0
        
        dataset.append((code, label))


total_samples = len(dataset)
random.shuffle(dataset)

# save('train', '.', dataset[:int(total_samples*0.8)])
# save('valid', '.', dataset[int(total_samples*0.8):int(total_samples*0.9)])
# save('test', '.', dataset[int(total_samples*0.9):])

save('train.pkl', '.', dataset[:int(total_samples*0.9)])
save('test.pkl', '.', dataset[int(total_samples*0.9):])
save('valid.pkl', '.', dataset[int(total_samples*0.9):])


# print(dataset)


