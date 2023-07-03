from csv import DictReader
from datetime import datetime
import json


def get_repo_metadata(metadata_filepath):
    metadata = {}
    prefix = 'https://github.com/'
    suffix = '.git'

    with open(metadata_filepath, 'r') as f:
        dict_reader = DictReader(f)

        for line in list(dict_reader):
            metadata[line['URL'][len(prefix): -len(suffix)]] = {'creation_time': line['creation_time'], 'update_time': line['update_time']}

    return metadata


def get_paradigms_over_time(paradigm_list, metadata_filepath):
    repos_over_time = {}

    langs = ['Fortran', 'cpp'] # ['Fortran', 'c', 'cpp']
    metadata = get_repo_metadata(metadata_filepath)

    for lang in langs:
        with open(f'{lang}_paradigms.json', 'r') as f:
            paradigm_per_repo = json.loads(f.read())

            for repo, paradigms in paradigm_per_repo.items():
                if repo in metadata and all([val for paradigm ,val in paradigms.items() if paradigm in paradigm_list]):
                    dt_object = datetime.strptime(metadata[repo]['update_time'], '%Y-%m-%dT%H:%M:%SZ')

                    year = dt_object.year
                    month = dt_object.month

                    if year not in repos_over_time:
                        repos_over_time[year] = {}

                    if month not in repos_over_time[year]:
                        repos_over_time[year][month] = 0

                    repos_over_time[year][month] += 1

    return repos_over_time


def cumulative_openmp(metadata_filepath):
    total = 0
    openmp_over_time_cumulative = {}

    openmp_over_time = get_paradigms_over_time(['OpenMP'], metadata_filepath)

    for year in range(2012, 2023):
        for month in range(12):
            y, m = year, month+1

            if y in openmp_over_time and m in openmp_over_time[y]:
                total += openmp_over_time[y][m]

            if y not in openmp_over_time_cumulative:
                openmp_over_time_cumulative[y] = {}

            openmp_over_time_cumulative[y][m] = total

    return openmp_over_time_cumulative


def get_paradigm_per_year(paradigm, metadata_filepath):
    paradigm_per_year = {}
    paradigm_over_time = get_paradigms_over_time([paradigm], metadata_filepath)

    for year in paradigm_over_time:
        paradigm_per_year[year] = sum(paradigm_over_time[year].values())

    return paradigm_per_year


def get_version_per_year(metadata_filepath):
    versions_per_year = {}

    langs = ['Fortran', 'cpp'] # ['Fortran', 'c', 'cpp']
    metadata = get_repo_metadata(metadata_filepath)

    for lang in langs:
        with open(f'{lang}_versions.json', 'r') as f:
            versions_per_repo = json.loads(f.read())

            for repo, versions in versions_per_repo.items():
                if repo in metadata:
                    dt_object = datetime.strptime(metadata[repo]['update_time'], '%Y-%m-%dT%H:%M:%SZ')

                    year = dt_object.year

                    if year not in versions_per_year:
                        versions_per_year[year] = {}

                    for version in versions['vers'].keys():
                        total = sum(versions['vers'][version].values())

                        if version not in versions_per_year[year]:
                            versions_per_year[year][version] = 0

                        versions_per_year[year][version] += total

    return versions_per_year


def reduce_paradigms(lang):
    count_paradigms = {'CUDA': 0, 'OpenCL': 0, 'OpenACC': 0, 'SYCL': 0, 
                                          'TBB': 0, 'Cilk': 0, 'OpenMP': 0, 'MPI': 0}

    with open(f'{lang}_paradigms.json', 'r') as f:
        paradigm_per_repo = json.loads(f.read())

        for paradigms in paradigm_per_repo.values():
            for paradigm, val in paradigms.items():
                if val:
                    count_paradigms[paradigm] += 1

        return count_paradigms


def reduce_versions(lang):
    count_versions = {'total_loop': 0, 'vers': {'2': {}, '3':{}, '4':{}, '4.5':{}, '5':{}} }

    with open(f'{lang}_versions.json', 'r') as f:
        version_per_repo = json.loads(f.read())

        for versions in version_per_repo.values():

            count_versions['total_loop'] += versions['total_loop']

            for k in ['2', '3', '4', '4.5', '5']:
                for clause, amount in versions['vers'][k].items():
                    count_versions['vers'][k][clause] = amount if clause not in count_versions['vers'][k] else \
                                                                count_versions['vers'][k][clause]+amount
                    
        return count_versions
    
print(reduce_versions('cpp'))
# print(get_paradigm_per_year('MPI', 'hpcorpus.timestamps.csv'))

# fortran
# {'total_loop': 1203773, 'vers': 
# {'2': {' private': 12466, 'for': 17283, 'shared': 5587, 'parallel_for': 7845, 'critical': 1067, 'schedule_dynamic': 806, 'master': 579, 'schedule_static': 2084, 'section': 977, 'reduction': 2692, ' if': 667, 'barrier': 859, 'firstprivate': 1477, 'single': 549, 'lastprivate': 473, 'nowait': 12, 'collapse': 4781, 'num_threads': 1009, 'flush': 205}, 
# '3': {'atomic_update': 209, 'atomic_write': 263, 'atomic_read': 248, 'taskwait': 312, 'taskyield': 11, 'task': 1656, 'task_final': 50, 'atomic_capture': 196, 'schedule(auto)': 15, 'task_mergeable': 39}, 
# '4': {'cancel': 469, 'proc_bind': 79, 'taskgroup': 233, 'simd': 1199}, 
# '4.5': {'target': 2561, 'target_private': 147, 'target_defaultmap': 148, 'linear': 316, 'simdlen': 96, 'taskloop': 311, 'target_firstprivate': 70, 'target_nowait': 11, 'task_priority': 43}, 
# '5': {'scan': 128, 'order(concurrent)': 120, 'task_in_reduction': 50, 'atomic_hint': 50, 'inscan': 75, 'loop': 436, 'task_detach': 38, 'task_affinity': 38, 'mutexinoutset': 2}}}


# cpp
# {'total_loop': 19841237, 'vers': 
# {'2': {'parallel_for': 89489, 'critical': 8657, 'for': 89489, 'collapse': 5176, 'section': 9654, 'schedule_static': 5689, 'single': 3473, 'master': 7871, ' if': 8724, ' private': 14615, 'num_threads': 4588, 'reduction': 23205, 'schedule_dynamic': 3842, 'shared': 10957, 'flush': 1398, 'firstprivate': 10423, 'barrier': 2728, 'nowait': 2276, 'lastprivate': 8524}, 
# '3': {'taskwait': 1012, 'task': 18272, 'taskyield': 705, 'atomic_update': 552, 'atomic_capture': 1497, 'schedule(auto)': 208, 'atomic_write': 288, 'atomic_read': 242, 'task_mergeable': 98, 'task_final': 431}, 
# '4': {'cancel': 988, 'simd': 54557, 'task_depend': 455, 'taskgroup': 1113, 'proc_bind': 798}, 
# '4.5': {'linear': 4648, 'target': 79126, 'target_private': 6748, 'simdlen': 1229, 'target_defaultmap': 1688, 'target_depend': 4129, 'task_priority': 476, 'taskloop': 11074, 'target_firstprivate': 2612, 'target_nowait': 716}, 
# '5': {'loop': 11139, 'task_in_reduction': 1331, 'scan': 341, 'task_detach': 52, 'order(concurrent)': 50, 'atomic_hint': 25, 'inscan': 77, 'mutexinoutset': 12, 'task_affinity': 96, 'taskwait_depend': 37}}}