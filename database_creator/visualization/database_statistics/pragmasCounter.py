import os
import re
import json
from tqdm import tqdm
from functools import reduce

# clauses = ['nowait', 'private', 'firstprivate', 'lastprivate', 'shared', 'reduction', 'atomic', 'section', 'for', 'task', 'barrier', 'critical', 'flush', 'single', 'master', 'target', 'static_schedule', 'dynamic_schedule']
clauses = ['nowait', 'private', 'firstprivate', 'lastprivate', 'shared', 'reduction', 'atomic', 'section', 'do', 'task', 'barrier', 'critical', 'flush', 'single', 'master', 'target', 'static_schedule', 'dynamic_schedule', 'loop_total', 'omp_lock']

# C comments:

# ignore one line comment
LINE_C_COMMENT_RE = re.compile("\/\/.*")
MULTILINE_COMMENT_RE = re.compile("\/\*.*?\*\/", re.MULTILINE|re.DOTALL)

# LINE_F_COMMENT_RE = re.compile("![^$].*$")


def is_omp_pragma(line, is_fortran):
	'''
	Return true if the given line is for-pragma
	'''
	sub_line = line.lstrip() # remove redundant white spaces
	if is_fortran:
		return sub_line.startswith('!$') and 'omp' in line and 'end' not in line
	else:
		return sub_line.startswith('#pragma ') and ' omp ' in line


def remove_c_comments(code):
	code = LINE_C_COMMENT_RE.sub("", code)
	return MULTILINE_COMMENT_RE.sub("", code)


def remove_f_comments(code):
	code_buf = []
	# code = LINE_F_COMMENT_RE.sub("", code)

	for line in code.split('\n'):
		if not line.startswith('c') and (not (line.lstrip().startswith('!') and (not line.lstrip().startswith('!$')))):
			code_buf.append(line)

	return '\n'.join(code_buf)


def clauses_counter(line, clauses_dict):
	'''
	count each clause in line of code

	Precondition:
		clauses_dict is initiated with all possible clauses
	'''
	
	for clause in clauses[: -4]:
		if clause in line:
			clauses_dict[clause] += 1

	if 'schedule' in line:
		if 'static' in line:
			clauses_dict['static_schedule'] += 1
		if 'dynamic' in line:
			clauses_dict['dynamic_schedule'] += 1


def scan_file(code, clauses_amount, is_fortran):
	if is_fortran:
		code = remove_f_comments(code)
	else:
		code = remove_c_comments(code)

	for line in code.split('\n'):
		l = line.lower()

		if line.lstrip().startswith('do'):
			clauses_amount['loop_total']  += 1

		if line.lstrip().startswith('omp_set_lock'):
			clauses_amount['omp_lock'] += 1

		if is_omp_pragma(l, is_fortran=is_fortran): # check if pragma
			# print(l)
			clauses_counter(l, clauses_amount)


def iterate_jsons(json_dir, is_fortran=False):
    clauses_amount = {clause: 0 for clause in clauses}

    for json_file in tqdm(os.listdir(json_dir)):
        with open(os.path.join(json_dir, json_file), 'r') as f:
            for line in f:
                js = json.loads(line.strip())

                if 'content' not in js:
                    continue

                scan_file(js['content'], clauses_amount, is_fortran=is_fortran)

    return clauses_amount


print(iterate_jsons('/home/talkad/shared/nadavsc/c'))
# print(iterate_jsons('/home/talkad/LIGHTBITS_SHARE/dataset_gal_cpp/cpp'))
# print(iterate_jsons('/home/talkad/LIGHTBITS_SHARE/dataset_gal_fortran/f90', is_fortran=True))

# cpp:
# {'nowait': 2274, 'private': 34420, 'firstprivate': 10363, 'lastprivate': 8519, 'shared': 10208, 'reduction': 22875, 'atomic': 5394, 'section': 9654, 'for': 89726, 'task': 18270, 'barrier': 2728, 'critical': 8657, 'flush': 1398, 'single': 3469, 'master': 7871, 'target': 79076, 'static_schedule': 5628, 'dynamic_schedule': 3823, 'omp_lock': 389}

# c:
# {'nowait': 1306, 'private': 17408, 'firstprivate': 2612, 'lastprivate': 2231, 'shared': 10148, 'reduction': 3948, 'atomic': 5180, 'section': 3266, 'for': 40339, 'task': 5931, 'barrier': 3765, 'critical': 6431, 'flush': 808, 'single': 1411, 'master': 1789, 'target': 8265, 'static_schedule': 8184, 'dynamic_schedule': 2623, 'loop_total': 8739725, 'omp_lock': 561}

# fortran
# {'nowait': 17, 'private': 17538, 'firstprivate': 1565, 'lastprivate': 481, 'shared': 6563, 'reduction': 2930, 'atomic': 2103, 'section': 1006, 'do': 18798, 'task': 1695, 'barrier': 869, 'critical': 1077, 'flush': 205, 'single': 623, 'master': 607, 'target': 2592, 'static_schedule': 2187, 'dynamic_schedule': 864, 'loop_total': 774559, 'omp_lock': 0}
