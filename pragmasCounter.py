import matplotlib.pyplot as plt
import os

fortran_extensions = ['.f', '.f90']
exts = ['.c', '.f', '.f77', '.f90', '.f95', '.f03', '.cc', '.cpp', '.cxx', '.h']
clauses = {b'nowait': 'nowait', b'private': 'private', b'firstprivate': 'firstprivate', b'lastprivate': 'lastprivate', b'shared': 'shared', b'reduction': 'reduction', b'static_schedule' : 'static_schedule', b'dynamic_schedule': 'dynamic_schedule'}


def is_for_pragma(line):
	sub_line = line.lstrip() # remove redundant white spaces

	return (sub_line.startswith(b'#pragma ') and b' omp ' in line and b' for' in line) or \
		(sub_line.startswith(b'!$omp ') and b' do' in line and b' end' not in line)


def is_fortran_comment(line):
	'''
	A line with a c, C, or ! in column one is a comment line

	Parameters:
		line - lower case binary string from source code
	'''
	sub_line = line.lstrip() # remove redundant white spaces

	return len(sub_line) == 0 or \
		line[0] == ord('c') or \
		(sub_line[0] == ord('!') and not (len(sub_line) > 1 and sub_line[1] == ord('$')))


def is_c_comment(line):
	'''
	A line starts with //

	Parameters:
		line - lower case binary string from source code
	'''
	sub_line = line.lstrip() # remove redundant white spaces

	return len(sub_line) >= 2 and sub_line[:2] == b'//'


def is_multiple_line_comment(line, start_comment=True):
	sub_line = line.lstrip() # remove redundant white spaces
	
	return (len(sub_line) >= 2 and sub_line[:2] == b'/*') if start_comment else (len(sub_line) >= 3 and sub_line[-3:] == b'*/\n')

	

def clauses_counter(line, clauses_dict):
	'''
	count each clause in line of code

	Precondition:
		clauses_dict is initiated with all possible clauses
	'''
	
	for clause in list(clauses.keys())[: -2]:
		if clause in line:
			clauses_dict[clauses[clause]] += 1

	if b'schedule' in line:
		if b'static' in line:
			clauses_dict['static_schedule'] += 1
		if b'dynamic' in line:
			clauses_dict['dynamic_schedule'] += 1


def scan_file(root, filename, clauses_amount):
	total_clauses = 0
	curr_line_comment = False
	is_fortran = os.path.splitext(filename)[1].lower() in fortran_extensions

	is_line_comment = lambda line: is_fortran_comment(line) if is_fortran else is_c_comment(line)

	with open(f'{root}/{filename}', 'rb') as f:
		for line in f:
			l = line.lower()
			
			if not is_fortran:
				if is_multiple_line_comment(line): # check for multiple line comment
					curr_line_comment = True
				if is_multiple_line_comment(line, start_comment=False):
					curr_line_comment = False
					continue

			if curr_line_comment or is_line_comment(l):
				#if b'#pragma ' in line:
				#	print(line, f'{root}/{filename}') debug
				continue
			
			if is_for_pragma(l): # check if pragma
				#print(line)
				total_clauses += 1
				clauses_counter(l, clauses_amount)

	return total_clauses
	

def scan_dir(root_dir):
	num_files = 0
	num_relevant_files = 0
	total = 0
	clauses_dist = {}
	clauses_amount = {clause: 0 for clause in clauses.values()}
	
	for idx, (root, dirs, files) in enumerate(os.walk(root_dir)):
		for file_name in files:
			ext = os.path.splitext(file_name)[1].lower()
			amount = scan_file(root, file_name, clauses_amount)
			clauses_dist[ext] = (clauses_dist[ext] if ext in clauses_dist else 0) + amount
			total += amount
			num_files += 1
			if amount > 0:
				num_relevant_files += 1

			#if ext == '.cpp':
			#	print(f'{root}/{file_name} : {amount}')
			
		if idx % (10**3) == 0:
			print(f'{idx // (10**3)}) {clauses_dist}')
			#print(f'{idx // (10**3)}) {clauses_amount} | {total}')

	return total, clauses_amount, num_files, num_relevant_files


res = scan_dir("/home/talkad/Downloads/thesis/data_gathering_script/repositories_openMP")
print(res)


