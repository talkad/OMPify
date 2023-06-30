import os
import json
from tqdm import tqdm
import re
import multiprocessing


corpus_dir = '/mnt/c/Users/tal74/Downloads/'
# dirs = ['c_220'] # 'c_0', 'c_40', 'c_70', 'c_100', 'c_140', 'c_180', 'c_220', 'c_260', 'c_300', 'c_340']
# dirs = ['Fortran/Fortran', 'cpp1', 'cpp2', 'cpp3', 'cpp4', 'cpp5', 'c_0', 'c_40', 'c_70', 'c_100', 'c_140', 'c_180', 'c_220', 'c_260', 'c_300', 'c_340']
dirs = ['c_220', 'c_260', 'c_300', 'c_340']


paradigms = {
    'CUDA': [r'^\W*#\W*include\W+<cuda.h>.*$'],
    'OpenCL': [r'^\W*#\W*include\W+<cl/cl.h>.*$'],
    'OpenACC': [r'^\W*#\W*include\W+<openacc.h>.*$'],
    'SYCL': [r'^\W*#\W*include\W+<cl/sycl.hpp>.*$'],
    'TBB': [r'^\W*#\W*include\W+<tbb/tbb.h>.*$'],
    'Cilk': [r'^\W*#\W*include\W+<cilk/cilk.h>.*$'],
    'OpenMP': [r'^\W*#\W*include\W+<omp.h>.*$', r'\W*use\W+omp_lib.*$'],
    'MPI': [r'^\W*#\W*include\W+<mpi.h>.*$', r'\W*use\W+mpi.*$']
}

def get_parallel_paradigms(code):
    matched_paradigms = []

    for paradigm, patterns in paradigms.items():
        for pattern in patterns:
            if re.search(pattern, code, re.MULTILINE):
                matched_paradigms.append(paradigm)
                break
    
    return matched_paradigms


def save_paradigms(dir):
    repos = {}
    dir_path = os.path.join(corpus_dir, dir)

    for file in tqdm(os.listdir(dir_path)):
        file_path = os.path.join(dir_path, file)

        with open(file_path, 'r') as f:
            for sample in f:
                
                try:
                    js = json.loads(sample.strip())
                except:
                    continue

                if 'content' not in js:
                    continue

                repo_name = js['repo_name']
                code = js['content']
                code = code.lower()

                paradigms = get_parallel_paradigms(code)
                if repo_name not in repos:
                    repos['repo_name'] = {'CUDA': False, 'OpenCL': False, 'OpenACC': False, 'SYCL': False, 
                                          'TBB': False, 'Cilk': False, 'OpenMP': False, 'MPI': False}
                    
                for paradigm in paradigms:
                    repos['repo_name'][paradigm] = True

    save_dir = dir.replace('/', '_')
    with open(f'{save_dir}.json', 'w') as f:
        f.write(json.dumps(repos))


pool = multiprocessing.Pool(processes=4)
results = pool.map(save_paradigms, dirs)

pool.close()
pool.join()


# # repos = set()

# # files = ['repos_c1', 'repos_c2', 'repos_c3', 'repos_c4', 'repos_c5', 'repos_c6', 'repos_c7', 'repos_c8', 'repos_c9', 'repos_c10', 'repos', 'repos1', 'repos2', 'repos3', 'repos4', 'repos5']
# # corpus_dir = '/home/talkad/OpenMPdb/database_creator/visualization/hpcorpus_stats/repos'



# # for file in tqdm(files):
# #     dir_path = os.path.join(corpus_dir, f'{file}.jsonl')

# #     with open(dir_path, 'r') as f:
# #         for sample in f:
# #             js = json.loads(sample.strip())

# #             str = js['username']+'/'+js['repo_name']
# #             repos.add(str)

# # with open('hpcorpus_repos.jsonl', 'w') as f:
# #     for repo in repos:
# #         r = repo.split('/')
# #         f.write(json.dumps({'username': r[0], 'repo_name': r[1]}) + '\n')
