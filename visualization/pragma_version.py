import os
import json
import pickle
import pandas as pd
import matplotlib.pyplot as plt

plt.rc('axes', labelsize=16)
plt.rc('xtick', labelsize=16)    
plt.rc('ytick', labelsize=16)


def get_omp_version(line):
    '''
        For a given pragma, determine the version of OpenMP it is related
    '''

    if line.startswith('#pragma'):
        if any([(clause in line) for clause in ['requires','loop', 'order(concurrent)', 'scan', 'inscan',  'mutexinoutset', 'taskloop']]) or ('atomic' in line and any([clause in line for clause in ['hint']])) or ('taskwait' in line and 'depend' in line)  or ('task' in line and any([clause in line for clause in ['detach', 'affinity', 'in_reduction']])):
            return 5
        elif any([(clause in line) for clause in ['linear', 'simdlen']]) or ('task' in line and 'priority' in line) or ('target' in line and any([clause in line for clause in ['private', 'nowait', 'is_device_ptr', 'depend', 'firstprivate', 'defaultmap']])):
            return 4.5
        elif any([(clause in line) for clause in ['cancel', 'taskgroup', 'proc_bind', 'target', 'simd']])  or ('task' in line and any([clause in line for clause in ['depend']])):
            return 4
        elif any((clause in line) for clause in ['task', 'collapse', 'taskwait', 'schedule(auto)', 'taskyield']) or ('atomic' in line and any([clause in line for clause in ['read', 'write', 'capture', 'update']])) or ('task' in line and any([clause in line for clause in ['final', 'mergeable']])) :
            return 3
    else:
        if any([line.startswith(func) for func in ['omp_fulfill_event',  'omp_get_supported_active_levels', 'omp_set_affinity', 'omp_get_affinity', 'omp_pause_resource', 'omp_pause_resource_all', 'omp_get_device_num', 'omp_set_affinity_format', 'omp_get_affinity_format']]):
            return 5
        elif any([line.startswith(func) for func in ['omp_get_max_task_priority']]):
            return 4.5
        elif any([line.startswith(func) for func in ['omp_get_cancellation', 'omp_get_team_num', 'omp_is_initial_device', 'omp_get_proc_bind', 'omp_set_default_device', 'omp_get_default_device', 'omp_get_num_devices', 'omp_get_num_teams']]):
            return 4
        elif any([line.startswith(func) for func in ['omp_set_max_active_levels', 'omp_in_final', 'omp_get_thread_limit', 'omp_get_max_active_levels', 'omp_get_active_level', 'omp_get_team_size', 'omp_get_ancestor_thread_num', 'omp_get_level', 'omp_set_schedule', 'omp_get_schedule']]):
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

# redundant_line_comments = re.compile("\/\/.*")
# redundant_multiline_comments = re.compile("\/\*.*?\*\/", re.MULTILINE|re.DOTALL)


# def paren_matcher(fid, code, open_paren='{', closing_paren='}'):
#     '''
#     Parameters:
#         fid - the index of opening paren for strcture block
#         code - string of code

#     Returns:
#         the index of the closing paren
#     '''
#     in_struct = False
#     paren_counter = 0
    
#     for idx, ch in enumerate(code[fid:]):
#         if ch == open_paren:
#             paren_counter += 1
#             in_struct = True
#         elif in_struct and ch == closing_paren:
#             paren_counter -= 1

#         if paren_counter == 0 and in_struct:
#             return idx

#     return -1


# def is_next_char_paren(code):
#     return code.lstrip().startswith('{')


# def is_single_line_for(code):
#     idx = paren_matcher(0, code, open_paren='(', closing_paren=')')
#     return not is_next_char_paren(code[idx + 1:])


# def inspect_file_LOC(file_path):
#     '''
#     Return for a fiven file all openMP verions used
#     '''    
#     with open(file_path, 'r') as f:
#         code = f.read()
#         loc = count_pragmas(code)

#         code = redundant_line_comments.sub("\n", code)
#         code = redundant_multiline_comments.sub("\n", code)
#         code = '\n'.join(filter(lambda line: len(line.lstrip()) > 0, code.split('\n')))
#         code = code.lower()
        
#         total_lines = code.count('\n')+1
#         idx = code.find('#pragma')

#         while idx != -1:
#             code = code[idx:]
#             pragma = code[: code.find('\n')]
#             pragma = pragma[:pragma.find('{')] if '{' in pragma else pragma

#             if 'omp' in pragma and 'parallel' in pragma:
#                 if is_next_char_paren(code[len(pragma):]):
#                     paren_idx = paren_matcher(0, code)
#                     if paren_idx != -1:
#                         loc += code[:paren_idx].count('\n')
#                         code = code[paren_idx + 1:]
#                     else:
#                         return (total_lines, loc)
#                 elif 'for' in pragma and is_single_line_for(code[len(pragma): ]):
#                     loc += 2
#                     code = code[len(pragma):]
#                 elif 'for' in pragma:
#                     paren_idx = paren_matcher(0, code)
#                     if paren_idx != -1:
#                         loc += code[:paren_idx].count('\n')
#                         code = code[paren_idx + 1:]
#                     else:
#                         return (total_lines, loc)
#                 else:
#                     code = code[len(pragma):]
#             else:
#                 code = code[len(pragma):]

#             idx = code.find('#pragma')

#     return (total_lines, loc)


def scan_json(json_path):
    '''
    count total use of each version
    '''
    versions = [2, 3, 4, 4.5, 5]
    version_amount = {version:0 for version in versions}
    total_pragma, task_amount, for_amount = 0, 0, 0

    types = {' for': 0, ' task': 0, ' section': 0, ' critical': 0, ' atomic': 0, ' flush': 0, ' single': 0, ' master': 0}
    
    with open(json_path, 'r') as f:
        database = json.load(f)

    for pragmas in database.values():
        for idx, pragma in enumerate(pragmas):
            prag = pragma[:pragma.find('//')]
            l = prag.split()
            if (len(l) == 3 and l[0] == '#pragma' and l[1] == 'omp' and l[2] == 'parallel') or l[0] != '#pragma':
                continue
            if idx == 200:
                break

            version = get_omp_version(prag)
            version_amount[version] += 1

            if 'pragma' in pragma:
                total_pragma += 1
                for k in types.keys():
                    if k in pragma:
                        types[k] += 1

    print(total_pragma, types)  # 483026 {' for': 211562, ' task': 23556, ' sections': 16192, ' critical': 7824, ' atomic': 12411, ' flush': 2727, ' single': 5388, ' master': 8614} omp_set_lock=769
    return version_amount


def get_update_year(reponame):
    '''
        Get the last update date a given repo changed
    '''
    with open('metadata.pickle', 'rb') as f:
        metadata = pickle.load(f)

    for repo in metadata:
        if repo['fullname'] == reponame:
            return repo['update_date'].year # created_date

    return None


def scan_json_per_year(json_path, include_base=False):
    '''
    count total use of each version
    '''
    versions = [2, 3, 4, 4.5, 5]
    year2versions = {}
    repo_per_year = {}

    with open(json_path, 'r') as f:
        database = json.load(f)

    for repo in database:
        year = get_update_year(repo)
        if year is None:
            continue
        elif year not in year2versions:
            year2versions[year] = {version:0 for version in versions}
            repo_per_year[year] = 0

        repo_per_year[year] += 1

        for idx, pragma in enumerate(database[repo]):
            prag = pragma[:pragma.find('//')]
            version = get_omp_version(prag)
            
            l = prag.split()
            if not include_base:
                if len(l) == 3 and l[0] == '#pragma' and l[1] == 'omp' and l[2] == 'parallel':
                    # print(prag)
                    continue
            if l[0] != '#pragma':
                continue

            if idx == 150:
                break

            year2versions[year][version] += 1

    print(repo_per_year)
    print(year2versions)
    return year2versions


# print(scan_json('/home/talkad/Downloads/thesis/data_gathering_script/visualization/repo2pragmas.json'))
# 483026 {' for': 211562, ' task': 23556, ' section': 16192, ' critical': 7824, ' atomic': 12411, ' flush': 2727, ' single': 5388, ' master': 8614}# {2: 243161, 3: 16666, 4: 184426, 4.5: 39003, 5: 16842}
# {2: 91920, 3: 7207, 4: 49482, 4.5: 7670, 5: 2642}  % [0.5784005889718791, 0.04534957620452929, 0.3113622491678255, 0.04826297342704866, 0.016624612228717413]

# scan_json_per_year('/home/talkad/Downloads/thesis/data_gathering_script/visualization/repo2pragmas.json')
# {2021: 981, 2022: 1344, 2016: 211, 2020: 883, 2019: 700, 2018: 534, 2017: 430, 2014: 117, 2015: 168, 2013: 53}
# include all
#  res_last = {2021: {2: 10648, 3: 830, 4: 664, 4.5: 54, 5: 18}, 2022: {2: 18487, 3: 1313, 4: 1794, 4.5: 119, 5: 62}, 2016: {2: 2310, 3: 105, 4: 21, 4.5: 0, 5: 0}, 2020: {2: 10021, 3: 712, 4: 647, 4.5: 25, 5: 32}, 2019: {2: 7893, 3: 407, 4: 294, 4.5: 5, 5: 9}, 2018: {2: 6406, 3: 371, 4: 60, 4.5: 0, 5: 1}, 2017: {2: 4951, 3: 219, 4: 123, 4.5: 0, 5: 16}, 2014: {2: 1303, 3: 33, 4: 2, 4.5: 0, 5: 0}, 2015: {2: 1890, 3: 80, 4: 85, 4.5: 1, 5: 0}, 2013: {2: 384, 3: 8, 4: 0, 4.5: 0, 5: 0}}
# res = {2021: {2: 8008, 3: 660, 4: 693, 4.5: 30, 5: 31}, 2022: {2: 4248, 3: 271, 4: 231, 4.5: 15, 5: 18}, 2019: {2: 9295, 3: 588, 4: 541, 4.5: 18, 5: 21}, 2014: {2: 2085, 3: 91, 4: 34, 4.5: 7, 5: 0}, 2016: {2: 5271, 3: 282, 4: 208, 4.5: 3, 5: 11}, 2020: {2: 11124, 3: 839, 4: 912, 4.5: 65, 5: 23}, 2017: {2: 7875, 3: 383, 4: 234, 4.5: 31, 5: 11}, 2015: {2: 4371, 3: 292, 4: 394, 4.5: 3, 5: 3}, 2018: {2: 9514, 3: 587, 4: 379, 4.5: 30, 5: 18}, 2013: {2: 1837, 3: 84, 4: 64, 4.5: 2, 5: 2}, 2012: {2: 665, 3: 1, 4: 0, 4.5: 0, 5: 0}}

# without base
# res_creation = {2021: {2: 7659, 3: 671, 4: 764, 4.5: 33, 5: 31}, 2022: {2: 4097, 3: 273, 4: 243, 4.5: 15, 5: 18}, 2019: {2: 9073, 3: 634, 4: 611, 4.5: 19, 5: 22}, 2014: {2: 2060, 3: 105, 4: 36, 4.5: 7, 5: 0}, 2016: {2: 5543, 3: 352, 4: 244, 4.5: 3, 5: 12}, 2020: {2: 11446, 3: 1009, 4: 1056, 4.5: 103, 5: 37}, 2017: {2: 16388, 3: 1019, 4: 14344, 4.5: 2438, 5: 766}, 2015: {2: 4420, 3: 297, 4: 433, 4.5: 3, 5: 3}, 2018: {2: 10239, 3: 648, 4: 428, 4.5: 40, 5: 28}, 2013: {2: 1826, 3: 89, 4: 87, 4.5: 2, 5: 2}, 2012: {2: 643, 3: 1, 4: 0, 4.5: 0, 5: 0}}
res = {2021: {2: 10427, 3: 945, 4: 696, 4.5: 90, 5: 32}, 2022: {2: 19365, 3: 1352, 4: 2432, 4.5: 125, 5: 63}, 2016: {2: 2080, 3: 105, 4: 21, 4.5: 0, 5: 0}, 2020: {2: 9295, 3: 712, 4: 647, 4.5: 25, 5: 35}, 2019: {2: 7369, 3: 447, 4: 304, 4.5: 5, 5: 10}, 2018: {2: 5929, 3: 371, 4: 60, 4.5: 0, 5: 1}, 2017: {2: 4507, 3: 219, 4: 123, 4.5: 0, 5: 16}, 2014: {2: 1199, 3: 33, 4: 2, 4.5: 0, 5: 0}, 2015: {2: 1804, 3: 80, 4: 85, 4.5: 1, 5: 0}, 2013: {2: 371, 3: 8, 4: 0, 4.5: 0, 5: 0}}
res = {2021: {2: 9929, 3: 1005, 4: 764, 4.5: 91, 5: 28}, 2022: {2: 17745, 3: 1678, 4: 2876, 4.5: 238, 5: 135}, 2016: {2: 1906, 3: 99, 4: 21, 4.5: 0, 5: 0}, 2020: {2: 8225, 3: 713, 4: 728, 4.5: 26, 5: 30}, 2019: {2: 8695, 3: 758, 4: 1140, 4.5: 110, 5: 68}, 2018: {2: 5925, 3: 374, 4: 72, 4.5: 0, 5: 1}, 2017: {2: 12729, 3: 817, 4: 14200, 4.5: 2407, 5: 771}, 2014: {2: 1140, 3: 33, 4: 2, 4.5: 0, 5: 0}, 2015: {2: 1545, 3: 66, 4: 111, 4.5: 1, 5: 0}, 2013: {2: 332, 3: 8, 4: 0, 4.5: 0, 5: 0}}


versions = {2: [], 3:[], 4:[], 4.5:[], 5:[]}
for ver in versions.keys():
    for year in sorted(res.keys()):
        versions[ver].append(res[year][ver])

print(versions)


# df = pd.DataFrame({'OpenMP 2.5':  [643, 1826, 2060, 4420, 5543, 16388, 10239, 9073, 11446, 7659, 4097], # 64293
#                    'OpenMP 3':    [1, 89, 105, 297, 352, 1019, 648, 634, 1009, 671, 273], # 4078
#                     'OpenMP 4':   [0, 87, 36, 433, 244, 14344, 428, 611, 1056, 764, 243], # 3690
#                     'OpenMP 4.5': [0, 2, 7, 3, 3, 2438, 40, 19, 103, 33, 15], # 204
#                     'OpenMP 5':   [0, 2, 0, 3, 12, 766, 28, 22, 37, 31, 18]}, index=list(range(2012, 2023))) # 138   => sum 72403

# df = pd.DataFrame({'OpenMP 2.5':  [371, 1199, 1804, 2080, 4507, 5929, 7369, 9295, 10427, 19365],
#                    'OpenMP 3':    [8, 33, 80, 105, 219, 371, 447, 712, 945, 1352],
#                     'OpenMP 4':   [0, 2, 85, 21, 123, 60, 304, 647, 696, 2432], 
#                     'OpenMP 4.5': [0, 0, 0, 0, 0, 0, 5, 25, 90, 125],
#                     'OpenMP 5':   [0, 0, 0, 0, 0, 0, 15, 40, 37, 64]}, index=list(range(2012, 2022))) 


df = pd.DataFrame({'OpenMP 2.5':  [332, 1140, 1545, 2263, 13685, 14482, 9484, 18216, 9959, 19517],
                   'OpenMP 3':    [8, 33, 66, 106, 830, 1030, 817, 1515, 934, 1766],
                    'OpenMP 4':   [0, 2, 111, 21, 14250, 13975, 1201, 15551, 843, 3484], 
                    'OpenMP 4.5': [0, 0, 0, 0, 2407, 2407, 110, 2422, 56, 267],
                    'OpenMP 5':   [0, 0, 0, 0, 0, 622, 571, 1008, 15, 253]}, index=list(range(2013, 2023))) 


ax = df.plot.bar(rot=0, figsize=(16, 10))
# df.plot.bar(rot=0, stacked=True)

# df = df.apply(lambda x: x*100/sum(x), axis=1)
# print(df)
# df.plot.bar(rot=0, stacked=True)

ax.set_yscale('log')
ax.set_title('Use of OpenMP Differrent Versions', fontsize=20)
ax.set_ylabel('Amount of Pragmas')
ax.set_xlabel('Year')


plt.show()

  
