
def is_for(line):
	'''
	Return true if the given line is the beggining of for-loop
	'''
	sub_line = line.lstrip() # remove redundant white spaces
	return sub_line.startswith('for') and sub_line[3:].lstrip().startswith('(')


def is_for_pragma(line):
	'''
	Return true if the given line is for-pragma
	'''
	sub_line = line.lstrip() # remove redundant white spaces

	return (sub_line.startswith('#pragma ') and ' omp ' in line and ' for' in line)


def count_for(file_path):
    '''
    Returns the amout of for-loops and pragmas exist in a given file
    '''
    loop_amount, pragma_amount = 0, 0

    with open(file_path, 'r') as f:
        try:
            code = f.read()
        except UnicodeDecodeError:
            return 0

        for line in code.split('\n'):
            l = line.lower()
			
            if is_for(l):
                loop_amount += 1

            if is_for_pragma(l):
                pragma_amount += 1

    return loop_amount, pragma_amount