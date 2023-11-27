import os
import json
import random

def split_jsonl(input_file, train_output_file, test_output_file, split_ratio=0.8):
    with open(input_file, 'r', encoding='utf-8') as file:
        lines = file.readlines()

    random.shuffle(lines)

    split_index = int(len(lines) * split_ratio)

    train_lines = lines[:split_index]
    test_lines = lines[split_index:]

    with open(train_output_file, 'w', encoding='utf-8') as train_file:
        train_file.writelines(train_lines)

    with open(test_output_file, 'w', encoding='utf-8') as test_file:
        test_file.writelines(test_lines)

if __name__ == "__main__":
    data_dir = '/home/talkad/LIGHTBITS_SHARE/OMP_Dataset/cpu/replaced'

    input_file_path = 'cpu_openmp_unique.jsonl'

    train_output_file_path = 'train.jsonl'
    test_output_file_path = 'test.jsonl'

    split_ratio = 0.9

    split_jsonl(os.path.join(data_dir, input_file_path), 
                os.path.join(data_dir, train_output_file_path),
                os.path.join(data_dir, test_output_file_path), 
                split_ratio)