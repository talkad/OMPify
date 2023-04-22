from multiprocessing import Process, Manager
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

    def is_next_curly(self, code):
        return code.lstrip().startswith('{')

    def find_statement(self, code):
        code_buf = code.split('\n')
        
        for idx, line in enumerate(code_buf):
            if line.rstrip().endswith(';'):
                return idx

        return -1

    def balance_bracket_idx(self, code):
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


    def find_closing_bracket(self, code, num):
        found = False
        code_buf = code.split('\n')
        bracket_amount = num

        for idx, line in enumerate(code_buf):
            bracket_amount += line.count('{')
            bracket_amount -= line.count('}')

            if bracket_amount == 0:
                return idx

        return -1

    def add_curly_braces(self, code):
        code = code.replace('}', '\n}')
        code_buf = code.split('\n')
        idx = 0
        code_res = []


        while idx < len(code_buf):  
            line = code_buf[idx]   
            code_res.append(line)
            l = line.lstrip()

            if l.startswith('for ') or l.startswith('for('):
                current_code = '\n'.join(code_buf[idx:])
                updated_code = current_code[line.rfind(')')+1:]

                if self.is_next_curly(updated_code):
                    closing_idx = self.balance_bracket_idx(current_code)

                    if closing_idx > 0:
                        code_res.append( self.add_curly_braces('\n'.join(code_buf[idx+1: idx+closing_idx])))
                        idx += closing_idx-1
                else:
                    code_res.append('{')
                    next_statement =  self.find_statement(current_code)

                    num_braces = '\n'.join(code_buf[idx: idx+next_statement]).count('{')
                    closing_idx =  self.find_closing_bracket('\n'.join(code_buf[idx+next_statement:]), num_braces)

                    code_res.append(self.add_curly_braces('\n'.join(code_buf[idx+1: idx+closing_idx+next_statement+1])))
                    idx += closing_idx+next_statement
                    code_res.append('}')

            idx += 1

        return '\n'.join(code_res)


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
        pragma_for_func = 'omp_for_pragma_talkad7420'
        pragma_func = 'omp_pragma_talkad7420'

        for line in code.split('\n'):
            l = line.lstrip().lower()

            if l.startswith('#pragma') and ' omp' in l and ' for' in l:
                updated_code.append(f'{pragma_for_func}(\"{l}\");')
            elif l.startswith('#pragma') and ' omp' in l and  any([(f' {clause}' in l) for clause in ['atomic', 'barrier', 'critical']]):
                updated_code.append(f'{pragma_func}(\"{l}\");')
            else:
                updated_code.append(line)

        return '\n'.join(updated_code)

    def mark_for_loop(self, code):
        updated_code = []
        loop_func = 'for_loop_talkad7420();'

        for line in code.split('\n'):
            l = line.lstrip().lower()

            if l.startswith('for ') or l.startswith('for('):
                updated_code.append(loop_func)
            updated_code.append(line)

        return '\n'.join(updated_code)

    def clean_code(self, code):
        code = self.cleaner.join_splited_lines(self.cleaner.remove_comments(code))
        return self.cleaner.add_curly_braces(self.cleaner.remove_empty_lines(code))

    def pipeline(self, code, result):
        code = self.clean_code(code)
        code = self.mark_for_loop(code)
        result['code'] =  self.cleaner.remove_empty_lines(self.wrap_pragma(code))

    def inject(self, code):
        manager = Manager()
        return_dict = manager.dict()
        t = Process(target=self.pipeline, args=(code, return_dict), daemon=True)

        t.start()
        t.join(20.0)

        if t.is_alive():
            t.terminate()
            return ''
        elif 'code' in return_dict:
            return return_dict['code']
        else:
            return ''

