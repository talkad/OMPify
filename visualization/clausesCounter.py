import os
import re
import json
from functools import reduce
from parsers.cParser import CLoopParser
from parsers.cppParser import CppLoopParser
from parsers.fortranParser import FortranLoopParser


clauses = ['nowait', 'private', 'firstprivate', 'lastprivate', 'shared', 'reduction', 'static_schedule', 'dynamic_schedule', 'target', 'reduction_private']


def clauses_counter(line, clauses_dict):
	'''
	count each clause in line of code

	Precondition:
		clauses_dict is initiated with all possible clauses
	'''
	
	for clause in clauses[: -1]:
		if clause in line:
			clauses_dict[clause] += 1

	if 'reduction' in line and 'private' in line:
		clauses_dict['reduction_private'] += 1

	if 'schedule' in line:
		if 'static' in line:
			clauses_dict['static_schedule'] += 1
		if 'dynamic' in line:
			clauses_dict['dynamic_schedule'] += 1


def is_target(code, pragma):

	if 'target' in pragma:
		return 1

	for line in code.split('\n'):

		if 'pragma ' in line and 'omp' in line and 'target' in line:
			return 1
	return 0


def get_parser(lang):
	if lang == 'c':
		return CLoopParser('', '')
	elif lang == 'cpp':
		return CppLoopParser('', '')
	else:
		return FortranLoopParser('', '')


def scan_dir(root_dir, lang='c'):
	pos, neg = 0, 0
	
	parser = get_parser(lang)
	clauses_amount = {clause: 0 for clause in clauses}
	
	with open(root_dir, 'r') as f:
		database = json.load(f)

	for data_path in database.values():
		data = parser.load(data_path)
		code = data.textual_loop
		pragma = '' if data.omp_pragma is None else data.omp_pragma if lang=='fortran' else data.omp_pragma.string

		if data.omp_pragma is not None:
			pos += 1
			clauses_counter(pragma, clauses_amount)
		else:
			neg += 1
		
		clauses_amount['target'] += is_target(code, pragma)

	print(pos, neg)
	return clauses_amount


def scan_json(json_path, lang='c'):	
	clauses_amount = {clause: 0 for clause in clauses}
	
	with open(scan_json, 'r') as f:
		database = json.load(f)

	for pragmas in database.values():
		for paragma in pragmas:

			if not pragma.startswith('#pragma'):
				continue
				
			clauses_counter(pragma, clauses_amount)			

	return clauses_amount


# res = scan_dir('/home/talkad/Downloads/thesis/data_gathering_script/sample_fortran.json', lang='fortran')

# print(res)


# cpp
# 8241 14323
# {'nowait': 310, 'private': 1190, 'firstprivate': 133, 'lastprivate': 60, 'shared': 796, 'reduction': 1120, 'static_schedule': 944, 'dynamic_schedule': 536, 'target': 505, 'reduction_private': 169}


# c
# 14906 17193
# {'nowait': 752, 'private': 5568, 'firstprivate': 930, 'lastprivate': 147, 'shared': 1663, 'reduction': 2147, 'static_schedule': 1891, 'dynamic_schedule': 866, 'target': 212, 'reduction_private': 879}


# fortran
# 2749 3368
# {'nowait': 8, 'private': 1136, 'firstprivate': 33, 'lastprivate': 60, 'shared': 285, 'reduction': 327, 'static_schedule': 190, 'dynamic_schedule': 43, 'target': 73, 'reduction_private': 161}

