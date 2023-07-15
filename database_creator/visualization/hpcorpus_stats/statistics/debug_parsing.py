from csv import DictReader
from datetime import datetime
import json
import re
from tqdm import tqdm


def get_repo_metadata(metadata_filepath):
    '''
    Load metadata - fork, creation and last update time of each repo
    '''
    metadata = {}
    prefix = 'https://github.com/'
    suffix = '.git'

    with open(metadata_filepath, 'r') as f:
        dict_reader = DictReader(f)

        for line in list(dict_reader):
            creation_time = datetime.strptime(line['creation_time'], '%Y-%m-%dT%H:%M:%SZ')
            update_time = datetime.strptime(line['update_time'], '%Y-%m-%dT%H:%M:%SZ')
            fork = True if line['fork'] == 'True' else False

            if fork:
                continue

            metadata[line['URL'][len(prefix): -len(suffix)]] = {'creation_time': creation_time, 'update_time': update_time, 'fork': fork}

    return metadata    


def extract_parsed_pragmas(file, year):
    meta = get_repo_metadata('analyzed_data/hpcorpus.timestamps.csv')
    pattern = r'\(u\'(.*?)\''    
    result = {}
    curr_repo = ''

    with open(file, 'r') as f:
        for l in tqdm(f):
            if l.startswith('repo :'):
                curr_repo = l[len('repo :'):-1]
            else:
                if curr_repo not in meta:
                    continue
                if meta[curr_repo]['update_time'].year == year:
                    matches = re.findall(pattern, l)

                    if curr_repo not in result:
                        result[curr_repo] = []

                    result[curr_repo].append(matches)

    with open('c_debug_2022.log', 'w') as f:
        f.write(json.dumps(result))



def analyze(year):
    with open(f'c_debug_{year}.log', 'r') as f:
        data = json.loads(f.read())

    # num repos
    print('num repos', len(data))

    # num pragmas
    total = 0
    for val in data.values():
        total += len(val)
    print('num pragmas', total)

    # num pragmas

    max = 0
    k = ''
    for key, val in data.items():
        if max < len(val):
            max = len(val)
            k = key
    print('outlier repo', k, max)

    # count 2.5 clauses

    pass

# extract_parsed_pragmas('/homes/talkad/OMPify/database_creator/visualization/hpcorpus_stats/statistics/c_parsing.log', 2022)
analyze(2023)