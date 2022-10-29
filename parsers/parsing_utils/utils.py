import re
import copy
import itertools


redundant_ompts = re.compile("<ompts:testdescription>.*<\/ompts:testdescription>|<ompts:description>.*<\/ompts:description>|<ompts:version>.*<\/ompts:version>|<ompts:ompversion>.*<\/ompts:ompversion>|<ompts:directive>.*<\/ompts:directive>|<ompts:dependences>.*<\/ompts:dependences>|<ompts:.*?>|<\/ompts:.*>")
redundant_directives = re.compile("MAYBE_INLINE|TM_CALLABLE|__block|RESTRICT|__targetConst__|__targetHost__")

if_directive = re.compile("^\W*#\W*if\W(.*)|^\W*#\W*elif\W(.*)")
ifdef_directive = re.compile("^\W*#\W*ifdef\W(.*)|^\W*#\W*ifndef\W(.*)")


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



def remove_redundants(code):
    '''
    remove lines containing namespace or #error
    '''
    code_buf = []

    for line in code.split('\n'):
        l = line.lower().split()

        if (len(l) > 2 and l[0] == 'using' and l[1] == 'namespace') or \
        line.lstrip().startswith('#error'):
            continue

        code_buf.append(line)

    return '\n'.join(code_buf)


def remove_paren(code):
    flag = False
    num_paren = 0
    idx = 0

    for letter in code:
        if flag and num_paren == 0:
            return code[idx: ]

        if letter == '(':
            flag = True
            num_paren += 1
        elif letter == ')':
            num_paren -= 1

        idx += 1

    return ''


def remove_attribute(code):
    splitted_code = code.split('__attribute__')
    
    if len(splitted_code) == 1:
        return code

    updated_code = list(map(lambda code: remove_paren(code), splitted_code[1:]))

    return ''.join(list(splitted_code[0]) + updated_code)


def remove_ompt(code):
    '''
    Remove redundant compiler directives
    '''
    code = redundant_directives.sub("", code)
    return redundant_ompts.sub("", code)


def update_code_pipline(code):
    FAKE_TYPEDEFS = '_fake_typedefs.h'
    
    code = remove_redundants(code)
    code = remove_attribute(code)
    code = remove_ompt(code)
    code = FAKE_INCLUDE  = f'#include \"{FAKE_TYPEDEFS}\"\n{code}'

    return code


def is_if_directive(line):
	'''
	Returns true if the line is compiler-directive condition
    There are 19879 if-directives
	'''
	sub_line = line.lstrip().lower()
	return sub_line.startswith("#if") or sub_line.startswith("#elif") or sub_line.startswith("#ifdef") or sub_line.startswith("#ifndef")


def update_if_directive(line, stat):
    '''
    Return positive/negative form of line (which is a condition) according to stat
    '''
    if stat:
        return line

    match = re.search(if_directive, line)

    if match is not None:
        if '#elif' in line:
            return f'#elif !({match.group(2)})'
        else:
            return f'#if !({match.group(1)})'

    match = re.search(ifdef_directive, line)

    if match is not None:
        if '#ifdef' in line:
            return f'#ifndef {match.group(1)}'
        else:
            return f'#ifdef {match.group(2)}'

    return line


def get_if_permutations(code):
    '''
    for a given code segment return all possible permutations for conditions 
    '''
    limit = 6 # 2**6 = 64 permutations

    code_permutations = []
    code_buf = code.split('\n')
    if_amount = sum(list(map(is_if_directive, code_buf)))

    if if_amount == 0:
        return [code]

    if_idx = list(filter(lambda idx: is_if_directive(code_buf[idx]), range(len(code_buf))))[:limit]
    bool_permutations = [list(i) for i in itertools.product([False, True], repeat=min(if_amount, limit))]
    
    for permutation in bool_permutations:
        code_buf_copy = copy.deepcopy(code_buf)

        for idx, stat in zip(if_idx, permutation):
            code_buf_copy[idx] = update_if_directive(code_buf[idx], stat)

        code_permutations.append("\n".join(code_buf_copy))
    
    return code_permutations


    


# code = """
# #define AA 5

# #ifdef BB

# #if (AA==1)

# int main(){
#     int sum = 0;
#     for(int i=0; i<10;++i)
#         sum += i;

#     printf(sum);
# }
# #elif (AA==2)

# int main(){
#     int sum = 0;
#     #pragma omp for
#     for(int i=0; i<10;++i)
#         sum += i;

#     printf(sum);
# }
# #else 
# int main(){
#     int sum = 0;
#     for(int i=0; i<10;++i)
#     {

#     }

#     printf(sum);
# }


# #endif

# """

# get_if_permutations(code)