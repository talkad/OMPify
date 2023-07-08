#!/usr/bin/python 

import os
import json



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



if __name__ == '__main__':

    with open('/homes/talkad/OMPify/database_creator/visualization/hpcorpus_stats/statistics/ENV.json', 'r') as f:
        vars = json.loads(f.read())

    print('c', iterate_jsons('c', vars))


# samples without the 'content' entry

# fortran
# ('Fortran', (0, 103))

# c
# ('c', (0, 3187))

# cpp
# ('cpp', (0, 2566))
