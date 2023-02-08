import re

redundant_line_comments = re.compile("\/\/.*")
redundant_multiline_comments = re.compile("\/\*.*?\*\/", re.MULTILINE|re.DOTALL)


class CleanCode:

    def join_splited_lines(self, code_buf, delimiter='\\'):
        code = []
        splitted_line = False

        for line in code_buf.split('\n'):
            if not splitted_line and len(line) > 0 and line[-1] == delimiter:
                code.append(line[:-1])
                splitted_line = True
            elif splitted_line and len(line) > 0 and line[-1] == delimiter:
                code[-1] += line[:-1]
            elif splitted_line:
                code[-1] += line
                splitted_line = False
            else:
                code.append(line)

        return '\n'.join(code)

    def remove_comments(self, code):
        code = redundant_line_comments.sub("\n", code)
        code = redundant_multiline_comments.sub("\n", code)
        return code

    def remove_empty_lines(self, code):
        return '\n'.join(filter(lambda line: len(line.lstrip()) > 0 , code.split('\n')))

    def skip_pragma(self, code_buf):
        for idx, line in enumerate(code_buf):
            if not line.lstrip().lower().startswith('#pragma'):
                return '\n'.join(code_buf[:idx]), idx

        return '\n'.join(code_buf[:idx]), idx

    def skip_next_statement(self, code_buf):
        start = False
        num_braces = 0

        if not any(code_buf[0].lstrip().lower().startswith(struct) for struct in ['for', 'if', 'while']):
            return code_buf[0], 1
        else:
            # print('aaaaaa', code_buf)

            # find next line ends with semicolon
            open_bracket = False

            for idx, line in enumerate(code_buf):
                if line.lstrip().endswith('{'):
                    open_bracket = True
                if line.rstrip().endswith(';'):
                    break

            # either its a multiline struct block or single line
            if not open_bracket:
                # print('cccc', idx)
                return '\n'.join(code_buf[:idx+1]), idx+1
            else:

                for idx, line in enumerate(code_buf):
                    if start and num_braces == 0:
                        # print('dddd', idx)
                        return '\n'.join(code_buf[:idx+1]), idx+1
                    
                    num_braces += line.count('{') - line.count('}')
                    if num_braces > 0:
                        start = True

        return '\n'.join(code_buf), len(code_buf)

    def add_curly_braces(self, code):
        updated_code = []
        code_buf = code.split('\n')

        idx= 0

        while idx < len(code_buf):
            line = code_buf[idx]
            updated_code.append(line)

            if line.lstrip().lower().startswith('for'):
                if not line.rstrip().endswith('{') and not code_buf[idx+1].lstrip().startswith('{'):
                    updated_code.append('{')
                    pragma, length = self.skip_pragma(code_buf[idx+1:])
                    # print('bb', pragma, length)
                    idx += length
                    updated_code.append(pragma)

                    code_statement, length = self.skip_next_statement(code_buf[idx+1:])
                    # print(code_statement)
                    updated_code.append(self.add_curly_braces(code_statement))
                    updated_code.append('}')
                    idx += length

            idx += 1

        return '\n'.join(updated_code)
                

class Injector:
    '''
    inject patches to a program, to easily detect the for loops after conversion using Clang+LLVM.
    also for preserving the OpenMP pragmas in the code.
    '''
    def __init__(self, lang='C'):
        self.lang = lang
        self.cleaner = CleanCode()

    def wrap_pragma(self, code):
        updated_code = []
        pragma_func = 'omp_pragma_talkad7420'

        for line in code.split('\n'):
            l = line.lstrip().lower()

            if l.startswith('#pragma') and ' omp' in l:
                updated_code.append(f'{pragma_func}(\"{l}\");')
            else:
                updated_code.append(line)

        return '\n'.join(updated_code)

    def mark_while_loop(self, code):
        updated_code = []
        pragma_func = 'while_loop_talkad7420();'

        for line in code.split('\n'):
            l = line.lstrip().lower()

            if l.startswith('while') or l.startswith('do ') or l.startswith('do{'):
                updated_code.append(pragma_func)
            updated_code.append(line)

        return '\n'.join(updated_code)

    def clean_code(self, code):
        code = self.cleaner.join_splited_lines(self.cleaner.remove_comments(code))
        return self.cleaner.add_curly_braces(self.cleaner.remove_empty_lines(code))


    def inject(self, code):
        code = self.clean_code(code)
        code = self.mark_while_loop(code)
        return  self.cleaner.remove_empty_lines(self.wrap_pragma(code))






def is_next_curly(code):
    '''
    for a given segment of code, return true if next char is curly bracket
    '''
    return code.lstrip().startswith('{')

def skip_struct_block(code):
    '''
    for a given code, return the index of the closing bracket of the next structure block
    '''
    found = False
    num_braces = 0

    for idx, ch in enumerate(code):
        if found and num_braces == 0:
            return idx

        if ch == '{':
            found = True
            num_braces += 1
        elif ch == '}':
            num_braces -= 1

    return -1

# def add_curly_braces(code):
#     code_buf = code.split('\n')
#     code_res = []
#     num_braces = 0
#     found_bracket = False
#     close_next_line = False

#     for idx, line in enumerate(code_buf):      
#         code_res.append(line)
#         l = line.lstrip()

#         if l.startswith('for ') or l.startswith('for('):
#             updated_code = '\n'.join(code_buf[idx:])[line.rfind(')')+1:]

#             if not is_next_curly(updated_code):
#                 num_braces += 1
#                 code_res.append('{')
#             else:
#                 num_braces += 1
#                 found_bracket = True

#         # elif any((l.startswith(f'{keyword} ') or l.startswith(f'{keyword}(')) for keyword in ['do', 'while', 'if']):
#         #     updated_code = '\n'.join(code_buf[idx:])[line.rfind(')')+1:]

#         #     if is_next_curly(updated_code):
#         #         # skip and recursion
#         #     else:
#         #         close_next_line = True
#         #         # consider 
#         #         # if()
#         #         #     if()
#         #         #         f();


def find_statement(code):
    code_buf = code.split('\n')
    
    for idx, line in enumerate(code_buf):
        if line.rstrip().endswith(';'):
            return idx

    return -1


def balance_bracket_idx(code):
    found = False
    code_buf = code.split('\n')
    bracket_amount = 0

    for idx, line in enumerate(code_buf):
        bracket_amount += line.count('{')
        bracket_amount -= line.count('}')

        if bracket_amount > 0:
            found = True
        elif found and bracket_amount == 0:
            return idx

    return -1


def find_closing_bracket(code, num):
    found = False
    code_buf = code.split('\n')
    bracket_amount = num

    for idx, line in enumerate(code_buf):
        bracket_amount += line.count('{')
        bracket_amount -= line.count('}')

        if bracket_amount == 0:
            return idx

    return -1



def add_curly_braces(code):
    code_buf = code.split('\n')
    idx = 0
    code_res = []


    while idx < len(code_buf):  
        # print('aaaaaaaaaaaaaaaaaaaaaaaaaaa', idx)
        line = code_buf[idx]   
        code_res.append(line)
        l = line.lstrip()

        if l.startswith('for ') or l.startswith('for('):
            current_code = '\n'.join(code_buf[idx:])
            updated_code = current_code[line.rfind(')')+1:]

            if is_next_curly(updated_code):
                closing_idx = balance_bracket_idx(current_code)
                # print('a', closing_idx)

                if closing_idx > 0:
                    # recursion
                    code_res.append(add_curly_braces('\n'.join(code_buf[idx+1: idx+closing_idx])))

                    # code_res += code_buf[idx:idx+closing_idx]
                    # code_res += ['aaaaaaaaa;']
                    idx += closing_idx-1
            else:
                code_res.append('{')
                next_statement = find_statement(current_code)

                # print(f'=========={current_code}==========')
                # print('b', next_statement)
                num_braces = '\n'.join(code_buf[idx: idx+next_statement]).count('{')
                closing_idx = find_closing_bracket('\n'.join(code_buf[idx+next_statement:]), num_braces)
                # print('\n'.join(code_buf[idx+next_statement:]))
                # print('c', num_braces, closing_idx)

                # recursion
                code_res.append(add_curly_braces('\n'.join(code_buf[idx+1: idx+closing_idx+next_statement+1])))

                # code_res += code_buf[idx:idx+closing_idx]
                # code_res += ['bbbbbbbbbbb;']
                idx += closing_idx+next_statement
                code_res.append('}')

        idx += 1

    return '\n'.join(code_res)



# code = """

# for()
# for()
#     if(){
#         f;}
        


# """


code = """
/* FILE: omp_bug2.c
* DESCRIPTION:
*   This example attempts to show use of the parallel for construct.  However
*   it will generate errors at compile time.  Try to determine what is causing
*   the error.
******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define N       50
#define CHUNKSIZE   5

int main (int argc, char *argv[])
{
int i, chunk, tid;
float a[N], b[N], c[N];

/* Some initializations */
for (i=0; i < N; i++){
  a[i] = b[i] = i * 1.0;
}

chunk = CHUNKSIZE;
 //Problem: If one uses 'for' a 'for' loop needs to follow. Schedule needs to follow the for statement.

#pragma omp parallel private(i,tid)\\
shared(a,b,c,chunk)
{
  tid = omp_get_thread_num();
  #pragma omp for schedule(static,chunk)
  for (i=0; i < N; i++)
  for (i=0; i < N; i++)
  #pragma omp for
  for (i=0; i < N; i++)
    c[i] = a[i] + b[i];
  }


}
"""




# code = code.replace('}', '\n}')


# injector = Injector()
print(add_curly_braces(code))

# res = is_next_curly(code)
# print(res)
