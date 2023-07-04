#!/usr/bin/python 


import os
import json
import re


redundant_line_comments_c = re.compile("\/\/.*")
redundant_line_comments_fortran = re.compile("![^\$].*$|^c.*$", re.MULTILINE)
redundant_multiline_comments_c = re.compile("\/\*.*?\*\/", re.MULTILINE|re.DOTALL)


def remove_comments(code, is_fortran):
    '''
    Remove comments from given code

    Parameters:
        code: str - code textual representation
        is_fortran: bool - whether the code is written in the Fortran language

    Result:
        clean code
    '''
    if is_fortran:
        code = redundant_line_comments_fortran.sub("\n", code)
        code = redundant_multiline_comments_c.sub("\n", code)
    else:
        code = redundant_line_comments_c.sub("\n", code)
        code = redundant_multiline_comments_c.sub("\n", code)

    return code


def is_for(line, is_fortran):
    '''
	Return true if the given line is the beginning of a for-loop
      
    Parameters:
        line: str - line of code
        is_fortran: bool - whether the code is written in the Fortran language

    Result:
        true if the given line is the beginning of a for-loop
    '''
    sub_line = line.lstrip() # remove redundant white spaces

    if is_fortran:
        return sub_line.startswith('do ')
    
    return sub_line.startswith('for') and sub_line[3:].lstrip().startswith('(')


def is_omp_pragma(line, is_fortran):
	'''
	Return true if the given line is a for-pragma
      
    Parameters:
        line: str - line of code
        is_fortran: bool - whether the code is written in the Fortran language

    Result:
        true if the given line is a for-pragma
    '''
	sub_line = line.lstrip() # remove redundant white spaces
	if is_fortran:
		return sub_line.startswith('!$') and 'omp' in line and 'end' not in line
	else:
		return sub_line.startswith('#pragma ') and ' omp ' in line



def update_versions(line, versions, is_fortran):
    '''
    Determine the version of OpenMP pragma 

    Parameters:
        code: str - line of code
        versions: dict - amount of each clause (per version)
        is_fortran: bool - whether the code is written in the Fortran language

    Return:
        None (updates the versions dictionary)
    '''
    line = line.lstrip()
    #     version 5
    clauses = [clause for clause in ['requiers','loop', 'order(concurrent)', 'scan', 'inscan',  'mutexinoutset'] if clause in line]
    if 'atomic' in line and 'hint' in line:
        clauses.append('atomic_hint')
    if 'taskwait' in line and 'depend' in line:
        clauses.append('taskwait_depend')
    if 'task' in line:
        clauses += ['task_'+clause for clause in ['detach', 'affinity', 'in_reduction'] if clause in line]

    for clause in clauses:
        versions['5'][clause] = 1 if clause not in versions['5'] else versions['5'][clause]+1

    #     version 4.5
    clauses = [clause for clause in ['linear', 'simdlen', 'target', 'taskloop'] if clause in line]
    if 'task' in line and 'priority' in line:
        clauses.append('task_priority')
    if 'target' in line:
        clauses += ['target_'+clause for clause in ['private', 'nowait', 'depend', 'firstprivate', 'defaultmap'] if clause in line]
    
    for clause in clauses:
        versions['4.5'][clause] = 1 if clause not in versions['4.5'] else versions['4.5'][clause]+1

    #     version 4
    clauses = [clause for clause in ['cancel', 'taskgroup', 'proc_bind', 'simd'] if clause in line]
    if 'task' in line and 'depend' in line:
        clauses.append('task_depend')

    for clause in clauses:
        versions['4'][clause] = 1 if clause not in versions['4'] else versions['4'][clause]+1

    #     version 3
    clauses = [clause for clause in ['task', 'taskwait', 'schedule(auto)', 'taskyield'] if clause in line]
    if 'task' in line:
        clauses += ['task_'+clause for clause in ['final', 'mergeable'] if clause in line]
    if 'atomic' in line:
        clauses += ['atomic_'+clause for clause in ['read', 'write', 'capture', 'update'] if clause in line]
    
    for clause in clauses:
        versions['3'][clause] = 1 if clause not in versions['3'] else versions['3'][clause]+1

    #     version 2.5
    clauses = [clause for clause in [' private', 'section', 'barrier', 'nowait', 'critical', 'flush', 'single', 'master', 'firstprivate', 'lastprivate', 'shared', 'reduction', ' if', 'num_threads', 'collapse'] if clause in line]
    
    if 'schedule' in line:
        clauses += ['schedule_'+clause for clause in ['static', 'dynamic'] if clause in line]

    if (is_fortran and ' do' in line) or (not is_fortran and ' for' in line):
        clauses += ['for']

    if 'parallel' in line and (is_fortran and ' do' in line) or (not is_fortran and ' for' in line):
        clauses += ['parallel_for']

    for clause in clauses:
        versions['2'][clause] = 1 if clause not in versions['2'] else versions['2'][clause]+1



def get_omp_version(code, is_fortran):
    '''
    Get the clauses versions found in a given code

    Parameters:
        code: str - code textual representation
        versions: dict - amount of each clause (per version)
        is_fortran: bool - whether the code is written in the Fortran language

    Return:
        Amount of for loops and OpenMP version statistics
    '''
    total_loop = 0
    versions = {'2': {}, '3':{}, '4':{}, '4.5':{}, '5':{}}

    for line in code.split('\n'):

        if is_for(line, is_fortran):
            total_loop += 1

        if is_omp_pragma(line, is_fortran):
            update_versions(line, versions, is_fortran)

    return total_loop, versions


paradigms = {
    'CUDA': [r'^\W*#\W*include.*[^a-zA-Z]cuda\.h.*$', r'^.*cudaMalloc\(.*$', r'^.*cudaFree\(.*$', r'^.*cudaMemcpy\(.*$', r'^.*cudaMemset\(.*$'],
    'OpenCL': [r'^\W*#\W*include.*[^a-zA-Z]cl\.h.*$', r'^.*clCreateContext\(.*$', r'^.*clReleaseContext\(.*$', r'^.*clCreateCommandQueue\(.*$', r'^.*clBuildProgram\(.*$', r'^.*clReleaseKernel\(.*$'],
    'OpenACC': [r'^\W*#\W*pragma\W*acc.*$'],
    'SYCL': [r'^\W*#\W*include.*[^a-zA-Z]sycl.hpp.*$', r'^\W*using\W+namespace\W+sycl.*$', r'^.*sycl::.*$'],   #, r'^.*sycl::event.*$', r'^.*sycl::handler.*$', r'^.*sycl::program.*$', r'^.*sycl::queue.*$', r'^.*sycl::buffer.*$', r'^.*sycl::kernel.*$', r'^.*sycl::range*$', r'^.*sycl::accessor.*$'],
    'TBB': [r'^\W*#\W*include.*[^a-zA-Z]tbb\.h.*$', r'^\W*using\W+namespace\W+tbb.*$', r'^.*tbb::.*$'],  #, r'^.*tbb::parallel_for.*$', r'^.*tbb::parallel_reduce.*$', r'^.*tbb::parallel_invoke.*$', r'^.*tbb::parallel_sort.*$', r'^.*tbb::parallel_scan.*$', r'^.*tbb::concurrent_vector.*$', r'^.*tbb::task_group.*$'],
    'Cilk': [r'^\W*#\W*include.*[^a-zA-Z]cilk\.h.*$', r'^\W*using\W+namespace\W+cilk.*$', r'^.*cilk_spawn\(.*$', r'^.*cilk_sync\(.*$', r'^.*cilk_for\(.*$'],
    'OpenMP': [r'^\W*#\W*pragma\W*omp.*$', r'^\W*!\$\W*[omp|OMP].*$'],
    'MPI': [r'^\W*#\W*include.*[^a-zA-Z]mpi\.h.*$', r'^\W*use\W+mpi.*$', r'^.*MPI_Init\(.*$', r'^.*MPI_Finalize\(.*$', r'^.*MPI_Comm_rank\(.*$', r'^.*MPI_Send\(.*$', r'^.*MPI_Recv\(.*$', r'^.*MPI_Alltoall\(.*$', r'^.*MPI_Scatter\(.*$', r'^.*MPI_Gather\(.*$', r'^.*MPI_Reduce\(.*$', r'^.*MPI_Allreduce\(.*$', r'^.*MPI_Bcast\(.*$']
} 



def get_parallel_paradigms(code):
    '''
    Get parallel paradigms found in a given code

    Parameters:
        code: str - code textual representation
    Return:
        list of the parallel paradigm
    '''
    matched_paradigms = []

    for paradigm, patterns in paradigms.items():
        for pattern in patterns:

            if re.search(pattern, code, re.MULTILINE):
                matched_paradigms.append(paradigm)
                break
    
    return matched_paradigms


def concat_lines(code, is_fortran):
    '''
    Concatenate sequential splitted lined

    Parameters:
        code: str - code textual representation
        is_fortran: bool - whether the code is written in the Fortran language

    Result:
        clean code
    '''
    split_token = '&' if is_fortran else '\\' 
    code_buffer = []
    concat = False

    for line in code.split('\n'):
        next_concat = line.rstrip().endswith(split_token)
        if concat:
            if next_concat:
                line = line.rstrip()[:-1] + " "

            code_buffer[-1] = code_buffer[-1].rstrip() + " " + line
        else:            
            if next_concat:
                line = line.rstrip()[:-1] + " "

            code_buffer.append(line)

        concat = next_concat

    return '\n'.join(code_buffer)



def iterate_jsons(dir, vars, is_fortran=False):
    '''
    Iterate over HPCorpus and return stats

    Prameters:
        dir: str - programming langauge directory to be iterated
        vars: dict - environment variables
        is_fortran: bool - whether the code is written in the Fortran language

    Return:
        Save OpenMP versions and parallel paradigms statistics
    '''

    par_paradigms = {}
    omp_versions = {}
    dir_path = os.path.join(vars['HPCorpus_path'], dir)

    for json_file in os.listdir(dir_path):
        with open(os.path.join(dir_path, json_file), 'r') as f:
            for line in f:
                try:
                    js = json.loads(line.strip())
                except:
                    continue

                if 'content' not in js:
                    continue
                
                repo_name = js['repo_name']
                code = js['content']
                code = remove_comments(code, is_fortran)
                code = concat_lines(code, is_fortran)

                # get_omp_version(code, versions, is_fortran)
                ### Parallel Paradigms ###
                paradigms = get_parallel_paradigms(code)

                if repo_name not in par_paradigms:
                    par_paradigms[repo_name] = {'CUDA': False, 'OpenCL': False, 'OpenACC': False, 'SYCL': False, 
                                          'TBB': False, 'Cilk': False, 'OpenMP': False, 'MPI': False}

                for paradigm in paradigms:
                    par_paradigms[repo_name][paradigm] = True
                ### Parallel Paradigms ###


                ### OpenMP versions ###
                total_loop, versions = get_omp_version(code.lower(), is_fortran)

                if repo_name not in omp_versions:
                    omp_versions[repo_name] = {'total_loop': 0, 'vers': {'2': {}, '3':{}, '4':{}, '4.5':{}, '5':{}} }

                omp_versions[repo_name]['total_loop'] += total_loop

                for ver in versions.keys():
                    for clause, amount in versions[ver].items():
                        omp_versions[repo_name]['vers'][ver][clause] = amount if clause not in omp_versions[repo_name]['vers'][ver] else \
                                                                                omp_versions[repo_name]['vers'][ver][clause] + amount
                ### OpenMP versions ###
    
    with open('{}_paradigms.json'.format(dir), 'w') as f:
        f.write(json.dumps(par_paradigms))

    with open('{}_versions.json'.format(dir), 'w') as f:
        f.write(json.dumps(omp_versions))



if __name__ == '__main__':

    with open('/homes/talkad/OMPify/database_creator/visualization/hpcorpus_stats/statistics/ENV.json', 'r') as f:
        vars = json.loads(f.read())

    iterate_jsons('Fortran', vars, is_fortran=True)
