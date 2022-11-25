import os
import re
import json
from functools import reduce
from parsers.cParser import CLoopParser

clauses = ['nowait', 'private', 'firstprivate', 'lastprivate', 'shared', 'reduction', 'static_schedule', 'dynamic_schedule']


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




def scan_dir(root_dir):
	pos, neg = 0, 0
	parser = CLoopParser('', 'c_loops')
	clauses_amount = {clause: 0 for clause in clauses}
	
	with open(root_dir, 'r') as f:
		database = json.load(f)

	for data_path in database.values():
		data = parser.load(data_path.replace('c_loops', 'c_loops_main'))

		if data.omp_pragma is not None:
			pos += 1
			clauses_counter(data.omp_pragma.string, clauses_amount)
		else:
			neg += 1

	print(pos, neg)
	return clauses_amount


# res = scan_dir('/home/talkad/Downloads/thesis/data_gathering_script/updated_db.json')
# print(res)

# 14488 14599
# {'nowait': 751, 'private': 5233, 'firstprivate': 728, 'lastprivate': 141, 'shared': 1646, 'reduction': 2123, 'static_schedule': 1884, 'dynamic_schedule': 863}

