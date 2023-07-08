from csv import DictReader
from datetime import datetime
import json


def unite_paradigms():
    '''
    Join the different JSON files by repo name
    '''
    repos = {}

    for lang in ['Fortran', 'c', 'cpp']:
        with open(f'analyzed_data/{lang}_paradigms.json', 'r') as f:
            paradigms = json.loads(f.read())

            for repo, pars in paradigms.items():
                if repo not in repos:
                    repos[repo] = {'CUDA': False, 'OpenCL': False, 'OpenACC': False, 'SYCL': False, 
                                          'TBB': False, 'Cilk': False, 'OpenMP': False, 'MPI': False}

                for par, val in pars.items():
                    repos[repo][par] |= val

    with open('analyzed_data/total_paradigms.json', 'w') as f:
        f.write(json.dumps(repos))

    
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

            metadata[line['URL'][len(prefix): -len(suffix)]] = {'creation_time': creation_time, 'update_time': update_time, 'fork': fork}

    return metadata


# def debug(metadata_filepath):

#     metadata = get_repo_metadata(metadata_filepath)
#     with open('analyzed_data/total_paradigms.json', 'r') as f:
#         paradigm_per_repo = json.loads(f.read())

#         for repo in metadata:
#             if repo not in paradigm_per_repo:
#                 print(repo)
    
    

def get_paradigms_over_time(paradigm_list, metadata_filepath, avoid_fork=True, key='update_time'):
    '''
    Get the number of repositories that used different parallelization APIs per month.
    '''
    repos_over_time = {}
    total = 0
    metadata = get_repo_metadata(metadata_filepath)

    with open('analyzed_data/total_paradigms.json', 'r') as f:
        paradigm_per_repo = json.loads(f.read())

        for repo, paradigms in paradigm_per_repo.items():
            if repo in metadata and all([val for paradigm ,val in paradigms.items() if paradigm in paradigm_list]):
                total += 1

                if avoid_fork and metadata[repo]['fork']:
                    continue

                dt_object = metadata[repo][key]

                year = dt_object.year
                month = dt_object.month

                if year not in repos_over_time:
                    repos_over_time[year] = {}

                if month not in repos_over_time[year]:
                    repos_over_time[year][month] = 0

                repos_over_time[year][month] += 1

    return repos_over_time


def get_total_repos_over_time(metadata_filepath):
    '''
    Figure 2

    Get the total usage of HPCorpus repositories
    '''
    paradigm_per_year = {}
    paradigm_over_time = get_paradigms_over_time([], metadata_filepath, avoid_fork=False, key='creation_time')

    for year in paradigm_over_time:
        paradigm_per_year[year] = sum(paradigm_over_time[year].values())

    return paradigm_per_year


def aggregate_paradigms(metadata_filepath):
    '''
    Figure 3

    Aggregate the usage of each parallelization API.
    '''
    metadata = get_repo_metadata(metadata_filepath)
    count_paradigms = {'CUDA': 0, 'OpenCL': 0, 'OpenACC': 0, 'SYCL': 0, 
                            'TBB': 0, 'Cilk': 0, 'OpenMP': 0, 'MPI': 0}
    amount_repos = 0
    amount_repo_forked = 0
    amount_missed_repo = 0

    with open('analyzed_data/total_paradigms.json', 'r') as f:
        paradigm_per_repo = json.loads(f.read())

        for repo_name, paradigms in paradigm_per_repo.items():
            
            if repo_name not in metadata:
                amount_missed_repo += 1
                continue

            if metadata[repo_name]['fork']:
                amount_repo_forked += 1
                continue

            amount_repos += 1

            for paradigm, val in paradigms.items():
                if val:
                    count_paradigms[paradigm] += 1

        print(f'Amount of valid repos: {amount_repos}')
        print(f'Amount of repos not exist in metadata: {amount_missed_repo}')
        print(f'Amount of forked repos: {amount_repo_forked}')

        return count_paradigms
    

def cumulative_openmp(metadata_filepath):
    '''
    Figure 4

    Accumulate the usage of OpenMP API over the last decade
    '''
    total = 0
    openmp_over_time_cumulative = {}

    openmp_over_time = get_paradigms_over_time(['OpenMP'], metadata_filepath, avoid_fork=True, key='update_time')

    for year in range(2008, 2024):
        for month in range(12):
            y, m = year, month+1

            if y in openmp_over_time and m in openmp_over_time[y]:
                total += openmp_over_time[y][m]

            if y not in openmp_over_time_cumulative:
                openmp_over_time_cumulative[y] = {}

            openmp_over_time_cumulative[y][m] = total

    return openmp_over_time_cumulative


def get_paradigm_per_year(paradigms, metadata_filepath, key='creation_time'):
    '''
    Get the usage of a given parallelization API over the last decade.
    '''
    paradigm_per_year = {}
    paradigm_over_time = get_paradigms_over_time(paradigms, metadata_filepath, key=key)

    for year in paradigm_over_time:
        paradigm_per_year[year] = sum(paradigm_over_time[year].values())

    return paradigm_per_year


def get_paradigms_per_year(metadata_filepath):
    '''
    Figure 5

    Get the usage of each parallelization API per year
    '''
    paradigms_per_year = {}

    for paradigm in ['CUDA', 'OpenCL', 'OpenACC', 'SYCL', 'TBB', 'Cilk', 'OpenMP', 'MPI']:
        paradigms_per_year[paradigm] = get_paradigm_per_year([paradigm], metadata_filepath, key='update_time')

    return paradigms_per_year


def get_omp_mpi_usage(metadata_filepath):
    '''
    Figure 6

    Get the usage of OpenMP + MPI
    '''
    return get_paradigm_per_year(['OpenMP', 'MPI'], metadata_filepath, key='update_time')


def get_version_per_year(metadata_filepath):
    '''
    Figure 7

    Get the usage of each OpenMP version across the last decade
    '''
    versions_per_year = {}

    langs = ['Fortran', 'c', 'cpp']
    metadata = get_repo_metadata(metadata_filepath)

    for lang in langs:
        with open(f'analyzed_data/{lang}_versions.json', 'r') as f:
            versions_per_repo = json.loads(f.read())

            for repo, versions in versions_per_repo.items():
                if repo in metadata:
                    dt_object = metadata[repo]['update_time']

                    year = dt_object.year

                    if year not in versions_per_year:
                        versions_per_year[year] = {}

                    for version in versions['vers'].keys():
                        total = sum(versions['vers'][version].values())

                        if version not in versions_per_year[year]:
                            versions_per_year[year][version] = 0

                        versions_per_year[year][version] += total

    return versions_per_year


def get_loops():
    '''
    Figure 8

    get for-loop usage
    '''
    result = {}

    for lang in ['c', 'cpp', 'Fortran']:
        count_versions = aggregate_versions(lang)
        result[lang] = {'total_loop': count_versions['total_loop'],
                        'omp_for': count_versions['vers']['ver2.5']['do_' if lang=='Fortran' else 'for_']}
        
    return result


def get_scheduling():
    '''
    Figure 10

    Get scheduling  usage
    '''
    result = {}

    for lang in ['c', 'cpp', 'Fortran']:
        count_versions = aggregate_versions(lang)

        result[lang] = {'schedule_static': count_versions['vers']['ver2.5']['schedule_static'],
                        'schedule_dynamic': count_versions['vers']['ver2.5']['schedule_dynamic'],
                        'schedule_guided': count_versions['vers']['ver2.5']['schedule_guided'],
                        'schedule_auto': count_versions['vers']['ver3.0']['schedule_auto'],
                        'schedule_runtime': count_versions['vers']['ver2.5']['schedule_runtime']}
        
    return result


def aggregate_versions(lang):
    '''
    Figure 11

    Aggregate the usage of each OpenMP directive
    '''
    count_versions = {'total_loop': 0, 'vers': {'ver2.5': {}, 'ver3.0':{}, 'ver3.1':{}, 'ver4.0':{}, 'ver4.5':{}, 'ver5.0':{}, 'ver5.1':{}, 'ver5.2':{}} }

    with open(f'analyzed_data/{lang}_versions.json', 'r') as f:
        version_per_repo = json.loads(f.read())

        for versions in version_per_repo.values():

            count_versions['total_loop'] += versions['total_loop']

            for k in ['ver2.5', 'ver3.0', 'ver3.1', 'ver4.0', 'ver4.5', 'ver5.0', 'ver5.1', 'ver5.2']:
                for clause, amount in versions['vers'][k].items():
                    count_versions['vers'][k][clause] = amount if clause not in count_versions['vers'][k] else \
                                                                count_versions['vers'][k][clause]+amount
                    
        return count_versions
    



# unite_paradigms()

# print(get_total_repos_over_time('analyzed_data/hpcorpus.timestamps.csv'))
# niranjan sum = 230043
# tal with fork = 218761
# tal no fork = 189903





# print(aggregate_paradigms('analyzed_data/hpcorpus.timestamps.csv'))
# Amount of valid repos: 189903
# Amount of repos not exist in metadata: 51193
# Amount of forked repos: 28858
# {'CUDA': 879, 'OpenCL': 956, 'OpenACC': 61, 'SYCL': 24, 'TBB': 374, 'Cilk': 97, 'OpenMP': 3881, 'MPI': 2115}


# print(cumulative_openmp('analyzed_data/hpcorpus.timestamps.csv'))


# print(get_paradigms_per_year('analyzed_data/hpcorpus.timestamps.csv'))






# print(get_version_per_year('analyzed_data/hpcorpus.timestamps.csv'))
# {2023: {'ver4.0': 305673, 'ver2.5': 281990, 'ver3.1': 5646, 'ver4.5': 32471, 'ver5.0': 60423, 'ver3.0': 7217, 'ver5.2': 19251, 'ver5.1': 814}, 2014: {'ver4.0': 192, 'ver2.5': 12143, 'ver3.1': 12, 'ver4.5': 48, 'ver5.0': 2, 'ver3.0': 214, 'ver5.2': 55, 'ver5.1': 0}, 2021: {'ver4.0': 2093, 'ver2.5': 26843, 'ver3.1': 248, 'ver4.5': 375, 'ver5.0': 17, 'ver3.0': 163, 'ver5.2': 982, 'ver5.1': 0}, 2022: {'ver4.0': 24374, 'ver2.5': 72646, 'ver3.1': 4319, 'ver4.5': 4655, 'ver5.0': 4420, 'ver3.0': 1575, 'ver5.2': 2706, 'ver5.1': 0}, 2015: {'ver4.0': 1288, 'ver2.5': 44075, 'ver3.1': 450, 'ver4.5': 549, 'ver5.0': 80, 'ver3.0': 662, 'ver5.2': 426, 'ver5.1': 0}, 2016: {'ver4.0': 3953, 'ver2.5': 39625, 'ver3.1': 619, 'ver4.5': 997, 'ver5.0': 47, 'ver3.0': 1126, 'ver5.2': 697, 'ver5.1': 0}, 2020: {'ver4.0': 540, 'ver2.5': 16681, 'ver3.1': 77, 'ver4.5': 133, 'ver5.0': 2, 'ver3.0': 326, 'ver5.2': 464, 'ver5.1': 0}, 2017: {'ver4.0': 59524, 'ver2.5': 42243, 'ver3.1': 603, 'ver4.5': 2641, 'ver5.0': 6530, 'ver3.0': 929, 'ver5.2': 2522, 'ver5.1': 0}, 2018: {'ver4.0': 139, 'ver2.5': 15217, 'ver3.1': 93, 'ver4.5': 80, 'ver5.0': 1, 'ver3.0': 28, 'ver5.2': 22, 'ver5.1': 0}, 2013: {'ver4.0': 0, 'ver2.5': 2508, 'ver3.1': 0, 'ver4.5': 18, 'ver5.0': 1, 'ver3.0': 44, 'ver5.2': 1, 'ver5.1': 0}, 2019: {'ver4.0': 736, 'ver2.5': 22814, 'ver3.1': 323, 'ver4.5': 290, 'ver5.0': 26, 'ver3.0': 88, 'ver5.2': 275, 'ver5.1': 0}, 2012: {'ver4.0': 0, 'ver2.5': 0, 'ver3.1': 0, 'ver4.5': 0, 'ver5.0': 0, 'ver3.0': 0, 'ver5.2': 0, 'ver5.1': 0}}


# print(get_loops())
# {'c': {'total_loop': 21822609, 'omp_for': 29389}, 'cpp': {'total_loop': 19841237, 'omp_for': 89397}, 'Fortran': {'total_loop': 1203773, 'omp_for': 17251}}

# print(get_scheduling())
# {'c': {'schedule_static': 8705, 'schedule_dynamic': 2362, 'schedule_guided': 340, 'schedule_auto': 54, 'schedule_runtime': 162}, 'cpp': {'schedule_static': 4897, 'schedule_dynamic': 3772, 'schedule_guided': 1498, 'schedule_auto': 240, 'schedule_runtime': 377}, 'Fortran': {'schedule_static': 2013, 'schedule_dynamic': 805, 'schedule_guided': 285, 'schedule_auto': 27, 'schedule_runtime': 131}}




# for lang in ['c', 'cpp', 'Fortran']:
#     print(aggregate_versions(lang))

# C {'total_loop': 21822609, 'vers': 
# {'ver2.5': {'shared_': 9126, 'critical_': 4755, 'lastprivate_': 1804, 'parallel_': 33969, 'private_': 11106, 'firstprivate_': 2275, 'for_': 29389, 'flush_': 589, 'default_': 7050, 'parallel_for_': 22154, 'num_threads_': 2285, 'reduction_': 3390, 'schedule_static': 8705, 'schedule_': 11719, 'single_': 1019, 'master_': 1244, 'ordered_': 924, 'barrier_': 2825, 'nowait_': 1088, 'schedule_dynamic': 2362, 'threadprivate_': 328, 'section_': 1395, 'sections_': 1087, 'atomic_': 3601, 'parallel_sections_': 400, 'schedule_guided': 340, 'copyin_': 187, 'copyprivate_': 16, 'schedule_runtime': 162}, 
# 'ver3.0': {'taskwait_': 640, 'task_': 1903, 'atomic_read_': 330, 'atomic_update_': 195, 'atomic_write_': 543, 'schedule_auto': 54, 'atomic_capture_': 424}, '
# ver3.1': {'collapse_': 2861, 'taskyield_': 29}, 
# 'ver4.0': {'target_': 6199, 'map_': 1647, 'declare_target_': 296, 'simd_': 9712, 'declare_simd_': 1064, 'cancel_': 633, 'teams_distribute_simd_': 714, 'distribute_simd_': 2095, 'proc_bind_': 397, 'target_teams_distribute_': 1162, 'target_update_': 218, 'teams_': 3112, 'cancellation_': 602, 'target_teams_': 1313, 'target_data_': 522, 'distribute_': 3145, 'target_teams_distribute_simd_': 582, 'simdlen_': 828, 'teams_distribute_': 1547, 'taskgroup_': 481, 'declare_reduction_': 292}, 
# 'ver4.5': {'if_': 1829, 'taskloop_simd_': 563, 'target_enter_data_': 147, 'defaultmap_': 181, 'linear_': 973, 'target_exit_data_': 135, 'taskloop_': 1251, 'depend_': 817}, 
# 'ver5.0': {'target_simd_': 1501, 'taskloop_simd_': 563, 'parallel_target_': 1283, 'copyprivate_': 16, 'allocate_': 362, 'task_reduction_': 68, 'in_reduction_': 166, 'loop_': 195, 'parallel_master_': 203, 'parallel_loop_': 28, 'declare_variant_': 619, 'declare_mapper_': 51, 'master_taskloop_': 481, 'requires_': 70, 'parallel_master_taskloop_': 162, 'master_taskloop_simd_': 203, 'parallel_master_taskloop_simd_': 24, 'scan_': 140, 'teams_loop_': 21, 'target_parallel_loop_': 10, 'target_teams_loop_': 8}, 
# 'ver5.1': {'masked_': 34, 'parallel_masked_taskloop_simd_': 3, 'masked_taskloop_simd_': 6, 'masked_taskloop_': 14, 'scope_': 35, 'error_': 68, 'assume_': 26, 'parallel_masked_': 13, 'nothing_': 21, 'unroll_': 9, 'tile_': 1},
# 'ver5.2': {'mergeable_': 62, 'device_': 480, 'aligned_': 1956, 'to_': 192, 'grainsize_': 79, 'final_': 121, 'thread_limit_': 293, 'uniform_': 639, 'num_tasks_': 69, 'nogroup_': 80, 'safelen_': 918, 'dist_schedule_': 271, 'enter_': 151, 'is_device_ptr_': 240, 'untied_': 431, 'priority_': 102, 'use_device_ptr_': 10, 'initializer_': 132, 'from_': 107, 'message_': 31, 'match_': 542, 'device_type_': 22, 'hint_': 59, 'nontemporal_': 424, 'begin_declare_variant_': 39, 'bind_': 99, 'severity_': 31, 'use_device_addr_': 30, 'doacross_': 85, 'link_': 14, 'assumes_': 116, 'exclusive_': 69, 'allocator_': 14, 'align_': 13, 'at_': 33, 'filter_': 27, 'destroy_': 20, 'begin_declare_target_': 3, 'begin_assumes_': 38, 'inclusive_': 70, 'when_': 22, 'metadirective_': 22, 'partial_': 7, 'append_args_': 2, 'full_': 1, 'sizes_': 1}}}


# CPP {'total_loop': 19841237, 'vers': 
# {'ver2.5': {'parallel_': 113947, 'for_': 89397, 'parallel_for_': 71275, 'critical_': 8657, 'section_': 2528, 'sections_': 7038, 'parallel_sections_': 3384, 'schedule_static': 4897, 'single_': 3460, 'master_': 7732, 'schedule_': 11203, 'reduction_': 19227, 'private_': 14000, 'num_threads_': 4514, 'default_': 7642, 'ordered_': 2567, 'schedule_dynamic': 3772, 'shared_': 7908, 'flush_': 1355, 'firstprivate_': 9639, 'atomic_': 5360, 'barrier_': 2728, 'schedule_guided': 1498, 'nowait_': 2238, 'lastprivate_': 7557, 'threadprivate_': 3158, 'schedule_runtime': 377, 'copyprivate_': 239, 'copyin_': 489}, 
# 'ver3.0': {'taskwait_': 1012, 'task_': 4169, 'atomic_update_': 546, 'atomic_capture_': 1491, 'schedule_auto': 240, 'atomic_read_': 242, 'atomic_write_': 288}, 
# 'ver3.1': {'collapse_': 5176, 'taskyield_': 705}, 
# 'ver4.0': {'simd_': 54468, 'cancel_': 492, 'teams_': 47679, 'target_': 78930, 'target_teams_': 21906, 'target_teams_distribute_': 18067, 'distribute_': 43111, 'map_': 14411, 'teams_distribute_': 31217, 'declare_simd_': 436, 'simdlen_': 1225, 'declare_reduction_': 585, 'teams_distribute_simd_': 16639, 'distribute_simd_': 24021, 'declare_target_': 939, 'target_update_': 2581, 'target_teams_distribute_simd_': 9488, 'target_data_': 4717, 'taskgroup_': 1113, 'proc_bind_': 698, 'cancellation_': 216}, 
# 'ver4.5': {'if_': 8185, 'depend_': 4851, 'linear_': 4508, 'taskloop_simd_': 5777, 'target_enter_data_': 1282, 'defaultmap_': 1534, 'target_exit_data_': 1359, 'taskloop_': 11074}, 
# 'ver5.0': {'copyprivate_': 239, 'parallel_target_': 23052, 'allocate_': 1312, 'declare_mapper_': 136, 'task_reduction_': 840, 'taskloop_simd_': 5777, 'master_taskloop_': 4841, 'master_taskloop_simd_': 2559, 'declare_variant_': 1038, 'parallel_master_taskloop_': 2270, 'in_reduction_': 1187, 'target_simd_': 18034, 'parallel_master_': 3118, 'parallel_master_taskloop_simd_': 1195, 'requires_': 115, 'scan_': 255, 'loop_': 25}, 
# 'ver5.1': {'dispatch_': 40, 'interop_': 72, 'unroll_': 45, 'tile_': 44, 'masked_': 29}, 
# 'ver5.2': {'message_': 3, 'uniform_': 47, 'untied_': 194, 'mergeable_': 98, 'safelen_': 2307, 'device_type_': 51, 'to_': 1920, 'grainsize_': 346, 'match_': 935, 'link_': 69, 'final_': 390, 'thread_limit_': 1000, 'enter_': 1282, 'num_tasks_': 381, 'nogroup_': 152, 'aligned_': 3274, 'from_': 867, 'priority_': 476, 'initializer_': 237, 'device_': 1050, 'use_device_ptr_': 247, 'dist_schedule_': 1064, 'is_device_ptr_': 2328, 'exclusive_': 42, 'allocator_': 177, 'nontemporal_': 108, 'destroy_': 39, 'hint_': 81, 'init_': 38, 'inclusive_': 67, 'begin_declare_variant_': 21, 'use_device_addr_': 47, 'begin_assumes_': 22, 'assumes_': 56, 'sizes_': 39, 'metadirective_': 26, 'novariants_': 6, 'partial_': 24, 'append_args_': 35, 'when_': 25, 'adjust_args_': 11, 'full_': 9, 'use_': 17, 'align_': 26, 'at_': 1, 'bind_': 22, 'indirect_': 10, 'filter_': 16, 'begin_declare_target_': 6, 'nocontext_': 6}}}


# FORTRAN {'total_loop': 1203773, 'vers': 
# {'ver2.5': {'parallel_': 13871, 'default_': 4659, 'private_': 12675, 'shared_': 3234, 'do_': 17251, 'parallel_do_': 7839, 'critical_': 1066, 'schedule_dynamic': 805, 'schedule_': 3258, 'threadprivate_': 758, 'atomic_': 2005, 'master_': 556, 'schedule_static': 2013, 'reduction_': 2624, 'sections_': 387, 'section_': 579, 'barrier_': 858, 'single_': 533, 'firstprivate_': 1466, 'parallel_sections_': 129, 'schedule_guided': 285, 'copyin_': 98, 'ordered_': 468, 'lastprivate_': 473, 'nowait_': 12, 'schedule_runtime': 131, 'parallel_workshare_': 258, 'num_threads_': 842, 'flush_': 205, 'workshare_': 574, 'copyprivate_': 1}, 
# 'ver3.0': {'atomic_update_': 193, 'atomic_capture_': 177, 'schedule_auto': 27, 'task_': 663, 'taskwait_': 312, 'atomic_read_': 241, 'atomic_write_': 260}, 
# 'ver3.1': {'collapse_': 4781, 'taskyield_': 11}, 
# 'ver4.0': {'declare_reduction_': 186, 'teams_distribute_simd_': 160, 'distribute_simd_': 236, 'simd_': 1129, 'proc_bind_': 79, 'declare_target_': 329, 'teams_': 656, 'target_update_': 412, 'simdlen_': 89, 'cancel_': 270, 'cancellation_': 198, 'target_': 2532, 'target_teams_': 355, 'target_teams_distribute_': 230, 'distribute_': 562, 'target_teams_distribute_simd_': 103, 'target_data_': 553, 'declare_simd_': 132, 'map_': 991, 'teams_distribute_': 337, 'taskgroup_': 232}, 
# 'ver4.5': {'taskloop_simd_': 124, 'if_': 626, 'target_enter_data_': 94, 'defaultmap_': 148, 'linear_': 305, 'target_exit_data_': 55, 'taskloop_': 307}, 
# 'ver5.0': {'in_reduction_': 56, 'taskloop_simd_': 124, 'task_reduction_': 37, 'parallel_target_': 260, 'requires_': 79, 'scan_': 53, 'target_simd_': 137, 'loop_': 115, 'parallel_loop_': 28, 'allocate_': 187, 'parallel_master_taskloop_simd_': 18, 'declare_variant_': 237, 'target_teams_loop_': 9, 'parallel_master_taskloop_': 31, 'teams_loop_': 19, 'target_parallel_loop_': 9, 'master_taskloop_': 65, 'parallel_master_': 43, 'master_taskloop_simd_': 38, 'copyprivate_': 1}, 
# 'ver5.1': {'masked_': 93, 'parallel_masked_taskloop_simd_': 11, 'masked_taskloop_simd_': 24, 'masked_taskloop_': 40, 'scope_': 55, 'error_': 83, 'assume_': 12, 'parallel_masked_': 31, 'nothing_': 5}, 
# 'ver5.2': {'use_': 217, 'to_': 303, 'thread_limit_': 143, 'enter_': 102, 'aligned_': 86, 'uniform_': 37, 'use_device_addr_': 154, 'final_': 50, 'link_': 78, 'is_device_ptr_': 28, 'from_': 217, 'untied_': 58, 'use_device_ptr_': 42, 'device_': 271, 'safelen_': 95, 'device_type_': 66, 'grainsize_': 62, 'exclusive_': 20, 'dist_schedule_': 127, 'nontemporal_': 51, 'initializer_': 131, 'num_tasks_': 44, 'nogroup_': 64, 'hint_': 70, 'inclusive_': 31, 'init_': 2, 'mergeable_': 39, 'priority_': 43, 'severity_': 36, 'doacross_': 23, 'match_': 229, 'assumes_': 24, 'message_': 43, 'at_': 46, 'bind_': 34, 'filter_': 43, 'destroy_': 12, 'when_': 1, 'allocator_': 39}}}









