import json

count_paradims = {'CUDA': 0, 'OpenCL': 0, 'OpenACC': 0, 'SYCL': 0, 
                                          'TBB': 0, 'Cilk': 0, 'OpenMP': 0, 'MPI': 0}

with open('Fortran_paradigms.json', 'r') as f:
    paradigm_per_repo = json.loads(f.read())
    print(len(paradigm_per_repo.keys()))
#     paradigm_per_repo = {'c': {'SYCL': 0, 'TBB': 15, 'CUDA': 498, 'OpenMP': 2012, 'Cilk': 87, 'OpenCL': 6330, 'OpenACC': 84, 'MPI': 1654},
#                          'cpp': {'SYCL': 20, 'TBB': 444, 'CUDA': 963, 'OpenMP': 3652, 'Cilk': 67, 'OpenCL': 2549, 'OpenACC': 20, 'MPI': 1546},
#                          'fortran':  {'SYCL': 0, 'TBB': 0, 'CUDA': 4, 'OpenMP': 467, 'Cilk': 0, 'OpenCL': 2, 'OpenACC': 0, 'MPI': 246}
# }

    for paradigms in paradigm_per_repo.values():
        for paradigm, val in paradigms.items():
            if val:
                count_paradims[paradigm] += 1

    # for paradigms in paradigm_per_repo.values():
    #     for paradigm, val in paradigms.items():
    #         count_paradims[paradigm] += val

    print(count_paradims)



# count_versions = {'total_loop': 0, 'vers': {'2': {}, '3':{}, '4':{}, '4.5':{}, '5':{}} }


# with open('c_versions.json', 'r') as f:
#     # version_per_repo = json.loads(f.read())
#     version_per_repo = {'c': {'vers': {'4.5': {u'target_defaultmap': 181, u'target': 6276, u'simdlen': 946, u'target_depend': 143, u'task_priority': 102, u'taskloop': 1276, u'target_firstprivate': 470, u'target_private': 1084, u'target_nowait': 219, u'linear': 1075}, '3': {u'task': 4506, u'atomic_read': 337, u'taskwait': 642, u'taskyield': 30, u'task_mergeable': 80, u'atomic_write': 552, u'atomic_update': 207, u'task_final': 122, u'schedule(auto)': 41, u'atomic_capture': 431}, '2': {u' private': 11011, u'lastprivate': 1990, u'schedule_static': 8841, u'for': 29449, u'barrier': 2835, u'num_threads': 2323, u'parallel_for': 29449, u'section': 2503, u'single': 1034, u'schedule_dynamic': 2668, u'reduction': 3696, u'firstprivate': 5037, u'master': 1314, u'critical': 4758, u'flush': 600, u'shared': 10599, u' if': 1784, u'collapse': 3165}, '5': {u'taskwait_depend': 13, u'scan': 329, u'task_detach': 32, u'order(concurrent)': 270, u'task_affinity': 44, u'task_in_reduction': 126, u'atomic_hint': 35, u'inscan': 147, u'mutexinoutset': 13, u'loop': 1538}, '4': {u'cancel': 1241, u'proc_bind': 397, u'task_depend': 399, u'taskgroup': 494, u'simd': 9942}}, 'total_loop': 21822609},
#                             'cpp': {'vers': {'4.5': {u'target_defaultmap': 1688, u'linear': 4648, u'simdlen': 1229, u'target_depend': 4129, u'task_priority': 476, u'taskloop': 11074, u'target_firstprivate': 2612, u'target_private': 6748, u'target_nowait': 716, u'target': 79126}, '3': {u'task': 18272, u'atomic_read': 242, u'taskwait': 1012, u'task_mergeable': 98, u'taskyield': 705, u'atomic_update': 552, u'task_final': 431, u'schedule(auto)': 208, u'atomic_capture': 1497, u'atomic_write': 288}, '2': {u' private': 14615, u'lastprivate': 8524, u'schedule_static': 5689, u'for': 89489, u'barrier': 2728, u'num_threads': 4588, u'parallel_for': 89489, u'section': 9654, u'reduction': 23205, u'schedule_dynamic': 3842, u'single': 3473, u'firstprivate': 10423, u'master': 7871, u'critical': 8657, u'flush': 1398, u'shared': 10957, u' if': 8724, u'collapse': 5176}, '5': {u'taskwait_depend': 37, u'scan': 341, u'task_detach': 52, u'order(concurrent)': 50, u'task_affinity': 96, u'task_in_reduction': 1331, u'atomic_hint': 25, u'inscan': 77, u'mutexinoutset': 12, u'loop': 11139}, '4': {u'cancel': 988, u'proc_bind': 798, u'task_depend': 455, u'taskgroup': 1113, u'simd': 54557}}, 'total_loop': 19841237},
#                             'fortran':  {'vers': {'4.5': {u'target_defaultmap': 148, u'linear': 316, u'simdlen': 96, u'task_priority': 43, u'taskloop': 311, u'target_firstprivate': 70, u'target_private': 147, u'target_nowait': 11, u'target': 2561}, '3': {u'atomic_write': 263, u'atomic_read': 248, u'taskwait': 312, u'task': 1656, u'task_mergeable': 39, u'taskyield': 11, u'atomic_update': 209, u'task_final': 50, u'schedule(auto)': 15, u'atomic_capture': 196}, '2': {u' private': 12466, u'lastprivate': 473, u'schedule_static': 2084, u'barrier': 859, u'for': 17283, u'num_threads': 1009, u'firstprivate': 1477, u'parallel_for': 7845, u'section': 977, u'single': 549, u'schedule_dynamic': 806, u'reduction': 2692, u'critical': 1067, u'master': 579, u'flush': 205, u'shared': 5587, u' if': 667, u'collapse': 4781}, '5': {u'scan': 128, u'task_detach': 38, u'order(concurrent)': 120, u'task_affinity': 38, u'task_in_reduction': 50, u'atomic_hint': 50, u'inscan': 75, u'mutexinoutset': 2, u'loop': 436}, '4': {u'cancel': 469, u'proc_bind': 79, u'taskgroup': 233, u'simd': 1199}}, 'total_loop': 1203773}
#     }

#     for versions in version_per_repo.values():

#         count_versions['total_loop'] += versions['total_loop']

#         for k in ['2', '3', '4', '4.5', '5']:
#             for clause, amount in versions['vers'][k].items():
#                 count_versions['vers'][k][clause] = amount if clause not in count_versions['vers'][k] else \
#                                                             count_versions['vers'][k][clause]+amount
                
#     print(count_versions)


# c
# {'SYCL': 0, 'TBB': 15, 'CUDA': 498, 'OpenMP': 2012, 'Cilk': 87, 'OpenCL': 6330, 'OpenACC': 84, 'MPI': 1654}

# {'vers': 
# '2': {u' private': 11011, u'lastprivate': 1990, u'schedule_static': 8841, u'for': 29449, u'barrier': 2835, u'num_threads': 2323, u'parallel_for': 29449, u'section': 2503, u'single': 1034, u'schedule_dynamic': 2668, u'reduction': 3696, u'firstprivate': 5037, u'master': 1314, u'critical': 4758, u'flush': 600, u'shared': 10599, u' if': 1784, u'collapse': 3165}, 
# '3': {u'task': 4506, u'atomic_read': 337, u'taskwait': 642, u'taskyield': 30, u'task_mergeable': 80, u'atomic_write': 552, u'atomic_update': 207, u'task_final': 122, u'schedule(auto)': 41, u'atomic_capture': 431}, 
# '4': {u'cancel': 1241, u'proc_bind': 397, u'task_depend': 399, u'taskgroup': 494, u'simd': 9942}}, 'total_loop': 21822609}
# {'4.5': {u'target_defaultmap': 181, u'target': 6276, u'simdlen': 946, u'target_depend': 143, u'task_priority': 102, u'taskloop': 1276, u'target_firstprivate': 470, u'target_private': 1084, u'target_nowait': 219, u'linear': 1075}, 
# '5': {u'taskwait_depend': 13, u'scan': 329, u'task_detach': 32, u'order(concurrent)': 270, u'task_affinity': 44, u'task_in_reduction': 126, u'atomic_hint': 35, u'inscan': 147, u'mutexinoutset': 13, u'loop': 1538}, 

# cpp
# {'SYCL': 20, 'TBB': 444, 'CUDA': 963, 'OpenMP': 3652, 'Cilk': 67, 'OpenCL': 2549, 'OpenACC': 20, 'MPI': 1546}

# {'vers': 
# '2': {u' private': 14615, u'lastprivate': 8524, u'schedule_static': 5689, u'for': 89489, u'barrier': 2728, u'num_threads': 4588, u'parallel_for': 89489, u'section': 9654, u'reduction': 23205, u'schedule_dynamic': 3842, u'single': 3473, u'firstprivate': 10423, u'master': 7871, u'critical': 8657, u'flush': 1398, u'shared': 10957, u' if': 8724, u'collapse': 5176}, 
# '3': {u'task': 18272, u'atomic_read': 242, u'taskwait': 1012, u'task_mergeable': 98, u'taskyield': 705, u'atomic_update': 552, u'task_final': 431, u'schedule(auto)': 208, u'atomic_capture': 1497, u'atomic_write': 288}, 
# {'4.5': {u'target_defaultmap': 1688, u'linear': 4648, u'simdlen': 1229, u'target_depend': 4129, u'task_priority': 476, u'taskloop': 11074, u'target_firstprivate': 2612, u'target_private': 6748, u'target_nowait': 716, u'target': 79126}, 
# '4': {u'cancel': 988, u'proc_bind': 798, u'task_depend': 455, u'taskgroup': 1113, u'simd': 54557}}, 'total_loop': 19841237}
# '5': {u'taskwait_depend': 37, u'scan': 341, u'task_detach': 52, u'order(concurrent)': 50, u'task_affinity': 96, u'task_in_reduction': 1331, u'atomic_hint': 25, u'inscan': 77, u'mutexinoutset': 12, u'loop': 11139}, 


# Fortran
# {'SYCL': 0, 'TBB': 0, 'CUDA': 4, 'OpenMP': 467, 'Cilk': 0, 'OpenCL': 2, 'OpenACC': 0, 'MPI': 246}

# {'vers': 
# '2': {u' private': 12466, u'lastprivate': 473, u'schedule_static': 2084, u'barrier': 859, u'for': 17283, u'num_threads': 1009, u'firstprivate': 1477, u'parallel_for': 7845, u'section': 977, u'single': 549, u'schedule_dynamic': 806, u'reduction': 2692, u'critical': 1067, u'master': 579, u'flush': 205, u'shared': 5587, u' if': 667, u'collapse': 4781}, 
# '3': {u'atomic_write': 263, u'atomic_read': 248, u'taskwait': 312, u'task': 1656, u'task_mergeable': 39, u'taskyield': 11, u'atomic_update': 209, u'task_final': 50, u'schedule(auto)': 15, u'atomic_capture': 196}, 
# '4': {u'cancel': 469, u'proc_bind': 79, u'taskgroup': 233, u'simd': 1199}}, 'total_loop': 1203773}
# {'4.5': {u'target_defaultmap': 148, u'linear': 316, u'simdlen': 96, u'task_priority': 43, u'taskloop': 311, u'target_firstprivate': 70, u'target_private': 147, u'target_nowait': 11, u'target': 2561}, 
# '5': {u'scan': 128, u'task_detach': 38, u'order(concurrent)': 120, u'task_affinity': 38, u'task_in_reduction': 50, u'atomic_hint': 50, u'inscan': 75, u'mutexinoutset': 2, u'loop': 436}, 



# sum 
# {'SYCL': 20, 'TBB': 459, 'CUDA': 1465, 'OpenMP': 6131, 'Cilk': 154, 'OpenCL': 8881, 'OpenACC': 104, 'MPI': 3446}


# Amount repositories
# c 144522
# cpp 150481
# Fortran  3683