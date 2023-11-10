import json
from tqdm import tqdm

source_file = '/homes/talkad/Downloads/OMP_Dataset/mixed/replaced/total_c_cpp.jsonl'
target_cpu = '/homes/talkad/Downloads/OMP_Dataset/cpu/replaced/cpu_openmp.jsonl'
target_gpu = '/homes/talkad/Downloads/OMP_Dataset/gpu/replaced/gpu_openmp.jsonl'

with open(source_file, 'r') as src, open(target_cpu, 'w') as cpu, open(target_gpu, 'w') as gpu:
    for line in tqdm(src):
        js = json.loads(line.strip())

        pragma = js['pragma']

        if 'target' in pragma or ('teams' in pragma and 'distribute' in pragma):
            gpu.write(line)
        elif all([not clause in pragma for clause in ['target', 'teams', 'distribute']]):
            cpu.write(line)