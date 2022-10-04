import re
from enum import Enum
# from parser import Parser
import pycparser
from pycparser.c_parser import CParser

class State(Enum):
    start = 1
    for_stmt = 2
    for_body = 3
    for_body_found = 4

loops = []


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

def skip_lines(code):
    '''
    skip lines that are not part of a for loop
    '''
    code_lines = code.split('\n')

    for idx, line in enumerate(code_lines):
        if line.lstrip().startswith('for'):
            code = '\n'.join(code_lines[idx:])
            return code

    return ''

def extract_for(code, is_nested=False):
    '''
    Extract for loops by parentheses balancing
    '''
    code = skip_lines(code)

    idx = 0
    loop = ''
    body = ''

    num_paren = 0
    state = State.start

    for idx, ch in enumerate(code):
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

            if num_paren > 0:
                body += ch

            if num_paren == 0 and ch == ';': # single line loop (no curly brackets)
                # print('a\n', loop)
                loops.append(loop)
                break

            if ch == '{':
                num_paren += 1
                state = State.for_body_found
            elif  ch == '}':
                num_paren -= 1

            if num_paren == 0 and state == State.for_body_found:
                # print('c\n', loop)
                loops.append(loop)
                extract_for(body, is_nested=True)
                break

    if not is_nested and len(code) > 0:
        # print('d\n', code[idx + 1:])
        extract_for(code[idx + 1:])
        
        




with open('/home/talkad/Downloads/thesis/data_gathering_script/asd/1/canny.cpp', 'r') as f:
    code = f.read()
    code = LINE_COMMENT_RE.sub('\n', code)
    code = MULTILINE_COMMENT_RE.sub('', code)

    # extract_for(code)
    # parser = CParser()
    # for loop in loops:

    #     try:
    #         ast = parser.parse(loop)
    #     except pycparser.plyparser.ParseError as e: 
    #         print('aaa')
    #         print(loop)
    #         print(e)
    #         # print(ast)
    # # print(loops)


src = r'''

    void f(char * restrict joe){}

int main(void)
{
    unsigned int long k = 4;
    int p = - - k;
    return 0;
}
'''

src = r'''
int main(void)
{
for (int row = 0; row < height; row++)
  function("hello world");
}
'''

parser = CParser()
ast = parser.parse(src)
ast.show()
        

