import os
import re
from functools import reduce

fortran_extensions = ['.f', '.f77', '.f90', '.f95', '.f03']
exts = ['.c', '.f', '.f77', '.f90', '.f95', '.f03', '.cc', '.cpp', '.cxx', '.h']
# clauses = {b'nowait': 'nowait', b'private': 'private', b'firstprivate': 'firstprivate', b'lastprivate': 'lastprivate', b'shared': 'shared', b'reduction': 'reduction', b'static_schedule' : 'static_schedule', b'dynamic_schedule': 'dynamic_schedule'}
clauses = ['nowait', 'private', 'firstprivate', 'lastprivate', 'shared', 'reduction', 'static_schedule', 'dynamic_schedule']

# C comments:

# ignore one line comment
LINE_C_COMMENT_RE = re.compile("//.*?\n" )
# ignore multiline comment
MULTILINE_COMMENT_RE = re.compile("/\*.*?\*/", re.DOTALL)


def is_for_pragma(line):
	'''
	Return true if the given line is for-pragma
	'''
	sub_line = line.lstrip() # remove redundant white spaces

	return (sub_line.startswith('#pragma ') and ' omp ' in line and ' for' in line) or \
		(sub_line.startswith('!$omp ') and ' do' in line and ' end' not in line)


def remove_c_comments(code):
	code = LINE_C_COMMENT_RE.sub("", code)
	return MULTILINE_COMMENT_RE.sub("", code)


def remove_fortran_comments(code):
	code = reduce(lambda acc, cur: acc if cur.lstrip().lower().startswith('!') and not cur.lstrip().lower().startswith('!$omp') else f'{acc}\n{cur}', code.split('\n'))
	return reduce(lambda acc, cur: acc if cur.lstrip().lower().startswith(('c ','c\t','c\n')) else f'{acc}\n{cur}', code.split('\n'))


def clauses_counter(line, clauses_dict):
	'''
	count each clause in line of code

	Precondition:
		clauses_dict is initiated with all possible clauses
	'''
	
	for clause in clauses[: -2]:
		if clause in line:
			clauses_dict[clause] += 1

	if 'schedule' in line:
		if 'static' in line:
			clauses_dict['static_schedule'] += 1
		if 'dynamic' in line:
			clauses_dict['dynamic_schedule'] += 1


def scan_file(root, filename, clauses_amount):
	total_clauses = 0
	is_fortran = os.path.splitext(filename)[1].lower() in fortran_extensions

	remove_comments = lambda code: remove_fortran_comments(code) if is_fortran else remove_c_comments(code)

	with open(os.path.join(root, filename), 'rb') as f:
		code = f.read().decode(errors='replace')
		code = remove_comments(code)

		for line in code.split('\n'):
			l = line.lower()
			
			if is_for_pragma(l): # check if pragma
				total_clauses += 1
				clauses_counter(l, clauses_amount)

	return total_clauses
	

def scan_dir(root_dir):
	file_dist = {}
	clauses_dist = {}
	clauses_amount = {clause: 0 for clause in clauses}
	
	for idx, (root, dirs, files) in enumerate(os.walk(root_dir)):
		for file_name in files:
			ext = os.path.splitext(file_name)[1].lower()
			amount = scan_file(root, file_name, clauses_amount)
			file_dist[ext] = (file_dist[ext] if ext in file_dist else 0) + 1
			clauses_dist[ext] = (clauses_dist[ext] if ext in clauses_dist else 0) + amount
			
		if idx % (10**3) == 0:
			print(f'num of files: {clauses_dist}')

	return clauses_amount, clauses_dist, file_dist


# res = scan_dir("/home/talkad/Downloads/thesis/data_gathering_script/repositories_openMP")
# print(res)


# ({'nowait': 7652, 'private': 60042, 'firstprivate': 15672, 'lastprivate': 16580, 'shared': 10960, 'reduction': 36193, 'static_schedule': 18481, 'dynamic_schedule': 13102}, 
# {'.cpp': 169966, '.c': 70113, '.f90': 15285, '.h': 3246, '.cc': 1193, '.f': 5571, '.f95': 337, '.f03': 36, '.cxx': 132}, 
# {'.cpp': 14421, '.c': 20174, '.f90': 4367, '.h': 1065, '.cc': 575, '.f': 1528, '.f95': 279, '.f03': 10, '.cxx': 71})