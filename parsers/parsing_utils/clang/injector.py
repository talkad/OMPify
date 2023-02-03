import re

redundant_line_comments = re.compile("\/\/.*")
redundant_multiline_comments = re.compile("\/\*.*\*\/", re.MULTILINE|re.DOTALL)


class CleanCode:

    def unite_lines(self, code):
        updated_code = []

        for idx, line in enumerate(code.split('\n')):
            if idx == 0:
                updated_code.append(line)
            elif line.rstrip().endswith('\\'):
                backslash_idx = line.rfind('\\')
                updated_code[-1] += "\n"+f"{line[:backslash_idx]}"
            else:
                updated_code.append(line)

        return '\n'.join(updated_code)

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

    def clean_code(self, code):
        code = self.cleaner.unite_lines(self.cleaner.remove_comments(code))
        return self.cleaner.add_curly_braces(self.cleaner.remove_empty_lines(code))


    def inject(self, code):
        code = self.clean_code(code)
        return  self.cleaner.remove_empty_lines(self.wrap_pragma(code))




# code = """
# #include <string>
# #include <omp.h>
# typedef int AA;



# int main()
# {
#         AA a;
#         int N = 10000;
#         int arr[N];

#         int sum = 0;

#         #pragma omp parallel for
#          for(int i = 0; i < N; i++)
#           #pragma omp parallel for  
#         for(int i = 0; i < N; i++)
#             if(arr[i] > 0)
#                 sum += arr[i];

#           sum = 1000;
# }
# """





# injector = Injector()
# print(injector.inject('/home/talkad/Downloads/thesis/data_gathering_script/asd/1/aa.cpp'))
