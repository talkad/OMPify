import os
from tqdm import tqdm
import json
from pycparser import c_generator
from parsers.cParser import CLoopParser


parser = CLoopParser('', '')
code_generator = c_generator.CGenerator()

samples = {}
memory = []
samples_count = 0
database_path = '/home/talkad/LIGHTBITS_SHARE/c_loops_detailed'

for repo in tqdm(os.listdir(database_path)):
    for dir_path, dir_names, file_names in os.walk(os.path.join(database_path, repo)):
        if not dir_names:

            loop_code = code_generator.visit(parser.load(dir_path).for_node)
            
            if loop_code in memory:
                continue

            memory.append(loop_code)
            samples[samples_count] = dir_path
            samples_count += 1


with open("sample_c_extended.json", "w") as f:
    json.dump(samples, f, sort_keys=True, indent=4, separators=(',', ': '))
