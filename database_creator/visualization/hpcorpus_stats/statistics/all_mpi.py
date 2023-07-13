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


def concat_lines(code, is_fortran):
    '''
    Concatenate sequential splitted lines

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




paradigms = {
    # 'CUDA': [r'^\W*#\W*include.*[^a-zA-Z]cuda\.h.*$', r'^.*cudaMalloc\(.*$', r'^.*cudaFree\(.*$', r'^.*cudaMemcpy\(.*$', r'^.*cudaMemset\(.*$'],
    # 'OpenCL': [r'^\W*#\W*include.*[^a-zA-Z]cl\.h.*$', r'^.*clCreateContext\(.*$', r'^.*clReleaseContext\(.*$', r'^.*clCreateCommandQueue\(.*$', r'^.*clBuildProgram\(.*$', r'^.*clReleaseKernel\(.*$'],
    # 'OpenACC': [r'^\W*#\W*pragma\W*acc.*$'],
    # 'SYCL': [r'^\W*#\W*include.*[^a-zA-Z]sycl.hpp.*$', r'^\W*using\W+namespace\W+sycl.*$', r'^.*sycl::.*$'],   #, r'^.*sycl::event.*$', r'^.*sycl::handler.*$', r'^.*sycl::program.*$', r'^.*sycl::queue.*$', r'^.*sycl::buffer.*$', r'^.*sycl::kernel.*$', r'^.*sycl::range*$', r'^.*sycl::accessor.*$'],
    # 'TBB': [r'^\W*#\W*include.*[^a-zA-Z]tbb\.h.*$', r'^\W*using\W+namespace\W+tbb.*$', r'^.*tbb::.*$'],  #, r'^.*tbb::parallel_for.*$', r'^.*tbb::parallel_reduce.*$', r'^.*tbb::parallel_invoke.*$', r'^.*tbb::parallel_sort.*$', r'^.*tbb::parallel_scan.*$', r'^.*tbb::concurrent_vector.*$', r'^.*tbb::task_group.*$'],
    # 'Cilk': [r'^\W*#\W*include.*[^a-zA-Z]cilk\.h.*$', r'^\W*using\W+namespace\W+cilk.*$', r'^.*cilk_spawn\(.*$', r'^.*cilk_sync\(.*$', r'^.*cilk_for\(.*$'],
    # 'OpenMP': [r'^\W*#\W*pragma\W*omp.*$', r'^\W*!\$\W*[omp|OMP].*$'],
    'MPI': [r'^\W*#\W*include.*[^a-zA-Z]mpi\.h.*$', r'^\W*use\W+mpi.*$', r'^(.* |)MPI_.*?\(.*$'] # ,r'^.*MPI_Init\(.*$', r'^.*MPI_Finalize\(.*$', r'^.*MPI_Comm_rank\(.*$', r'^.*MPI_Send\(.*$', r'^.*MPI_Recv\(.*$', r'^.*MPI_Alltoall\(.*$', r'^.*MPI_Scatter\(.*$', r'^.*MPI_Gather\(.*$', r'^.*MPI_Reduce\(.*$', r'^.*MPI_Allreduce\(.*$', r'^.*MPI_Bcast\(.*$']
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



# clauses_per_version = {
#     'ver2.5': [(['parallel'],[]), (['sections'],[]), (['section'],[]), (['single'],[]), (['workshare'],[]), (['for'],[]), (['do'],[]), (['parallel', 'for'],[]), (['parallel', 'do'],[]), (['parallel', 'sections'],[]), (['parallel', 'workshare'],[]), (['master'],[]), (['critical'],[]), (['barrier'],[]), (['atomic'],[]), (['flush'],[]), (['ordered'],[]), (['threadprivate'],[]), (['default'],[]), (['shared'],[]), (['private'],[]), (['firstprivate'],[]), (['lastprivate'],[]), (['reduction'],[]), (['copyin'],[]), (['copyprivate'],[]), (['num_threads'],[]), (['schedule'],[]), (['schedule'],['static']), (['schedule'],['dynamic']), (['schedule'],['guided']), (['schedule'],['runtime']), (['nowait'],[])],
#     'ver3.0': [(['task'],[]), (['taskwait'],[]), (['schedule'],['auto']), (['atomic', 'read'],[]), (['atomic', 'write'],[]), (['atomic', 'update'],[]), (['atomic', 'capture'],[])],
#     'ver3.1': [(['taskyield'],[]), (['collapse'],[])],
#     'ver4.0': [(['simd'],[]), (['simdlen'],[]), (['declare', 'simd'],[]), (['target'],[]), (['target', 'data'],[]), (['target', 'update'],[]), (['declare', 'target'],[]), (['teams'],[]), (['distribute'],[]), (['distribute', 'simd'],[]), (['target', 'teams'],[]), (['teams', 'distribute'],[]), (['teams', 'distribute', 'simd'],[]), (['target', 'teams', 'distribute'],[]), (['target', 'teams', 'distribute', 'simd'],[]), (['taskgroup'],[]), (['cancel'],[]), (['cancellation'],[]), (['map'],[]), (['declare', 'reduction'],[]), (['proc_bind'],[])],
#     'ver4.5': [(['taskloop'],[]), (['taskloop', 'simd'],[]), (['target', 'enter', 'data'],[]), (['target', 'exit', 'data'],[]), (['depend'],[]), (['linear'],[]), (['defaultmap'],[]), (['if'],[])],
#     'ver5.0': [(['declare', 'variant'],[]), (['requires'],[]), (['distribute', 'loop'],[]), (['loop'],[]), (['scan'],[]), (['allocate'],[]), (['taskloop', 'simd'],[]), (['parallel', 'loop'],[]), (['parallel', 'master'],[]), (['master', 'taskloop'],[]), (['master', 'taskloop', 'simd'],[]), (['parallel', 'master', 'taskloop'],[]), (['parallel', 'master', 'taskloop', 'simd'],[]), (['teams', 'loop'],[]), (['parallel', 'target'],[]), (['target', 'parallel', 'loop'],[]), (['target', 'simd'],[]), (['target', 'teams', 'loop'],[]), (['deobj'],[]), (['task_reduction'],[]), (['in_reduction'],[]), (['copyprivate'],[]), (['declare', 'mapper'],[])],
#     'ver5.1': [(['dispatch'],[]), (['assume'],[]), (['nothing'],[]), (['error'],[]), (['masked'],[]), (['scope'],[]), (['tile'],[]), (['unroll'],[]), (['interop'],[]), (['parallel', 'masked'],[]), (['masked', 'taskloop'],[]), (['masked', 'taskloop', 'simd'],[]), (['parallel', 'masked', 'taskloop', 'simd'],[])],
#     'ver5.2': [(['destroy'],[]), (['is_device_ptr'],[]), (['use_device_ptr'],[]), (['has_device_ptr'],[]), (['use_device_addr'],[]), (['initializer'],[]), (['inclusive'],[]), (['exclusive'],[]), (['enter'],[]), (['link'],[]), (['to'],[]), (['from'],[]), (['uniform'],[]), (['aligned'],[]), (['align'],[]), (['allocator'],[]), (['allocators'],[]), (['use_allocators'],[]), (['when'],[]), (['otherwise'],[]), (['metadirective'],[]), (['begin', 'metadirective'],[]), (['match'],[]), (['adjust_args'],[]), (['append_args'],[]), (['begin', 'declare', 'variant'],[]), (['novariants'],[]), (['nocontext'],[]), (['begin', 'declare', 'target'],[]), (['indirect'],[]), (['at'],[]), (['assumes'],[]), (['begin', 'assumes'],[]), (['severity'],[]), (['message'],[]), (['sizes'],[]), (['full'],[]), (['partial'],[]), (['nontemporal'],[]), (['safelen'],[]), (['filter'],[]), (['dist_schedule'],[]), (['bind'],[]), (['untied'],[]), (['mergeable'],[]), (['final'],[]), (['priority'],[]), (['grainsize'],[]), (['num_tasks'],[]), (['device_type'],[]), (['device'],[]), (['thread_limit'],[]), (['init'],[]), (['use'],[]), (['hint'],[]), (['nogroup'],[]), (['doacross'],[]), (['nogroup'],[])]
# }


def parse_openmp_pragma(pragma):
    '''
    parse OpenMP pragma into meaningful representation

    Parameters:
        pragma: str - string indicating the pragma
    Result:
        A list containing tuples representing the clause and their arguments

    Example:
        input: '#pragma omp for private  (a,b,c) lastprivate(d) schedule(static:8)'
        output: [('pragma', ''), ('omp', ''), ('for', ''), ('private', 'a,b,c'), ('lastprivate', 'd'), ('schedule', 'static:8')]

    '''
    pragma = pragma + " "
    pattern = r'(\w+(\s*\(.*?\)|\s))'
    matches = re.findall(pattern, pragma)
    clauses = []
    
    for match in matches:
        clause = match[0].strip()

        if '(' in clause:
            clause = clause[:clause.find('(')].strip()
            args = match[1].strip()[1:-1]
            clauses.append((clause, args))
        else:
            clauses.append((clause, ''))
    
    return clauses


def update_versions(line, versions):
    '''
    Determine the version of OpenMP pragma 

    Parameters:
        code: str - line of code
        versions: dict - amount of each clause (per version)

    Return:
        None (updates the versions dictionary)
    '''
    line = line.lstrip()
    parsed_pragma = parse_openmp_pragma(line)

    for ver, clauses_combination in clauses_per_version.items():
        for clause_combination in clauses_combination:
            
            clauses, args = clause_combination
            arg = '' if len(args)==0 else args[0]
            key = '_'.join(clauses) + '_' + arg

            if all([any([(clause==pragma_clause and arg in pragma_args) for pragma_clause, pragma_args in parsed_pragma]) for clause in clauses]):
                versions[ver][key] = 1 if key not in versions[ver] else versions[ver][key]+1


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

    versions = {'ver2.5': {}, 'ver3.0':{}, 'ver3.1':{}, 'ver4.0':{}, 'ver4.5':{}, 'ver5.0':{}, 'ver5.1':{}, 'ver5.2':{}}

    for line in code.split('\n'):

        if is_for(line, is_fortran):
            total_loop += 1

        if is_omp_pragma(line, is_fortran):
            update_versions(line, versions)

    return total_loop, versions




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
                js = json.loads(line.strip())

                if 'content' not in js:
                    continue
                
                repo_name = js['repo_name']
                code = js['content']
                code = remove_comments(code, is_fortran)
                code = concat_lines(code, is_fortran)

                ## Parallel Paradigms ###
                paradigms = get_parallel_paradigms(code)

                if repo_name not in par_paradigms:
                    par_paradigms[repo_name] = {'CUDA': False, 'OpenCL': False, 'OpenACC': False, 'SYCL': False, 
                                          'TBB': False, 'Cilk': False, 'OpenMP': False, 'MPI': False}

                for paradigm in paradigms:
                    par_paradigms[repo_name][paradigm] = True
                ## Parallel Paradigms ###


                # ### OpenMP versions ###
                # total_loop, versions = get_omp_version(code.lower(), is_fortran)

                # if repo_name not in omp_versions:
                #     omp_versions[repo_name] = {'total_loop': 0, 'vers':  {'ver2.5': {}, 'ver3.0':{}, 'ver3.1':{}, 'ver4.0':{}, 'ver4.5':{}, 'ver5.0':{}, 'ver5.1':{}, 'ver5.2':{}} }

                # omp_versions[repo_name]['total_loop'] += total_loop

                # for ver in versions.keys():
                #     for clause, amount in versions[ver].items():
                #         omp_versions[repo_name]['vers'][ver][clause] = amount if clause not in omp_versions[repo_name]['vers'][ver] else \
                #                                                                 omp_versions[repo_name]['vers'][ver][clause] + amount
                # ### OpenMP versions ###
    
    with open('{}_paradigms.json'.format(dir), 'w') as f:
        f.write(json.dumps(par_paradigms))

    # with open('{}_versions_correct.json'.format(dir), 'w') as f:
    #     f.write(json.dumps(omp_versions))



if __name__ == '__main__':

    with open('/homes/talkad/OMPify/database_creator/visualization/hpcorpus_stats/statistics/ENV.json', 'r') as f:
        vars = json.loads(f.read())

    iterate_jsons('c', vars, is_fortran=False)
