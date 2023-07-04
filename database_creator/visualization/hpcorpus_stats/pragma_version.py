#!/usr/bin/python 


import os
import json
import re


redundant_line_comments_c = re.compile("\/\/.*")
redundant_line_comments_fortran = re.compile("![^\$].*$|^c.*$", re.MULTILINE)
redundant_multiline_comments_c = re.compile("\/\*.*?\*\/", re.MULTILINE|re.DOTALL)

corpus_dir = '/tier2/bgu/bigQuery_repos'

def remove_comments(code, is_fortran):
    if is_fortran:
        code = redundant_line_comments_fortran.sub("\n", code)
        code = redundant_multiline_comments_c.sub("\n", code)
    else:
        code = redundant_line_comments_c.sub("\n", code)
        code = redundant_multiline_comments_c.sub("\n", code)

    return code


def is_for(line, is_fortran):
    '''
	Return true if the given line is the beginning of for-loop
	'''
    sub_line = line.lstrip() # remove redundant white spaces

    if is_fortran:
        return sub_line.startswith('do ')
    
    return sub_line.startswith('for') and sub_line[3:].lstrip().startswith('(')


def is_omp_pragma(line, is_fortran):
	'''
	Return true if the given line is for-pragma
	'''
	sub_line = line.lstrip() # remove redundant white spaces
	if is_fortran:
		return sub_line.startswith('!$') and 'omp' in line and 'end' not in line
	else:
		return sub_line.startswith('#pragma ') and ' omp ' in line



def get_omp_version_per_clause(line, res, is_fortran):
    '''
        For a given pragma, determine the version of OpenMP it is related
    '''
    if is_for(line, is_fortran):
        res['total_loop'] += 1

    if is_omp_pragma(line, is_fortran):
        #     version 5
        clauses = [clause for clause in ['requires','loop', 'order(concurrent)', 'scan', 'inscan',  'mutexinoutset', 'taskloop'] if clause in line]
        if 'atomic' in line and 'hint' in line:
            clauses.append('atomic_hint')
        if 'taskwait' in line and 'depend' in line:
            clauses.append('taskwait_depend')
        if 'task' in line:
            clauses += ['task_'+clause for clause in ['detach', 'affinity', 'in_reduction'] if clause in line]

        for clause in clauses:
            res['5'][clause] = 1 if clause not in res['5'] else res['5'][clause]+1

        #     version 4.5
        clauses = [clause for clause in ['linear', 'simdlen', 'target'] if clause in line]
        if 'task' in line and 'priority' in line:
            clauses.append('task_priority')
        if 'target' in line:
            clauses += ['target_'+clause for clause in ['private', 'nowait', 'is_device_ptr', 'depend', 'firstprivate', 'defaultmap'] if clause in line]
        
        for clause in clauses:
            res['4.5'][clause] = 1 if clause not in res['4.5'] else res['4.5'][clause]+1

        #     version 4
        clauses = [clause for clause in ['cancel', 'taskgroup', 'proc_bind', 'simd'] if clause in line]
        if 'task' in line and 'depend' in line:
            clauses.append('task_depend')

        for clause in clauses:
            res['4'][clause] = 1 if clause not in res['4'] else res['4'][clause]+1

        #     version 3
        clauses = [clause for clause in ['task', 'collapse', 'taskwait', 'schedule(auto)', 'taskyield'] if clause in line]
        if 'task' in line:
            clauses += ['task_'+clause for clause in ['final', 'mergeable'] if clause in line]
        if 'atomic' in line:
            clauses += ['atomic_'+clause for clause in ['read', 'write', 'capture', 'update'] if clause in line]
        
        for clause in clauses:
            res['3'][clause] = 1 if clause not in res['3'] else res['3'][clause]+1

        #     version 2.5
        clauses = [clause for clause in [' private', 'section', 'barrier', 'critical', 'flush', 'single', 'master', 'firstprivate', 'lastprivate', 'shared', 'reduction', ' if', 'num_threads'] if clause in line]
        
        if 'schedule' in line:
            clauses += ['schedule_'+clause for clause in ['static', 'dynamic'] if clause in line]

        if (is_fortran and ' do' in line) or (not is_fortran and ' for' in line):
            clauses += ['for']

        if 'parallel' in line and (is_fortran and ' do' in line) or (not is_fortran and ' for' in line):
            clauses += ['parallel_for']

        for clause in clauses:
            res['2'][clause] = 1 if clause not in res['2'] else res['2'][clause]+1


def iterate_jsons(dir, is_fortran=False):
    versions = {'total_loop': 0, '2': {}, '3':{}, '4':{}, '4.5':{}, '5':{}}
    dir_path = os.path.join(corpus_dir, dir)

    for json_file in os.listdir(dir_path):
        with open(os.path.join(dir_path, json_file), 'r') as f:
            for line in f:
                try:
                    js = json.loads(line.strip())
                except:
                    continue

                if 'content' not in js:
                    continue

                code = js['content']
                code = remove_comments(code, is_fortran)

                for line in code.split('\n'):
                    l = line.lstrip().lower()

                    get_omp_version_per_clause(l, versions, is_fortran)

    return versions


versions = iterate_jsons('cpp', is_fortran=False)
with open('cpp.json', 'w') as f:
    f.write(json.dumps(versions))



# fortran
# {"4.5": {"target_defaultmap": 154, "target": 2586, "simdlen": 97, "task_priority": 27, "target_firstprivate": 42, "target_private": 87, "target_is_device_ptr": 37, "target_nowait": 11, "linear": 452}, "total_loop": 1203913, "3": {"task": 1695, "atomic_read": 248, "taskwait": 312, "taskyield": 11, "task_mergeable": 24, "atomic_write": 263, "atomic_update": 209, "task_final": 34, "schedule(auto)": 15, "atomic_capture": 196, "collapse": 4793}, "2": {" private": 13862, "lastprivate": 481, "schedule_static": 2164, "barrier": 859, "for": 17359, "num_threads": 1039, "firstprivate": 1559, "parallel_for": 7885, "section": 979, "single": 584, "schedule_dynamic": 806, "reduction": 2819, "critical": 1067, "master": 579, "flush": 205, "shared": 6016, " if": 705}, "5": {"scan": 128, "task_detach": 38, "order(concurrent)": 124, "task_affinity": 38, "task_in_reduction": 33, "atomic_hint": 50, "taskloop": 331, "inscan": 75, "mutexinoutset": 2, "requires": 80, "loop": 452}, "4": {"cancel": 469, "proc_bind": 86, "taskgroup": 233, "simd": 1252}}


# c
# {"4.5": {"target_defaultmap": 88, "target": 6248, "simdlen": 789, "target_depend": 68, "task_priority": 23, "target_firstprivate": 338, "target_private": 938, "target_is_device_ptr": 91, "target_nowait": 144, "linear": 894}, "total_loop": 21936098, "3": {"task": 4501, "collapse": 2519, "taskwait": 641, "atomic_write": 552, "task_mergeable": 9, "taskyield": 29, "task_final": 42, "atomic_update": 207, "schedule(auto)": 34, "atomic_capture": 431, "atomic_read": 336}, "2": {" private": 9672, "lastprivate": 1623, "schedule_static": 6168, "barrier": 2833, "for": 29425, "num_threads": 1916, "parallel_for": 29425, "section": 2501, "reduction": 3056, "schedule_dynamic": 2093, "single": 1027, "firstprivate": 1908, "master": 1289, "critical": 4757, "flush": 599, "shared": 7712, " if": 1374}, "5": {"taskwait_depend": 13, "scan": 295, "task_detach": 32, "order(concurrent)": 125, "task_affinity": 44, "task_in_reduction": 88, "atomic_hint": 35, "taskloop": 1275, "inscan": 147, "mutexinoutset": 13, "requires": 71, "loop": 1514}, "4": {"cancel": 1240, "proc_bind": 166, "task_depend": 368, "taskgroup": 493, "simd": 9877}}


# cpp
# {"4.5": {"target_defaultmap": 1688, "target": 79076, "simdlen": 1224, "target_depend": 4124, "task_priority": 475, "target_firstprivate": 2609, "target_private": 6732, "target_is_device_ptr": 2503, "target_nowait": 715, "linear": 4642}, "total_loop": 19868390, "3": {"task": 18270, "collapse": 5168, "taskwait": 1012, "atomic_write": 288, "task_mergeable": 98, "taskyield": 705, "task_final": 431, "atomic_update": 552, "schedule(auto)": 208, "atomic_capture": 1497, "atomic_read": 242}, "2": {"num_threads": 4517, "lastprivate": 8519, "schedule_static": 5628, "barrier": 2728, "for": 89480, " private": 14387, "firstprivate": 10363, "parallel_for": 89480, "section": 9654, "single": 3469, "schedule_dynamic": 3823, "reduction": 22875, "critical": 8657, "master": 7871, "flush": 1398, "shared": 10194, " if": 8419}, "5": {"taskwait_depend": 37, "scan": 341, "task_detach": 52, "order(concurrent)": 50, "task_affinity": 96, "task_in_reduction": 1331, "atomic_hint": 25, "taskloop": 11074, "inscan": 77, "mutexinoutset": 12, "requires": 115, "loop": 11139}, "4": {"cancel": 988, "proc_bind": 798, "task_depend": 455, "taskgroup": 1113, "simd": 54557}}


