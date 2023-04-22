import os
import json
import pickle
import pandas as pd
import matplotlib.pyplot as plt



  
def scan_json_per_year(json_path):
    '''
    count total use of each version
    '''
    total_repos = 0
    total_days = 0
    
    with open(json_path, 'rb') as f:
        database = pickle.load(f)

    for repo in database:
        # print(repo['created_date'], repo['update_date'])
        days = repo['update_date'] - repo['created_date']

        if days is None:
            continue

        total_repos += 1
        total_days += days.days

    print(total_repos, total_days)
    return total_days / total_repos

print(scan_json_per_year('/home/talkad/Downloads/thesis/data_gathering_script/visualization/metadata.pickle'))
# 8983 3119833
# 347.30413002337747