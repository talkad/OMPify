import os
import re
from functools import reduce

exts = ['.cpp']

def log(file_name, msg):
    with open(file_name, 'a') as f:
        f.write(f'{msg}\n')

def is_for(line):
	'''
	Return true if the given line is the beggining of for-loop

	(just for rough estimation, it won't capture all the loops)
	'''
	sub_line = line.lstrip() # remove redundant white spaces

	return sub_line.startswith('for') and sub_line[3:].lstrip().startswith('(')




def scan_file(root, filename):
	loop_amount = 0

	with open(os.path.join(root, filename), 'rb') as f:
		code = f.read().decode(errors='replace')

		for line in code.split('\n'):
			l = line.lower()
			
			if is_for(l):
				log('loops.txt', l)
				loop_amount += 1

	return loop_amount
	

def scan_dir(root_dir):
	total = 0
	
	for idx, (root, dirs, files) in enumerate(os.walk(root_dir)):
		for file_name in files:
			ext = os.path.splitext(file_name)[1].lower()
			
			if ext in exts:
				amount = scan_file(root, file_name)
				total += amount
			
		if idx % (10**3) == 0:
			print(f'total: {total}')

	return total


res = scan_dir("/home/talkad/Downloads/thesis/data_gathering_script/repositories_openMP")
print(res)

# 289580