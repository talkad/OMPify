import os
import shutil
import json


# with open('../ENV.json', 'r') as f:
# 	vars = json.loads(f.read())

repos_dir = 'git_repos'
extentions = ['.c', '.f', '.f77', '.f90', '.f95', '.f03', '.cc', '.cpp', '.cxx', '.h']
fortran_extentions = ['.f', '.f77', '.f90', '.f95', '.f03']
start_idx = len(os.path.join(os.getcwd(), repos_dir)) + 1

def copy_file(src, dst):
	'''
	copy src file to a given destination (preserve the file structure)
	'''
	src = src[start_idx:]
	dst = os.path.join(dst, src)
	dstfolder = os.path.dirname(dst)

	if not os.path.exists(dstfolder):
		os.makedirs(dstfolder)

	shutil.copy(os.path.join(repos_dir, src), dst)


def is_for_pragma(line, lang='c'):
	'''
	Return true if the given line is a for-pragma
	'''
	line = line.lower()
	sub_line = line.lstrip() # remove redundant white spaces

	if lang == 'c':
		return 'pragma ' in line and ' omp ' in line and ' for' in line
	
	return 'omp ' in line and ' do' in line


def scan_dir(root_dir):

	result = {}
	copy = False

	for idx, (root, dirs, files) in enumerate(os.walk(root_dir)):
		for file_name in files:
			copy = False
			ext = os.path.splitext(file_name)[1].lower()
			
			if ext in extentions:
				with open(f'{root}/{file_name}', 'r') as f:
					for line in f:

						if is_for_pragma(line, 'c' if ext not in fortran_extentions else 'f'):
							copy = True
							result[ext] = (result[ext] if ext in result else 0) + 1

						if copy:
							copy_file(os.path.join(root, file_name), 'repositories_openMP')
							break

		if idx % 10**3 == 0:
			print(f'{idx // (10**3)}) {result}')

	return result

# res = scan_dir(vars['REPOS_DIR'])
# print(res)

# {'.cpp': 12381, '.c': 19784, '.f90': 4352, '.h': 933, '.cc': 513, '.f': 1528, '.f95': 279, '.f03': 10, '.cxx': 69}
