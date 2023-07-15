#!/usr/bin/python 

import os
import re
import json
from tqdm import tqdm


def files_per_repo(dir, vars):
    repositories = {}
    dir_path = os.path.join(vars['HPCorpus_path'], dir)

    for json_file in tqdm(os.listdir(dir_path)):
        with open(os.path.join(dir_path, json_file), 'r') as f:
            for line in f:
                js = json.loads(line.strip())

                if 'content' not in js:
                    continue
                
                repo_name = js['repo_name']

                print(repo_name)

    with open('debug.json', 'w') as f:
        f.write(json.dumps(repositories))

       

def clause__per_version(ver, lang='c'):
    clauses_per_version = {
        'ver2.5': [(['parallel'],[]), (['sections'],[]), (['section'],[]), (['single'],[]), (['workshare'],[]), (['for'],[]), (['do'],[]), (['parallel', 'for'],[]), (['parallel', 'do'],[]), (['parallel', 'sections'],[]), (['parallel', 'workshare'],[]), (['master'],[]), (['critical'],[]), (['barrier'],[]), (['atomic'],[]), (['flush'],[]), (['ordered'],[]), (['threadprivate'],[]), (['default'],[]), (['shared'],[]), (['private'],[]), (['firstprivate'],[]), (['lastprivate'],[]), (['reduction'],[]), (['copyin'],[]), (['copyprivate'],[]), (['num_threads'],[]), (['schedule'],[]), (['schedule'],['static']), (['schedule'],['dynamic']), (['schedule'],['guided']), (['schedule'],['runtime']), (['nowait'],[])],
        'ver3.0': [(['task'],[]), (['taskwait'],[]), (['schedule'],['auto']), (['atomic', 'read'],[]), (['atomic', 'write'],[]), (['atomic', 'update'],[]), (['atomic', 'capture'],[])],
        'ver3.1': [(['taskyield'],[]), (['collapse'],[])],
        'ver4.0': [(['simd'],[]), (['simdlen'],[]), (['declare', 'simd'],[]), (['target'],[]), (['target', 'data'],[]), (['target', 'update'],[]), (['declare', 'target'],[]), (['teams'],[]), (['distribute'],[]), (['distribute', 'simd'],[]), (['target', 'teams'],[]), (['teams', 'distribute'],[]), (['teams', 'distribute', 'simd'],[]), (['target', 'teams', 'distribute'],[]), (['target', 'teams', 'distribute', 'simd'],[]), (['taskgroup'],[]), (['cancel'],[]), (['cancellation'],[]), (['map'],[]), (['declare', 'reduction'],[]), (['proc_bind'],[])],
        'ver4.5': [(['taskloop'],[]), (['taskloop', 'simd'],[]), (['target', 'enter', 'data'],[]), (['target', 'exit', 'data'],[]), (['depend'],[]), (['linear'],[]), (['defaultmap'],[]), (['if'],[])],
        'ver5.0': [(['declare', 'variant'],[]), (['requires'],[]), (['distribute', 'loop'],[]), (['loop'],[]), (['scan'],[]), (['allocate'],[]), (['taskloop', 'simd'],[]), (['parallel', 'loop'],[]), (['parallel', 'master'],[]), (['master', 'taskloop'],[]), (['master', 'taskloop', 'simd'],[]), (['parallel', 'master', 'taskloop'],[]), (['parallel', 'master', 'taskloop', 'simd'],[]), (['teams', 'loop'],[]), (['parallel', 'target'],[]), (['target', 'parallel', 'loop'],[]), (['target', 'simd'],[]), (['target', 'teams', 'loop'],[]), (['deobj'],[]), (['task_reduction'],[]), (['in_reduction'],[]), (['copyprivate'],[]), (['declare', 'mapper'],[])],
        'ver5.1': [(['dispatch'],[]), (['assume'],[]), (['nothing'],[]), (['error'],[]), (['masked'],[]), (['scope'],[]), (['tile'],[]), (['unroll'],[]), (['interop'],[]), (['parallel', 'masked'],[]), (['masked', 'taskloop'],[]), (['masked', 'taskloop', 'simd'],[]), (['parallel', 'masked', 'taskloop', 'simd'],[])],
        'ver5.2': [(['destroy'],[]), (['is_device_ptr'],[]), (['use_device_ptr'],[]), (['has_device_ptr'],[]), (['use_device_addr'],[]), (['initializer'],[]), (['inclusive'],[]), (['exclusive'],[]), (['enter'],[]), (['link'],[]), (['to'],[]), (['from'],[]), (['uniform'],[]), (['aligned'],[]), (['align'],[]), (['allocator'],[]), (['allocators'],[]), (['use_allocators'],[]), (['when'],[]), (['otherwise'],[]), (['metadirective'],[]), (['begin', 'metadirective'],[]), (['match'],[]), (['adjust_args'],[]), (['append_args'],[]), (['begin', 'declare', 'variant'],[]), (['novariants'],[]), (['nocontext'],[]), (['begin', 'declare', 'target'],[]), (['indirect'],[]), (['at'],[]), (['assumes'],[]), (['begin', 'assumes'],[]), (['severity'],[]), (['message'],[]), (['sizes'],[]), (['full'],[]), (['partial'],[]), (['nontemporal'],[]), (['safelen'],[]), (['filter'],[]), (['dist_schedule'],[]), (['bind'],[]), (['untied'],[]), (['mergeable'],[]), (['final'],[]), (['priority'],[]), (['grainsize'],[]), (['num_tasks'],[]), (['device_type'],[]), (['device'],[]), (['thread_limit'],[]), (['init'],[]), (['use'],[]), (['hint'],[]), (['nogroup'],[]), (['doacross'],[]), (['nogroup'],[])]
    }       
    clauses = [clause[0][0] for clause in clauses_per_version[ver]]
    result = {}

    with open(f'analyzed_data/parsed_pragmas/{lang}_parsing.log', 'r') as f:
        for l in f:   
            pattern = r'\(u\'(.*?)\''
            matches = re.findall(pattern, l)

            for clause in clauses:
                if clause in matches:
                    if clause not in result:
                        result[clause] = 0
                    result[clause] += 1
    
    return result



if __name__ == '__main__':

    # with open('/homes/talkad/OMPify/database_creator/visualization/hpcorpus_stats/statistics/ENV.json', 'r') as f:
    #     vars = json.loads(f.read())

    # files_per_repo('c', vars)

    # with open('/homes/talkad/OMPify/Fortran.json', 'r') as f:
    #     repos = json.loads(f.read())

    #     print(len(repos))

    #     print(sum(repos.values()))


# samples without the 'content' entry
# ('Fortran', (0, 103))
# 3683
# 138552

# ('c', (0, 3187))
# ('cpp', (0, 2566))

    print(clause__per_version('ver2.5'))