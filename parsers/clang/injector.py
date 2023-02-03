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




# code = """
# /* FILE: omp_bug2.c
# * DESCRIPTION:
# *   This example attempts to show use of the parallel for construct.  However
# *   it will generate errors at compile time.  Try to determine what is causing
# *   the error.
# ******************************************************************************/
# #include <omp.h>
# #include <stdio.h>
# #include <stdlib.h>
# #define N       50
# #define CHUNKSIZE   5

# int main (int argc, char *argv[])
# {
# int i, chunk, tid;
# float a[N], b[N], c[N];

# /* Some initializations */
# for (i=0; i < N; i++){
#   a[i] = b[i] = i * 1.0;
# }

# chunk = CHUNKSIZE;
#  //Problem: If one uses 'for' a 'for' loop needs to follow. Schedule needs to follow the for statement.

# #pragma omp parallel private(i,tid)\\
# shared(a,b,c,chunk)
# {
#   tid = omp_get_thread_num();
#   #pragma omp for schedule(static,chunk)
#   for (i=0; i < N; i++)
#     {
#     c[i] = a[i] + b[i];
#     printf("tid= %d i= %d c[i]= %f\n", tid, i, c[i]);
#     }
#   }


# }
# """



# injector = Injector()
# print(injector.inject(code))
