import os
import re
import json

# clauses = ['nowait', 'private', 'firstprivate', 'lastprivate', 'shared', 'reduction', 'atomic', 'section', 'for', 'task', 'barrier', 'critical', 'flush', 'single', 'master', 'target', 'static_schedule', 'dynamic_schedule']



clauses = ['nowait', 'private', 'firstprivate', 'lastprivate', 'shared', 'reduction', 'atomic', 'section', 'for', 'task', 'barrier', 'critical', 'flush', 'single', 'simd', 'master', 'target', 'parallel_for', 'static_schedule', 'dynamic_schedule', 'loop_total']


redundant_line_comments_c = re.compile("\/\/.*")
redundant_line_comments_fortran = re.compile("![^\$].*$|^c.*$", re.MULTILINE)
redundant_multiline_comments_c = re.compile("\/\*.*?\*\/", re.MULTILINE|re.DOTALL)


def remove_comments(code, lang='c'):
    if lang == 'fortran':
        code = redundant_line_comments_fortran.sub("\n", code)
        code = redundant_multiline_comments_c.sub("\n", code)
    else:
        code = redundant_line_comments_c.sub("\n", code)
        code = redundant_multiline_comments_c.sub("\n", code)

    return code


def is_for(line, lang='c'):
    '''
	Return true if the given line is the beginning of for-loop
	'''
    sub_line = line.lstrip() # remove redundant white spaces

    if lang == 'c':
        return sub_line.startswith('for') and sub_line[3:].lstrip().startswith('(')

    return sub_line.startswith('do ')


def is_omp_pragma(line, is_fortran):
	'''
	Return true if the given line is for-pragma
	'''
	sub_line = line.lstrip() # remove redundant white spaces
	if is_fortran:
		return sub_line.startswith('!$') and 'omp' in line and 'end' not in line
	else:
		return sub_line.startswith('#pragma ') and ' omp ' in line


def clauses_counter(line, clauses_dict, is_fortran):
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
			
	if is_fortran:
		if 'parallel ' in line and 'do' in line:
			clauses_dict['parallel_for'] += 1
	else:
		if 'parallel ' in line and 'for' in line:
			clauses_dict['parallel_for'] += 1



def scan_file(code, clauses_amount, is_fortran):
	lang = 'fortran' if is_fortran else 'c'
	code = remove_comments(code, lang=lang)
	code = code.lower()

	for line in code.split('\n'):
		
		if is_for(line, lang=lang):
			clauses_amount['loop_total']  += 1

		if is_omp_pragma(line, is_fortran): # check if pragma
			clauses_counter(line, clauses_amount, is_fortran)


def iterate_jsons(json_dir, is_fortran=False):
    clauses_amount = {clause: 0 for clause in clauses}

    for json_file in os.listdir(json_dir):
        with open(os.path.join(json_dir, json_file), 'r') as f:
            for line in f:
                js = json.loads(line.strip())

                if 'content' not in js:
                    continue

                scan_file(js['content'], clauses_amount, is_fortran=is_fortran)

    return clauses_amount




# clauses = iterate_jsons('/tier2/bgu/bigQuery_repos/c')
clauses = iterate_jsons('/tier2/bgu/bigQuery_repos/cpp')
# clauses = iterate_jsons('/tier2/bgu/bigQuery_repos/Fortran', is_fortran=True)

with open('cpp.json', 'w') as f:
    f.write(json.dumps(clauses))

