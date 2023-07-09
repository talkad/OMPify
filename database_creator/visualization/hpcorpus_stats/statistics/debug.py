#!/usr/bin/python 

import os
import json
from tqdm import tqdm
import requests


def get_repository_info(username, repo_name):
    api_url = "https://api.github.com/repos/{}/{}".format(username,repo_name)
    response = requests.get(api_url)
    
    if response.status_code == 200:
        data = response.json()
        created_at = data["created_at"]
        updated_at = data["updated_at"]
        
        return created_at, updated_at
    else:
        return None, None
    



def time_per_repo(dir, vars):
    repositories = {}
    dir_path = os.path.join(vars['HPCorpus_path'], dir)

    for json_file in tqdm(os.listdir(dir_path)):
        with open(os.path.join(dir_path, json_file), 'r') as f:
            for line in f:
                js = json.loads(line.strip())

                if 'content' not in js:
                    continue
                
                repo_name = js['repo_name']
                repo = js['repo_name'].split('/')

                if repo_name not in repositories:
                    created_at, updated_at = get_repository_info(repo[0], repo[1])
                    repositories[repo_name] = {'creation_time': created_at, 'update_time': updated_at}

    with open('{}_.json'.format(dir), 'w') as f:
        f.write(json.dumps(repositories))

                


def iterate_jsons(dir, vars):
    counter1, counter2 = 0, 0
    dir_path = os.path.join(vars['HPCorpus_path'], dir)

    for json_file in os.listdir(dir_path):
        with open(os.path.join(dir_path, json_file), 'r') as f:
            for line in f:
                try:
                    js = json.loads(line.strip())
                except:
                    counter1 += 1
                    continue

                if 'content' not in js:
                    counter2 += 1
                    continue
                
    return counter1,  counter2


def unite_paradigms():
    '''
    Join the different JSON files by repo name
    '''
    repos = {}

    for lang in ['Fortran', 'c', 'cpp']:
        with open(f'/homes/talkad/OMPify/{lang}_paradigms.json', 'r') as f:
            paradigms = json.loads(f.read())

            for repo, pars in paradigms.items():
                if repo not in repos:
                    repos[repo] = {'CUDA': False, 'OpenCL': False, 'OpenACC': False, 'SYCL': False, 
                                          'TBB': False, 'Cilk': False, 'OpenMP': False, 'MPI': False}

                for par, val in pars.items():
                    repos[repo][par] |= val

    with open('total_paradigms_mpi.json', 'w') as f:
        f.write(json.dumps(repos))



if __name__ == '__main__':

    unite_paradigms()
    # with open('/homes/talkad/OMPify/database_creator/visualization/hpcorpus_stats/statistics/ENV.json', 'r') as f:
    #     vars = json.loads(f.read())

    # time_per_repo('c', vars)


    with open('/homes/talkad/OMPify/total_paradigms_mpi.json', 'r') as f:
        repos = json.loads(f.read())

        print(len(repos))

        # print(sum(repos.values()))

    



# samples without the 'content' entry
# ('Fortran', (0, 103))
# 3683
# 138552

# ('c', (0, 3187))


# ('cpp', (0, 2566))
# 150481
# 4735196
