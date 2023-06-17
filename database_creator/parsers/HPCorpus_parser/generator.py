import os 
import json
from convert_representation import generate_replaced, code2xsbt, code2dfg


def load_json(json_dir, json_file):
    '''
    Convert jsonl file to list of dicts
    '''
    dataset = []

    with open(os.path.join(json_dir, json_file), 'r') as f:
        for line in f:
            js = json.loads(line.strip())
            dataset.append(js)

    return dataset


def update_batch(batch, **kwargs):
    '''
    Update each sample according to the given kwargs
    '''
    updated_batch = []

    for sample in batch:
        code = sample['code']

        if 'replaced' in kwargs and kwargs['replaced'] == True:
            code = generate_replaced(code)
            sample['replaced'] = code
        if 'ast' in kwargs and kwargs['ast'] == True:
            sample['ast'] = code2xsbt(code)
        if 'dfg' in kwargs and kwargs['dfg'] == True:
            sample['dfg'] = code2dfg(code)

        updated_batch.append(sample)

    return updated_batch


def data_generator(json_dir, batch_size=32, **kwargs):
    for json_file in os.listdir(json_dir):
        dataset = load_json(json_dir, json_file)

        for i in range(0, len(dataset), batch_size):
            batch = dataset[i: i+batch_size]
            batch = update_batch(batch, kwargs)

            yield batch
