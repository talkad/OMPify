import os
import re
from enum import Enum
from parsers.parser import *
from parsers.parsing_utils import utils
from parsers.visitors import *
import pycparser
from pycparser.c_parser import CParser
from pycparser.c_ast import For, Pragma
from multiprocessing import Process, Manager
from parsers.fake_headers import fake
import json
import tempfile


LINE_COMMENT_RE = re.compile("//.*?\n")
MULTILINE_COMMENT_RE = re.compile("/\*.*?\*/", re.DOTALL)
DIRECTIVES_RE = re.compile("^\s*#(ifdef|ifndef|if|elif|else|endif).*$", re.MULTILINE)


def convert(match_obj):
    '''
    regular expression costum conversion
    '''
    num_groups = len(match_obj.groups())
    return match_obj.group(num_groups)


class State(Enum):
    '''
    Enum represets the state of the for loops extraction
    '''
    start = 1
    for_stmt = 2
    for_body = 3
    for_body_found = 4


class LoopExtractor:
    '''
    Implements the for-loop extraction functionality 
    '''
    def __init__(self):
        self.loops = []
        self.omp_pragmas = []

    def skip_lines(self, code):
        '''
        skip lines that are not part of a for loop
        '''
        pragma = None
        code_lines = code.split('\n')

        for idx, line in enumerate(code_lines):
            if utils.is_for_pragma(line):
                pragma =  line
            elif line.lstrip().startswith('for'):
                code = '\n'.join(code_lines[idx:])
                return code, pragma

        return '', None

    def extract_loops(self, code):
        '''
        Extract all loops and pragmas from given program by parentheses balancing
        '''      
        while len(code) > 0: 
            idx = 0
            code, pragma = self.skip_lines(code)

            loop = ''
            body = ''

            num_paren = 0
            num_braces = 0
            state = State.start

            # extract next loop
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
                    body += ch

                    if num_braces == 0 and num_paren == 0 and ch == ';': # single line loop (no curly brackets)
                        self.loops.append(loop)
                        self.omp_pragmas.append(pragma)
                        self.extract_loops(body)
                        break

                    if ch == '(':
                        num_paren += 1
                    elif ch == ')':
                        num_paren -= 1

                    if ch == '{':
                        num_braces += 1
                        state = State.for_body_found
                    elif  ch == '}':
                        num_braces -= 1

                    if num_braces == 0 and num_paren == 0 and state == State.for_body_found:
                        self.loops.append(loop)
                        self.omp_pragmas.append(pragma)
                        self.extract_loops(body)
                        break

                idx += 1

            code = code[idx + 1:]


class CppLoopParser(Parser):
    def __init__(self, repo_path, parsed_path):
        super().__init__(repo_path, parsed_path, ['.cpp'])

    def is_empty_loop(self, node):
        '''
        precondition - node is a For struct
        '''
        children = dict(node.children())

        try:
            # if there is no 'block_items' attribute than it's another compound struct
            temp = children['stmt'].block_items
        except:
            return False

        if children['stmt'].block_items is None:
            return True
        elif all(type(child) is For for child in children['stmt'].block_items):
             # return true if one of the for loops is empty
            return any(self.is_empty_loop(child) for child in children['stmt'].block_items)
        else:
            return False

    def create_ast(self, file_path, code_buf, result):
        with open('ENV.json', 'r') as f:
            vars = json.loads(f.read())

        cpp_args = ['-nostdinc', '-w', '-E', r'-I' + vars["FAKE_DIR_CPP"]]

        try:
            with tempfile.NamedTemporaryFile(suffix='.c', mode='w+') as tmp:    
                code_buf = '#include \"_fake_typedefs_cpp.h\"\n\n' + 'int main() {\n' + code_buf + '\n}'
                tmp.write(code_buf)
                tmp.seek(0)
                ast = pycparser.parse_file(tmp.name, use_cpp=True, cpp_path='mpicc', cpp_args = cpp_args)
                result['ast'] = ast.ext[-1].body.block_items[0]

        except pycparser.plyparser.ParseError as e:  
            # utils.log('error_logger.txt', f'Parser Error: {file_path} ->\n {e}\n')
            pass
        except Exception as e:  
            # utils.log('error_logger.txt', f'Unexpected Error: {file_path} ->\n {e}\n')
            pass

    def parse(self, file_path, code_buf):
        manager = Manager()
        return_dict = manager.dict()
        t = Process(target=self.create_ast, args=(file_path, code_buf, return_dict), daemon=True)

        t.start()
        t.join(60.0)

        if t.is_alive():
            t.terminate()
        elif 'ast' in return_dict:
            return return_dict['ast']


    def parse_file(self, root_dir, file_name, exclusions):
        '''
        Extract for loops from code and parse them into AST
        '''
        pos, neg = 0, 0
        file_path = os.path.join(root_dir, file_name)
        save_dir = os.path.join(self.parsed_path, root_dir[self.split_idx: ])
        name = os.path.splitext(file_name)[0]

        extractor = LoopExtractor()
        verify_loops = ForLoopChecker()
        func_call_checker = FuncCallChecker()

        with open(file_path, 'r+') as f:
            
            try:
                code = f.read()
            except UnicodeDecodeError:
                return 0, 0, False

            code = LINE_COMMENT_RE.sub('\n', code)
            code = MULTILINE_COMMENT_RE.sub('', code)

            extractor.extract_loops(code)

            pragmas = list(map(lambda pragma: None if pragma is None else Pragma(pragma), extractor.omp_pragmas))
            loops = list(map(lambda loop: self.parse(file_path, loop), extractor.loops))

            for idx, (pragma, loop) in enumerate(zip(pragmas, loops)):

                verify_loops.reset()
                func_call_checker.reset()

                if loop is None:
                    # attemp to parse again - this time after updating the code
                    code = re.sub(r"(((\w|<|>)+)::)+(\w+)", convert, extractor.loops[idx])
                    code = DIRECTIVES_RE.sub('', code)
                    loop = self.parse(file_path, code)

                    if loop is None:
                        # log('fail.txt', f'file: {file_path}\nPragma: {pragmas[idx]}\n{extractor.loops[idx]}\n==========\n')
                        continue

                verify_loops.visit(loop)
                if verify_loops.found:  # undesired tokens found
                    exclusions['bad_case'] += 1
                    continue
                
                generator = pycparser.c_generator.CGenerator()
                code = generator.visit(loop)
                if code in self.memory:
                    exclusions['duplicates'] += 1
                    continue

                if self.is_empty_loop(loop):
                    exclusions['empty'] += 1
                    continue

                func_call_checker.visit(loop)
                if func_call_checker.found:
                    exclusions['func_calls'] += 1
                                   
                self.create_directory(save_dir) 
                self.memory.append(code)
                self.save(os.path.join(save_dir, f"{name}{'_neg_' if pragma is None else '_pos_'}{idx}.pickle"), pragma, loop, code)

                if pragma is None:
                    neg += 1
                else:
                    pos += 1

            return pos, neg, True




# parser = CppLoopParser('../repositories_openMP', '../cpp_loops')
# parser = CppLoopParser('../asd', 'c_loops2')

# data = parser.load('/home/talkad/Downloads/thesis/data_gathering_script/cpp_loops/0scari/Parralel-programmin-pratice/p5/ompbase_neg_0.pickle')
# print(f'pragma: {data.omp_pragma}')
# print('code:\n')
# print(data.textual_loop)

# total = parser.scan_dir()
# print(total)

# removing compiler directives
# aaaaa 39233 284367
# (8095, 30294, {'bad_case': 2755, 'empty': 1692, 'duplicates': 202330, 'func_calls': 20416}, 14421, 248)

#without...
# pos examples:             8084   |   neg examples:             30281
# exclusions: {'bad_case': 2752, 'empty': 1692, 'duplicates': 202302, 'func_calls': 20396}