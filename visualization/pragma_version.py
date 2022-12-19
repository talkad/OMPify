import os
import json
import pickle
import pandas as pd
import matplotlib.pyplot as plt


def get_omp_version(line):
    '''
        For a given pragma, determine the version of OpenMP it is related
    '''

    if line.startswith('#pragma'):
        if any([(clause in line) for clause in ['target_device', 'masked', 'scope', 'interop', 'present', 'dispatch']]):
            return 5
        elif any([(clause in line) for clause in ['inscan', 'scan', 'task_reduction', 'requires', 'defaultmap', 'order(concurrent)']]):
            return 4.5
        elif any([(clause in line) for clause in ['linear',  'simdlen', 'taskloop', 'target', 'source', 'sink']]):
            return 4
        elif any((clause in line) for clause in ['proc_bind', 'simd', 'distribute', 'teams', 'cancel', 'depend', 'taskgroup', 'capture', 'task', 'taskyield', 'taskwait', 'schedule(auto)', 'schedule(guided)']):
            return 3
    else:
        if any([line.startswith(func) for func in ['omp_num_teams', 'omp_set_num_teams', 'omp_set_teams_thread_limit', 'omp_get_teams_thread_limit', 'omp_teams_thread_limit', 'omp_get_max_teams', 'omp_target_is_accessible', 'omp_calloc', 'omp_aligned_calloc', 'omp_aligned_alloc', 'omp_realloc']]):
            return 5
        elif any([line.startswith(func) for func in ['omp_target_offload', 'omp_set_nested', 'omp_get_nested',  'omp_set_affinity_format' , 'omp_get_supported_active_levels',  'omp_get_affinity_format', 'omp_set_affinity', 'omp_capture_affinity', 'omp_display_affinity' , 'omp_affinity_format' , 'omp_pause_resource', 'omp_pause_resource_all', 'omp_get_device_num']]):
            return 4.5
        elif any([line.startswith(func) for func in ['omp_get_max_task_priority', 'omp_max_task_priority']]):
            return 4
        elif any([line.startswith(func) for func in ['omp_in_final', 'omp_get_proc_bind', 'omp_cancellation', 'omp_set_default_device', 'omp_get_default_device', 'omp_display_env', 'omp_get_num_devices','omp_get_num_teams' ,'omp_get_team_num', 'omp_is_initial_device', 'omp_proc_bind']]):
            return 3
    
    return 2


def inspect_file(file_path):
    '''
    return dict of all pragmas omp found
    '''    
    pragmas = []
   
    with open(file_path, 'r') as f:
        code = f.readlines()

        for line in code:
            l = line.lstrip().lower()

            if (l.startswith('#pragma') and 'omp' in l) or l.startswith('omp_'):
                pragmas.append(l)
            
    return pragmas


def version_ratio(json_path):
    version_per_repo = {}
    total_versions = {}
    pragma_counter = {}

    with open(json_path, 'r') as f:
        repo2pragmas = json.load(f)

    for pragmas in repo2pragmas.values():
        data = parser.load(data_path)

    # to be continued


def scan_repos(omp_repo):
    repo2pragmas = {}

    for user_name in os.listdir(omp_repo):
        for repo_name in os.listdir(os.path.join(omp_repo, user_name)):
            repo2pragmas[f'{user_name}/{repo_name}'] = []

            for idx, (root, dirs, files) in enumerate(os.walk(os.path.join(omp_repo, user_name, repo_name))):
                for file in files:
                    repo2pragmas[f'{user_name}/{repo_name}'] += inspect_file(os.path.join(root, file))
    
    with open("repo2pragmas.json", "w") as f:
        json.dump(repo2pragmas, f, sort_keys=True, indent=4, separators=(',', ': '))


def scan_json(json_path):
    '''
    count total use of each version
    '''
    versions = [2, 3, 4, 4.5, 5]
    version_amount = {version:0 for version in versions}
    
    with open(json_path, 'r') as f:
        database = json.load(f)
        
    for pragmas in database.values():
        for pragma in pragmas:
            version = get_omp_version(pragma)
            version_amount[version] += 1

    return version_amount


def get_update_year(reponame):
    '''
        Get the last update date a given repo changed
    '''
    with open('metadata.pickle', 'rb') as f:
        metadata = pickle.load(f)

    for repo in metadata:
        if repo['fullname'] == reponame:
            return repo['created_date'].year

    return None

def scan_json_per_year(json_path):
    '''
    count total use of each version
    '''
    versions = [2, 3, 4, 4.5, 5]
    year2versions = {}
    
    with open(json_path, 'r') as f:
        database = json.load(f)
    
    for repo in database:
        year = get_update_year(repo)

        if year is None:
            continue
        elif year not in year2versions:
            year2versions[year] = {version:0 for version in versions}

        for pragma in database[repo]:
            if pragma.startswith('#pragma'):
                version = get_omp_version(pragma)
                # print(pragma, version)
                year2versions[year][version] += 1

    print(year2versions)
    return year2versions


# print(scan_json('/home/talkad/Downloads/thesis/data_gathering_script/visualization/repo2pragmas.json'))
# versions = {2: 213764, 3: 117565, 4: 163780, 4.5: 3683, 5: 1306}
# [0.4274442209326972, 0.235083923550984, 0.3274958108210791, 0.007364556546916804, 0.0026114881483229285]


# scan_json_per_year('/home/talkad/Downloads/thesis/data_gathering_script/visualization/repo2pragmas.json')

# update date
# all {2021: {2: 40342, 3: 2407, 4: 2968, 4.5: 261, 5: 0}, 2016: {2: 3819, 3: 106, 4: 4, 4.5: 8, 5: 0}, 2022: {2: 75428, 3: 49060, 4: 72473, 4.5: 2119, 5: 689}, 2020: {2: 35093, 3: 38333, 4: 53506, 4.5: 910, 5: 390}, 2019: {2: 15783, 3: 1235, 4: 1615, 4.5: 35, 5: 1}, 2018: {2: 15383, 3: 9160, 4: 12646, 4.5: 98, 5: 97}, 2017: {2: 20245, 3: 17100, 4: 20450, 4.5: 237, 5: 129}, 2014: {2: 1331, 3: 38, 4: 2, 4.5: 3, 5: 0}, 2015: {2: 2183, 3: 56, 4: 101, 4.5: 0, 5: 0}, 2013: {2: 387, 3: 1, 4: 0, 4.5: 4, 5: 0}}
# only pragmas {2021: {2: 37491, 3: 2404, 4: 2968, 4.5: 9, 5: 0}, 2016: {2: 3456, 3: 106, 4: 4, 4.5: 0, 5: 0}, 2022: {2: 70716, 3: 48983, 4: 72473, 4.5: 2040, 5: 657}, 2020: {2: 33390, 3: 38331, 4: 53506, 4.5: 853, 5: 390}, 2019: {2: 12205, 3: 1234, 4: 1615, 4.5: 2, 5: 1}, 2018: {2: 14247, 3: 9160, 4: 12646, 4.5: 84, 5: 97}, 2017: {2: 19203, 3: 17095, 4: 20450, 4.5: 223, 5: 129}, 2014: {2: 1241, 3: 38, 4: 2, 4.5: 0, 5: 0}, 2015: {2: 1656, 3: 56, 4: 101, 4.5: 0, 5: 0}, 2013: {2: 352, 3: 1, 4: 0, 4.5: 0, 5: 0}}

# creation date
# {2021: {2: 31432, 3: 30809, 4: 49056, 4.5: 1731, 5: 542}, 2022: {2: 3951, 3: 238, 4: 279, 4.5: 0, 5: 0}, 2019: {2: 35764, 3: 39687, 4: 55974, 4.5: 860, 5: 390}, 2014: {2: 2431, 3: 171, 4: 47, 4.5: 0, 5: 0}, 2016: {2: 36251, 3: 3671, 4: 4339, 4.5: 23, 5: 12}, 2020: {2: 21631, 3: 3885, 4: 4757, 4.5: 55, 5: 3}, 2017: {2: 29011, 3: 25653, 4: 32271, 4.5: 275, 5: 226}, 2015: {2: 10799, 3: 2134, 4: 2096, 4.5: 32, 5: 0}, 2018: {2: 19012, 3: 10028, 4: 13891, 4.5: 202, 5: 101}, 2013: {2: 3068, 3: 1126, 4: 1048, 4.5: 33, 5: 0}, 2012: {2: 588, 3: 0, 4: 0, 4.5: 0, 5: 0}}

# df = pd.DataFrame({'OpenMP 2.5':  [387, 1331, 2183, 3819, 20245, 15383, 15783, 35093, 40342, 75428] ,
#                    'OpenMP 3':    [1, 38, 56, 106, 17100, 9160 ,1235, 38333, 2407, 49060],
#                     'OpenMP 4':   [0, 2, 101, 4, 20450, 12646, 1615, 53506, 2968, 72473] ,
#                     'OpenMP 4.5': [0, 0, 0, 8, 237, 98, 35, 910, 261, 2119],
#                     'OpenMP 5':   [0, 0, 0, 0, 129, 97, 1, 390, 0, 689]}, index=list(range(2013, 2023)))

# df = pd.DataFrame({'OpenMP 2.5':  [352, 1241, 1656, 3456, 19203, 14247, 12205, 33390, 37491, 70716] ,
#                    'OpenMP 3':    [0, 38, 56, 106, 17095, 9160 ,1235, 38331, 2407, 48983],
#                     'OpenMP 4':   [0, 0, 101, 4, 20450, 12646, 1615, 53506, 2968, 72473] ,
#                     'OpenMP 4.5': [0, 0, 0, 0, 237, 84, 2, 853, 9, 2040],
#                     'OpenMP 5':   [0, 0, 0, 0, 129, 97, 1, 390, 0, 657]}, index=list(range(2013, 2023)))

df = pd.DataFrame({'OpenMP 2.5':  [588, 3068, 2431, 10799, 36251, 29011, 19012, 35764, 21631, 31432, 3951],
                   'OpenMP 3':    [0, 1126, 171, 2134, 3671, 25653, 10028, 39687, 3885, 30809, 238],
                    'OpenMP 4':   [0, 1048, 47, 2096, 4339, 32271, 13891, 55974, 4757, 49056, 279],
                    'OpenMP 4.5': [0, 33, 0, 32, 23, 275, 202, 860, 55, 1731, 0],
                    'OpenMP 5':   [0, 0, 0, 0, 12, 226, 101, 390, 3, 542, 0]}, index=list(range(2012, 2023)))

df.plot.bar(rot=0)
# df.plot.bar(rot=0, stacked=True)

# df = df.apply(lambda x: x*100/sum(x), axis=1)
# print(df)
# df.plot.bar(rot=0, stacked=True)
plt.show()

  
