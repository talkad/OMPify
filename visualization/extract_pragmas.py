import os
import json


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
    versions = [2, 3, 4, 4.5, 5]
    version_amount = {version:0 for version in versions}
    
    with open(json_path, 'r') as f:
        database = json.load(f)
        
    for pragmas in database.values():
        for pragma in pragmas:
            version = get_omp_version(pragma)
            version_amount[version] += 1

    return version_amount

# scan_repos('/home/talkad/Downloads/thesis/data_gathering_script/repositories_openMP')
print(scan_json('/home/talkad/Downloads/thesis/data_gathering_script/visualization/repo2pragmas.json'))

# versions = {2: 213764, 3: 117565, 4: 163780, 4.5: 3683, 5: 1306}
# [0.4274442209326972, 0.235083923550984, 0.3274958108210791, 0.007364556546916804, 0.0026114881483229285]