# times = {2008: {1: 0, 2: 0, 3: 0, 4: 0, 5: 0, 6: 0, 7: 0, 8: 0, 9: 0, 10: 0, 11: 0, 12: 0}, 2009: {1: 0, 2: 0, 3: 0, 4: 0, 5: 0, 6: 0, 7: 0, 8: 0, 9: 0, 10: 0, 11: 0, 12: 0}, 2010: {1: 0, 2: 0, 3: 0, 4: 0, 5: 0, 6: 0, 7: 0, 8: 0, 9: 0, 10: 0, 11: 0, 12: 0}, 2011: {1: 0, 2: 0, 3: 0, 4: 0, 5: 0, 6: 0, 7: 0, 8: 0, 9: 0, 10: 0, 11: 0, 12: 0}, 2012: {1: 0, 2: 0, 3: 0, 4: 0, 5: 0, 6: 0, 7: 0, 8: 0, 9: 0, 10: 0, 11: 0, 12: 0}, 2013: {1: 3, 2: 4, 3: 5, 4: 6, 5: 6, 6: 7, 7: 8, 8: 11, 9: 12, 10: 39, 11: 52, 12: 72}, 2014: {1: 95, 2: 121, 3: 144, 4: 171, 5: 191, 6: 215, 7: 237, 8: 248, 9: 270, 10: 288, 11: 302, 12: 321}, 2015: {1: 343, 2: 364, 3: 417, 4: 471, 5: 514, 6: 543, 7: 575, 8: 602, 9: 631, 10: 666, 11: 695, 12: 728}, 2016: {1: 779, 2: 823, 3: 863, 4: 903, 5: 929, 6: 952, 7: 974, 8: 993, 9: 1019, 10: 1054, 11: 1096, 12: 1116}, 2017: {1: 1146, 2: 1168, 3: 1196, 4: 1222, 5: 1242, 6: 1270, 7: 1291, 8: 1318, 9: 1340, 10: 1369, 11: 1378, 12: 1388}, 2018: {1: 1400, 2: 1414, 3: 1436, 4: 1461, 5: 1479, 6: 1493, 7: 1509, 8: 1526, 9: 1540, 10: 1557, 11: 1575, 12: 1587}, 2019: {1: 1608, 2: 1622, 3: 1640, 4: 1657, 5: 1668, 6: 1682, 7: 1707, 8: 1729, 9: 1740, 10: 1748, 11: 1764, 12: 1778}, 2020: {1: 1797, 2: 1804, 3: 1818, 4: 1838, 5: 1856, 6: 1879, 7: 1900, 8: 1918, 9: 1931, 10: 1951, 11: 1973, 12: 1995}, 2021: {1: 2009, 2: 2029, 3: 2048, 4: 2071, 5: 2093, 6: 2111, 7: 2135, 8: 2159, 9: 2182, 10: 2208, 11: 2236, 12: 2262}, 2022: {1: 2299, 2: 2325, 3: 2361, 4: 2394, 5: 2432, 6: 2465, 7: 2517, 8: 2559, 9: 2604, 10: 2657, 11: 2726, 12: 2788}, 2023: {1: 2919, 2: 2985, 3: 3102, 4: 3222, 5: 3404, 6: 3834, 7: 3881, 8: 3881, 9: 3881, 10: 3881, 11: 3881, 12: 3881}}
# l = []

# for year, months in times.items():
#     for m, val in months.items():
#         l.append((year+(m-1)/12, val))

# with open('aa.txt','w') as f:
#     for a in l:
#         f.write(str(a)+ ' ')
# print(l)




# import pandas as pd

# data = {'CUDA': {2023: 272, 2022: 101, 2017: 52, 2014: 65, 2016: 81, 2021: 54, 2018: 45, 2019: 56, 2020: 38, 2015: 101, 2013: 14}, 'OpenCL': {2023: 213, 2016: 75, 2014: 119, 2015: 133, 2013: 31, 2017: 61, 2022: 109, 2018: 45, 2021: 59, 2020: 59, 2019: 52}, 'OpenACC': {2023: 18, 2022: 9, 2015: 7, 2016: 11, 2018: 4, 2014: 2, 2019: 3, 2017: 4, 2020: 2, 2021: 1}, 'SYCL': {2021: 3, 2023: 15, 2022: 2, 2017: 1, 2020: 1, 2015: 1, 2016: 1}, 'TBB': {2023: 109, 2017: 28, 2015: 50, 2022: 39, 2014: 27, 2019: 33, 2018: 17, 2020: 19, 2016: 27, 2021: 12, 2013: 13}, 'Cilk': {2023: 19, 2016: 11, 2015: 13, 2017: 5, 2013: 2, 2014: 15, 2018: 8, 2022: 10, 2021: 8, 2020: 3, 2019: 3}, 'OpenMP': {2023: 1093, 2016: 388, 2020: 217, 2022: 526, 2021: 267, 2015: 407, 2019: 191, 2014: 249, 2018: 199, 2013: 72, 2017: 272}, 'MPI': {2023: 486, 2021: 148, 2022: 236, 2015: 281, 2019: 114, 2018: 101, 2020: 101, 2017: 153, 2014: 194, 2016: 244, 2013: 55, 2012: 2}}
# df = pd.DataFrame(data)
# df.to_csv('data.csv', index_label='Year')







# total = 0
# t ={2015: {2: 60, 10: 102, 12: 87, 8: 88, 6: 87, 5: 94, 11: 95, 1: 65, 7: 79, 3: 108, 9: 99, 4: 86}, 2016: {12: 52, 10: 64, 4: 82, 2: 59, 1: 80, 3: 75, 7: 48, 9: 60, 8: 50, 5: 77, 11: 67, 6: 58}, 2010: {4: 2, 8: 3, 7: 5, 11: 4, 9: 10, 12: 4, 6: 6, 5: 3, 10: 3, 3: 3, 1: 4, 2: 4}, 2014: {5: 50, 7: 57, 4: 58, 8: 60, 2: 58, 11: 61, 10: 76, 1: 53, 9: 86, 3: 58, 12: 65, 6: 61}, 2013: {11: 53, 5: 34, 1: 20, 4: 18, 10: 50, 7: 35, 3: 27, 6: 27, 12: 42, 9: 47, 8: 31, 2: 18}, 2011: {1: 7, 7: 13, 8: 5, 10: 8, 12: 6, 5: 10, 11: 7, 9: 7, 3: 5, 6: 10, 4: 3, 2: 2}, 2017: {2: 67, 4: 56, 8: 50, 10: 32, 1: 58, 9: 40, 6: 45, 3: 69, 7: 49, 5: 56, 11: 2, 12: 1}, 2012: {5: 12, 10: 15, 6: 23, 11: 16, 8: 13, 7: 17, 9: 22, 12: 11, 2: 16, 4: 21, 3: 22, 1: 5}, 2008: {11: 2, 12: 1, 10: 2, 8: 1}, 2018: {2: 2, 4: 3, 6: 2, 10: 2, 7: 2, 12: 1, 3: 1, 9: 1, 8: 1}, 2019: {7: 1, 3: 1, 10: 2, 12: 2, 1: 1, 2: 3, 8: 2, 6: 2}, 2009: {8: 1, 11: 2, 5: 2, 12: 3, 9: 2, 6: 1, 10: 1, 1: 1}, 2021: {1: 1, 3: 1, 4: 1}, 2020: {2: 2, 11: 2, 1: 2, 4: 1, 3: 1, 12: 1}, 2022: {3: 1, 12: 1}}
# for year, months in t.items():
#     total += sum(months.values())

# print(total)




# result = {}
# di = {2023: {'ver4.0': 305673, 'ver2.5': 281990, 'ver3.1': 5646, 'ver4.5': 32471, 'ver5.0': 60423, 'ver3.0': 7217, 'ver5.2': 19251, 'ver5.1': 814}, 2014: {'ver4.0': 192, 'ver2.5': 12143, 'ver3.1': 12, 'ver4.5': 48, 'ver5.0': 2, 'ver3.0': 214, 'ver5.2': 55, 'ver5.1': 0}, 2021: {'ver4.0': 2093, 'ver2.5': 26843, 'ver3.1': 248, 'ver4.5': 375, 'ver5.0': 17, 'ver3.0': 163, 'ver5.2': 982, 'ver5.1': 0}, 2022: {'ver4.0': 24374, 'ver2.5': 72646, 'ver3.1': 4319, 'ver4.5': 4655, 'ver5.0': 4420, 'ver3.0': 1575, 'ver5.2': 2706, 'ver5.1': 0}, 2015: {'ver4.0': 1288, 'ver2.5': 44075, 'ver3.1': 450, 'ver4.5': 549, 'ver5.0': 80, 'ver3.0': 662, 'ver5.2': 426, 'ver5.1': 0}, 2016: {'ver4.0': 3953, 'ver2.5': 39625, 'ver3.1': 619, 'ver4.5': 997, 'ver5.0': 47, 'ver3.0': 1126, 'ver5.2': 697, 'ver5.1': 0}, 2020: {'ver4.0': 540, 'ver2.5': 16681, 'ver3.1': 77, 'ver4.5': 133, 'ver5.0': 2, 'ver3.0': 326, 'ver5.2': 464, 'ver5.1': 0}, 2017: {'ver4.0': 59524, 'ver2.5': 42243, 'ver3.1': 603, 'ver4.5': 2641, 'ver5.0': 6530, 'ver3.0': 929, 'ver5.2': 2522, 'ver5.1': 0}, 2018: {'ver4.0': 139, 'ver2.5': 15217, 'ver3.1': 93, 'ver4.5': 80, 'ver5.0': 1, 'ver3.0': 28, 'ver5.2': 22, 'ver5.1': 0}, 2013: {'ver4.0': 0, 'ver2.5': 2508, 'ver3.1': 0, 'ver4.5': 18, 'ver5.0': 1, 'ver3.0': 44, 'ver5.2': 1, 'ver5.1': 0}, 2019: {'ver4.0': 736, 'ver2.5': 22814, 'ver3.1': 323, 'ver4.5': 290, 'ver5.0': 26, 'ver3.0': 88, 'ver5.2': 275, 'ver5.1': 0}, 2012: {'ver4.0': 0, 'ver2.5': 0, 'ver3.1': 0, 'ver4.5': 0, 'ver5.0': 0, 'ver3.0': 0, 'ver5.2': 0, 'ver5.1': 0}}
# vers = ['ver2', 'ver3', 'ver4', 'ver5']
# for year, versions in di.items():
#     result[year] = {k:0 for k in vers}

#     for k in vers:
#         for ver in versions.keys():

#             if ver.startswith(k):
#                 result[year][k] += di[year][ver]

# print(result)






''' Figure 11 '''

clauses_per_version = {
    'ver2.5':['schedule_guided', 'private_', 'copyin_', 'section_', 'schedule_dynamic', 'ordered_', 'atomic_', 'reduction_', 'shared_', 'critical_', 'lastprivate_', 'threadprivate_', 'nowait_', 'schedule_runtime', 'sections_', 'master_', 'firstprivate_', 'barrier_', 'for_', 'default_', 'schedule_static', 'single_', 'parallel_', 'parallel_workshare_', 'num_threads_', 'flush_', 'parallel_sections_', 'copyprivate_', 'parallel_for_', 'schedule_', 'workshare_'],
    'ver3.0': ['atomic_capture_', 'schedule_auto', 'task_', 'taskwait_', 'atomic_read_', 'atomic_update_', 'atomic_write_'],
    'ver3.1':['collapse_', 'taskyield_'],
    'ver4.0': ['declare_simd_', 'teams_distribute_simd_', 'cancel_', 'simd_', 'taskgroup_', 'declare_target_', 'target_teams_distribute_', 'target_update_', 'proc_bind_', 'simdlen_', 'distribute_simd_', 'cancellation_', 'declare_reduction_', 'target_', 'target_teams_', 'teams_', 'distribute_', 'target_data_', 'map_', 'teams_distribute_', 'target_teams_distribute_simd_'],
    'ver4.5': ['linear_', 'taskloop_simd_', 'target_exit_data_', 'if_', 'target_enter_data_', 'defaultmap_', 'depend_', 'taskloop_'],
    'ver5.0': ['allocate_', 'declare_mapper_', 'task_reduction_', 'taskloop_simd_', 'target_parallel_loop_', 'parallel_target_', 'master_taskloop_', 'requires_', 'declare_variant_', 'parallel_master_taskloop_', 'parallel_master_', 'in_reduction_', 'target_simd_', 'teams_loop_', 'parallel_master_taskloop_simd_', 'copyprivate_', 'scan_', 'loop_', 'target_teams_loop_', 'master_taskloop_simd_', 'parallel_loop_'],
    'ver5.1': ['masked_', 'unroll_', 'masked_taskloop_simd_', 'nothing_', 'error_', 'assume_', 'parallel_masked_taskloop_simd_', 'scope_', 'tile_', 'parallel_masked_', 'masked_taskloop_'],
    'ver5.2': ['mergeable_', 'to_', 'thread_limit_', 'enter_', 'safelen_', 'uniform_', 'use_device_addr_', 'doacross_', 'final_', 'match_', 'is_device_ptr_', 'from_', 'assumes_', 'untied_', 'device_', 'aligned_', 'grainsize_', 'exclusive_', 'allocator_', 'align_', 'dist_schedule_', 'at_', 'nontemporal_', 'filter_', 'initializer_', 'destroy_', 'num_tasks_', 'nogroup_', 'priority_', 'inclusive_']
    }

C = {'ver2.5': {'shared_': 9126, 'critical_': 4755, 'lastprivate_': 1804, 'parallel_': 33969, 'private_': 11106, 'firstprivate_': 2275, 'for_': 29389, 'flush_': 589, 'default_': 7050, 'parallel_for_': 22154, 'num_threads_': 2285, 'reduction_': 3390, 'schedule_static': 8705, 'schedule_': 11719, 'single_': 1019, 'master_': 1244, 'ordered_': 924, 'barrier_': 2825, 'nowait_': 1088, 'schedule_dynamic': 2362, 'threadprivate_': 328, 'section_': 1395, 'sections_': 1087, 'atomic_': 3601, 'parallel_sections_': 400, 'schedule_guided': 340, 'copyin_': 187, 'copyprivate_': 16, 'schedule_runtime': 162}, 
'ver3.0': {'taskwait_': 640, 'task_': 1903, 'atomic_read_': 330, 'atomic_update_': 195, 'atomic_write_': 543, 'schedule_auto': 54, 'atomic_capture_': 424},
'ver3.1': {'collapse_': 2861, 'taskyield_': 29}, 
'ver4.0': {'target_': 6199, 'map_': 1647, 'declare_target_': 296, 'simd_': 9712, 'declare_simd_': 1064, 'cancel_': 633, 'teams_distribute_simd_': 714, 'distribute_simd_': 2095, 'proc_bind_': 397, 'target_teams_distribute_': 1162, 'target_update_': 218, 'teams_': 3112, 'cancellation_': 602, 'target_teams_': 1313, 'target_data_': 522, 'distribute_': 3145, 'target_teams_distribute_simd_': 582, 'simdlen_': 828, 'teams_distribute_': 1547, 'taskgroup_': 481, 'declare_reduction_': 292}, 
'ver4.5': {'if_': 1829, 'taskloop_simd_': 563, 'target_enter_data_': 147, 'defaultmap_': 181, 'linear_': 973, 'target_exit_data_': 135, 'taskloop_': 1251, 'depend_': 817}, 
'ver5.0': {'target_simd_': 1501, 'taskloop_simd_': 563, 'parallel_target_': 1283, 'copyprivate_': 16, 'allocate_': 362, 'task_reduction_': 68, 'in_reduction_': 166, 'loop_': 195, 'parallel_master_': 203, 'parallel_loop_': 28, 'declare_variant_': 619, 'declare_mapper_': 51, 'master_taskloop_': 481, 'requires_': 70, 'parallel_master_taskloop_': 162, 'master_taskloop_simd_': 203, 'parallel_master_taskloop_simd_': 24, 'scan_': 140, 'teams_loop_': 21, 'target_parallel_loop_': 10, 'target_teams_loop_': 8}, 
'ver5.1': {'masked_': 34, 'parallel_masked_taskloop_simd_': 3, 'masked_taskloop_simd_': 6, 'masked_taskloop_': 14, 'scope_': 35, 'error_': 68, 'assume_': 26, 'parallel_masked_': 13, 'nothing_': 21, 'unroll_': 9, 'tile_': 1},
'ver5.2': {'mergeable_': 62, 'device_': 480, 'aligned_': 1956, 'to_': 192, 'grainsize_': 79, 'final_': 121, 'thread_limit_': 293, 'uniform_': 639, 'num_tasks_': 69, 'nogroup_': 80, 'safelen_': 918, 'dist_schedule_': 271, 'enter_': 151, 'is_device_ptr_': 240, 'untied_': 431, 'priority_': 102, 'use_device_ptr_': 10, 'initializer_': 132, 'from_': 107, 'message_': 31, 'match_': 542, 'device_type_': 22, 'hint_': 59, 'nontemporal_': 424, 'begin_declare_variant_': 39, 'bind_': 99, 'severity_': 31, 'use_device_addr_': 30, 'doacross_': 85, 'link_': 14, 'assumes_': 116, 'exclusive_': 69, 'allocator_': 14, 'align_': 13, 'at_': 33, 'filter_': 27, 'destroy_': 20, 'begin_declare_target_': 3, 'begin_assumes_': 38, 'inclusive_': 70, 'when_': 22, 'metadirective_': 22, 'partial_': 7, 'append_args_': 2, 'full_': 1, 'sizes_': 1}}


CPP = {'ver2.5': {'parallel_': 113947, 'for_': 89397, 'parallel_for_': 71275, 'critical_': 8657, 'section_': 2528, 'sections_': 7038, 'parallel_sections_': 3384, 'schedule_static': 4897, 'single_': 3460, 'master_': 7732, 'schedule_': 11203, 'reduction_': 19227, 'private_': 14000, 'num_threads_': 4514, 'default_': 7642, 'ordered_': 2567, 'schedule_dynamic': 3772, 'shared_': 7908, 'flush_': 1355, 'firstprivate_': 9639, 'atomic_': 5360, 'barrier_': 2728, 'schedule_guided': 1498, 'nowait_': 2238, 'lastprivate_': 7557, 'threadprivate_': 3158, 'schedule_runtime': 377, 'copyprivate_': 239, 'copyin_': 489}, 
'ver3.0': {'taskwait_': 1012, 'task_': 4169, 'atomic_update_': 546, 'atomic_capture_': 1491, 'schedule_auto': 240, 'atomic_read_': 242, 'atomic_write_': 288}, 
'ver3.1': {'collapse_': 5176, 'taskyield_': 705}, 
'ver4.0': {'simd_': 54468, 'cancel_': 492, 'teams_': 47679, 'target_': 78930, 'target_teams_': 21906, 'target_teams_distribute_': 18067, 'distribute_': 43111, 'map_': 14411, 'teams_distribute_': 31217, 'declare_simd_': 436, 'simdlen_': 1225, 'declare_reduction_': 585, 'teams_distribute_simd_': 16639, 'distribute_simd_': 24021, 'declare_target_': 939, 'target_update_': 2581, 'target_teams_distribute_simd_': 9488, 'target_data_': 4717, 'taskgroup_': 1113, 'proc_bind_': 698, 'cancellation_': 216}, 
'ver4.5': {'if_': 8185, 'depend_': 4851, 'linear_': 4508, 'taskloop_simd_': 5777, 'target_enter_data_': 1282, 'defaultmap_': 1534, 'target_exit_data_': 1359, 'taskloop_': 11074}, 
'ver5.0': {'copyprivate_': 239, 'parallel_target_': 23052, 'allocate_': 1312, 'declare_mapper_': 136, 'task_reduction_': 840, 'taskloop_simd_': 5777, 'master_taskloop_': 4841, 'master_taskloop_simd_': 2559, 'declare_variant_': 1038, 'parallel_master_taskloop_': 2270, 'in_reduction_': 1187, 'target_simd_': 18034, 'parallel_master_': 3118, 'parallel_master_taskloop_simd_': 1195, 'requires_': 115, 'scan_': 255, 'loop_': 25}, 
'ver5.1': {'dispatch_': 40, 'interop_': 72, 'unroll_': 45, 'tile_': 44, 'masked_': 29}, 
'ver5.2': {'message_': 3, 'uniform_': 47, 'untied_': 194, 'mergeable_': 98, 'safelen_': 2307, 'device_type_': 51, 'to_': 1920, 'grainsize_': 346, 'match_': 935, 'link_': 69, 'final_': 390, 'thread_limit_': 1000, 'enter_': 1282, 'num_tasks_': 381, 'nogroup_': 152, 'aligned_': 3274, 'from_': 867, 'priority_': 476, 'initializer_': 237, 'device_': 1050, 'use_device_ptr_': 247, 'dist_schedule_': 1064, 'is_device_ptr_': 2328, 'exclusive_': 42, 'allocator_': 177, 'nontemporal_': 108, 'destroy_': 39, 'hint_': 81, 'init_': 38, 'inclusive_': 67, 'begin_declare_variant_': 21, 'use_device_addr_': 47, 'begin_assumes_': 22, 'assumes_': 56, 'sizes_': 39, 'metadirective_': 26, 'novariants_': 6, 'partial_': 24, 'append_args_': 35, 'when_': 25, 'adjust_args_': 11, 'full_': 9, 'use_': 17, 'align_': 26, 'at_': 1, 'bind_': 22, 'indirect_': 10, 'filter_': 16, 'begin_declare_target_': 6, 'nocontext_': 6}}


FORTRAN = {'ver2.5': {'parallel_': 13871, 'default_': 4659, 'private_': 12675, 'shared_': 3234, 'do_': 17251, 'parallel_do_': 7839, 'critical_': 1066, 'schedule_dynamic': 805, 'schedule_': 3258, 'threadprivate_': 758, 'atomic_': 2005, 'master_': 556, 'schedule_static': 2013, 'reduction_': 2624, 'sections_': 387, 'section_': 579, 'barrier_': 858, 'single_': 533, 'firstprivate_': 1466, 'parallel_sections_': 129, 'schedule_guided': 285, 'copyin_': 98, 'ordered_': 468, 'lastprivate_': 473, 'nowait_': 12, 'schedule_runtime': 131, 'parallel_workshare_': 258, 'num_threads_': 842, 'flush_': 205, 'workshare_': 574, 'copyprivate_': 1}, 
'ver3.0': {'atomic_update_': 193, 'atomic_capture_': 177, 'schedule_auto': 27, 'task_': 663, 'taskwait_': 312, 'atomic_read_': 241, 'atomic_write_': 260}, 
'ver3.1': {'collapse_': 4781, 'taskyield_': 11}, 
'ver4.0': {'declare_reduction_': 186, 'teams_distribute_simd_': 160, 'distribute_simd_': 236, 'simd_': 1129, 'proc_bind_': 79, 'declare_target_': 329, 'teams_': 656, 'target_update_': 412, 'simdlen_': 89, 'cancel_': 270, 'cancellation_': 198, 'target_': 2532, 'target_teams_': 355, 'target_teams_distribute_': 230, 'distribute_': 562, 'target_teams_distribute_simd_': 103, 'target_data_': 553, 'declare_simd_': 132, 'map_': 991, 'teams_distribute_': 337, 'taskgroup_': 232}, 
'ver4.5': {'taskloop_simd_': 124, 'if_': 626, 'target_enter_data_': 94, 'defaultmap_': 148, 'linear_': 305, 'target_exit_data_': 55, 'taskloop_': 307}, 
'ver5.0': {'in_reduction_': 56, 'taskloop_simd_': 124, 'task_reduction_': 37, 'parallel_target_': 260, 'requires_': 79, 'scan_': 53, 'target_simd_': 137, 'loop_': 115, 'parallel_loop_': 28, 'allocate_': 187, 'parallel_master_taskloop_simd_': 18, 'declare_variant_': 237, 'target_teams_loop_': 9, 'parallel_master_taskloop_': 31, 'teams_loop_': 19, 'target_parallel_loop_': 9, 'master_taskloop_': 65, 'parallel_master_': 43, 'master_taskloop_simd_': 38, 'copyprivate_': 1}, 
'ver5.1': {'masked_': 93, 'parallel_masked_taskloop_simd_': 11, 'masked_taskloop_simd_': 24, 'masked_taskloop_': 40, 'scope_': 55, 'error_': 83, 'assume_': 12, 'parallel_masked_': 31, 'nothing_': 5}, 
'ver5.2': {'use_': 217, 'to_': 303, 'thread_limit_': 143, 'enter_': 102, 'aligned_': 86, 'uniform_': 37, 'use_device_addr_': 154, 'final_': 50, 'link_': 78, 'is_device_ptr_': 28, 'from_': 217, 'untied_': 58, 'use_device_ptr_': 42, 'device_': 271, 'safelen_': 95, 'device_type_': 66, 'grainsize_': 62, 'exclusive_': 20, 'dist_schedule_': 127, 'nontemporal_': 51, 'initializer_': 131, 'num_tasks_': 44, 'nogroup_': 64, 'hint_': 70, 'inclusive_': 31, 'init_': 2, 'mergeable_': 39, 'priority_': 43, 'severity_': 36, 'doacross_': 23, 'match_': 229, 'assumes_': 24, 'message_': 43, 'at_': 46, 'bind_': 34, 'filter_': 43, 'destroy_': 12, 'when_': 1, 'allocator_': 39}}


ver = 'ver5.2'
str = ''
count = 0
lang = FORTRAN
# for clause in clauses_per_version[ver]:
#     if 'schedule' in clause:
#         continue
#     str += '{' + clause.replace('_', ' ').strip() + '}, '

# print(str)


for idx, clause in enumerate(clauses_per_version[ver]):
    if 'schedule' in clause:
        count += 1
        continue
    num = 0 if clause not in lang[ver] else lang[ver][clause]
    str += '(' + '{}'.format(idx+1-count) + ', ' + '{}'.format(num) + ') '

print(str)