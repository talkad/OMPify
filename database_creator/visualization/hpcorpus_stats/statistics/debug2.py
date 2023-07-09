#!/usr/bin/python 

import os
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

       

if __name__ == '__main__':

    with open('/homes/talkad/OMPify/database_creator/visualization/hpcorpus_stats/statistics/ENV.json', 'r') as f:
        vars = json.loads(f.read())

    # files_per_repo('Fortran', vars)

    with open('/homes/talkad/OMPify/Fortran.json', 'r') as f:
        repos = json.loads(f.read())

        print(len(repos))

        print(sum(repos.values()))


# samples without the 'content' entry
# ('Fortran', (0, 103))
# 3683
# 138552

# ('c', (0, 3187))
# ('cpp', (0, 2566))
