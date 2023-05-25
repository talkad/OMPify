import os
import json
import pickle
import pandas as pd
from tqdm import tqdm
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
            return 2
    elif 'omp_' in line:
        if any([line.startswith(func) for func in ['omp_fulfill_event',  'omp_get_supported_active_levels', 'omp_set_affinity', 'omp_get_affinity', 'omp_pause_resource', 'omp_pause_resource_all', 'omp_get_device_num', 'omp_set_affinity_format', 'omp_get_affinity_format']]):
            return 5
        elif any([line.startswith(func) for func in ['omp_get_max_task_priority']]):
            return 4.5
        elif any([line.startswith(func) for func in ['omp_get_cancellation', 'omp_get_team_num', 'omp_is_initial_device', 'omp_get_proc_bind', 'omp_set_default_device', 'omp_get_default_device', 'omp_get_num_devices', 'omp_get_num_teams']]):
            return 4
        elif any([line.startswith(func) for func in ['omp_set_max_active_levels', 'omp_in_final', 'omp_get_thread_limit', 'omp_get_max_active_levels', 'omp_get_active_level', 'omp_get_team_size', 'omp_get_ancestor_thread_num', 'omp_get_level', 'omp_set_schedule', 'omp_get_schedule']]):
            return 3
        else:
            return 2

    return -1
    

def get_omp_version_per_clause(line, res):
    '''
        For a given pragma, determine the version of OpenMP it is related
    '''
    if line.startswith('#pragma'):
        if any([(clause in line) for clause in ['requires','loop', 'order(concurrent)', 'scan', 'inscan',  'mutexinoutset', 'taskloop']]) or ('atomic' in line and any([clause in line for clause in ['hint']])) or ('taskwait' in line and 'depend' in line)  or ('task' in line and any([clause in line for clause in ['detach', 'affinity', 'in_reduction']])):
            clauses = [clause for clause in ['requires','loop', 'order(concurrent)', 'scan', 'inscan',  'mutexinoutset', 'taskloop'] if clause in line]
            if 'atomic' in line and 'hint' in line:
                clauses.append('atomic_hint')
            if 'taskwait' in line and 'depend' in line:
                clauses.append('taskwait_depend')
            if 'task' in line:
                clauses += ['task_'+clause for clause in ['detach', 'affinity', 'in_reduction'] if clause in line]

            for clause in clauses:
                res['5'][clause] = 1 if clause not in res['5'] else res['5'][clause]+1

        elif any([(clause in line) for clause in ['linear', 'simdlen']]) or ('task' in line and 'priority' in line) or ('target' in line and any([clause in line for clause in ['private', 'nowait', 'is_device_ptr', 'depend', 'firstprivate', 'defaultmap']])):
            clauses = [clause for clause in ['linear', 'simdlen'] if clause in line]
            if 'task' in line and 'priority' in line:
                clauses.append('task_priority')
            if 'target' in line:
                clauses += ['target_'+clause for clause in ['private', 'nowait', 'is_device_ptr', 'depend', 'firstprivate', 'defaultmap'] if clause in line]
            
            for clause in clauses:
                res['4.5'][clause] = 1 if clause not in res['4.5'] else res['4.5'][clause]+1

        elif any([(clause in line) for clause in ['cancel', 'taskgroup', 'proc_bind', 'target', 'simd']])  or ('task' in line and any([clause in line for clause in ['depend']])):
            clauses = [clause for clause in ['cancel', 'taskgroup', 'proc_bind', 'target', 'simd'] if clause in line]
            if 'task' in line and 'depend' in line:
                clauses.append('task_depend')

            for clause in clauses:
                res['4'][clause] = 1 if clause not in res['4'] else res['4'][clause]+1

        elif any((clause in line) for clause in ['task', 'collapse', 'taskwait', 'schedule(auto)', 'taskyield']) or ('atomic' in line and any([clause in line for clause in ['read', 'write', 'capture', 'update']])) or ('task' in line and any([clause in line for clause in ['final', 'mergeable']])) :
            clauses = [clause for clause in ['task', 'collapse', 'taskwait', 'schedule(auto)', 'taskyield'] if clause in line]
            if 'task' in line:
                clauses += ['task_'+clause for clause in ['final', 'mergeable'] if clause in line]
            if 'atomic' in line:
                clauses += ['atomic_'+clause for clause in ['read', 'write', 'capture', 'update'] if clause in line]
            
            for clause in clauses:
                res['3'][clause] = 1 if clause not in res['3'] else res['3'][clause]+1
        else:
            res['2'] += 1


# def iterate_jsons(json_dir):
#     versions = {2: 0, 3:0, 4:0, 4.5:0, 5:0}

#     for json_file in tqdm(os.listdir(json_dir)):
#         with open(os.path.join(json_dir, json_file), 'r') as f:
#             for line in f:
#                 js = json.loads(line.strip())

#                 if 'content' not in js:
#                     continue

#                 for line in js['content'].split('\n'):
#                     l = line.lstrip().lower()

#                     ver = get_omp_version(l)

#                     if ver > 0:
#                         versions[ver] += 1

#     return versions


def iterate_jsons(json_dir):
    versions = {'2': 0, '3':{}, '4':{}, '4.5':{}, '5':{}}

    for json_file in tqdm(os.listdir(json_dir)):
        with open(os.path.join(json_dir, json_file), 'r') as f:
            for line in f:
                js = json.loads(line.strip())

                if 'content' not in js:
                    continue

                for line in js['content'].split('\n'):
                    l = line.lstrip().lower()

                    get_omp_version_per_clause(l, versions)

    return versions


print(iterate_jsons('/home/talkad/shared/talkad_Datasets/dataset_gal_cpp/cpp'))
# print(iterate_jsons('/home/talkad/shared/nadavsc/c'))

# cpp
# {2: 1094639, 3: 10115, 4: 99928, 4.5: 20374, 5: 18066}

# c
# {2: 3306337, 3: 9605, 4: 22708, 4.5: 3925, 5: 29305}



# c
# {'2': 458685, 
# '3': {'task': 4947, 'collapse': 1268, 'taskwait': 802, 'atomic_read': 621, 'atomic_write': 634, 'atomic_update': 375, 'atomic_capture': 1152, 'task_final': 46, 'task_mergeable': 9, 'schedule(auto)': 39, 'taskyield': 39}, 
# '4': {'simd': 10268, 'task_depend': 479, 'target': 10459, 'proc_bind': 259, 'taskgroup': 627, 'cancel': 2070}, 
# '4.5': {'target_private': 1233, 'target_firstprivate': 463, 'simdlen': 1146, 'task_priority': 47, 'linear': 1250, 'target_defaultmap': 112, 'target_is_device_ptr': 96, 'target_nowait': 157, 'target_depend': 86}, 
# '5': {'loop': 26954, 'taskloop': 1903, 'mutexinoutset': 26, 'requires': 258, 'scan': 1517, 'task_in_reduction': 121, 'atomic_hint': 48, 'inscan': 175, 'order(concurrent)': 138, 'taskwait_depend': 24, 'task_detach': 42, 'task_affinity': 158}}

# cpp
# {'2': 449197, 
# '3': {'task': 4025, 'taskwait': 665, 'collapse': 1656, 'atomic_capture': 1132, 'atomic_update': 397, 'schedule(auto)': 83, 'atomic_write': 212, 'atomic_read': 180, 'taskyield': 539, 'task_mergeable': 16, 'task_final': 102}, 
# '4': {'simd': 32706, 'target': 49914, 'taskgroup': 901, 'proc_bind': 546, 'cancel': 699, 'task_depend': 240}, 
# '4.5': {'target_private': 4883, 'target_is_device_ptr': 1811, 'target_firstprivate': 1830, 'linear': 2864, 'target_nowait': 470, 'target_depend': 2883, 'target_defaultmap': 1113, 'simdlen': 691, 'task_priority': 66}, 
# '5': {'loop': 10495, 'taskloop': 8619, 'scan': 1226, 'requires': 1092, 'task_in_reduction': 1130, 'order(concurrent)': 38, 'mutexinoutset': 336, 'inscan': 56, 'taskwait_depend': 36, 'atomic_hint': 13, 'task_affinity': 96, 'task_detach': 52}}