import matplotlib.pyplot as plt

data = {
'3': {'task': 4947, 'collapse': 1268, 'taskwait': 802, 'atomic_read': 621, 'atomic_write': 634, 'atomic_update': 375, 'atomic_capture': 1152, 'task_final': 46, 'task_mergeable': 9, 'schedule(auto)': 39, 'taskyield': 39}, 
'4': {'simd': 10268, 'task_depend': 479, 'target': 10459, 'proc_bind': 259, 'taskgroup': 627, 'cancel': 2070}, 
'4.5': {'target_private': 1233, 'target_firstprivate': 463, 'simdlen': 1146, 'task_priority': 47, 'linear': 1250, 'target_defaultmap': 112, 'target_is_device_ptr': 96, 'target_nowait': 157, 'target_depend': 86}, 
'5': {'loop': 26954, 'taskloop': 1903, 'mutexinoutset': 26, 'requires': 258, 'scan': 1517, 'task_in_reduction': 121, 'atomic_hint': 48, 'inscan': 175, 'order(concurrent)': 138, 'taskwait_depend': 24, 'task_detach': 42, 'task_affinity': 158}}

versions = list(data.keys())

num_subplots = len(versions)
fig, axs = plt.subplots(num_subplots, 2, figsize=(10, 2*num_subplots))

threshold = 1000

# Creating subplots for each OpenMP version
for i, version in enumerate(versions):

    clauses = list(data[version].keys())
    values = list(data[version].values())
    
    clauses1, values1 = [], [] 
    for c1, v1 in zip(clauses, values):
        if v1 > threshold:
            clauses1.append(c1)
            values1.append(v1)
    
    axs[i][0].bar(clauses1, values1)
    # axs[i].set_yscale('log')
    axs[i][0].set_xticklabels(clauses1, rotation=-15)

    # axs[i].set_xlabel('Clause')
    axs[i][0].set_ylabel('Frequency')
    axs[i][0].set_title(f'OpenMP Version {version}')

    clauses1, values1 = [], [] 
    for c1, v1 in zip(clauses, values):
        if v1 <= threshold:
            clauses1.append(c1)
            values1.append(v1)
    
    axs[i][1].bar(clauses1, values1)
    # axs[i].set_yscale('log')
    axs[i][1].set_xticklabels(clauses1, rotation=-30)

    # axs[i].set_xlabel('Clause')
    axs[i][1].set_ylabel('Frequency')
    axs[i][1].set_title(f'OpenMP Version {version}')

plt.tight_layout()
# plt.show()
plt.savefig('c_clauses.eps', format='eps')