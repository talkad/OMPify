import re
from enum import Enum

class State(Enum):
    start = 1
    for_stmt = 2
    for_body = 3
    for_body_found = 4


LINE_COMMENT_RE = re.compile("//.*?\n" )
MULTILINE_COMMENT_RE = re.compile("/\*.*?\*/", re.DOTALL)


def is_for_pragma(line):
    '''
    Returns true if the given line is an OMP pragma

    Parameters:
        line (str) - a single line from source coded 
    '''
    sub_line = line.lstrip() # remove redundant white spaces

    return sub_line.startswith('#pragma ') and ' omp' in line and ' for' in line

def is_eof(cursor, code):
    return cursor >= len(code)

def extract_for(code, start, end):

    code_lines = code.split('\n')

    for idx, line in enumerate(code_lines):
        if line.lstrip().startswith('for'):
            code = '\n'.join(code_lines[idx:])
            break


    loop = ''
    body = ''

    num_paren = 0
    state = State.start

    for ch in code:
        if state == State.start:
            loop += ch

            if ch == '(':
                state = State.for_stmt
                num_paren += 1

        elif state == State.for_stmt:
            loop += ch

            if ch == '(':
                num_paren += 1
            elif ch == ')':
                num_paren -= 1

            if num_paren == 0:
                state = State.for_body

        elif state == State.for_body or state == State.for_body_found:
            loop += ch

            if ch == '{':
                num_paren += 1
                state = State.for_body_found
            elif  ch == '}':
                num_paren -= 1

            if num_paren == 0 and state == State.for_body_found:
                print(loop)
                return




with open('/home/talkad/Downloads/thesis/data_gathering_script/asd/1/canny.cpp', 'r') as f:
    code = f.read()
    code = LINE_COMMENT_RE.sub('', code)
    code = MULTILINE_COMMENT_RE.sub('', code)

    extract_for(code, 0, len(code))


        

