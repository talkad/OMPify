import os
import pycparser
from parsers.parser import Parser
from pycparser.c_ast import For
import pickle
from parsers.visitors import *
from functools import reduce
from parsers.fake_headers import fake
from parsers.parsing_utils import utils
import re
import json
from multiprocessing import Process, Manager
import tempfile
import shutil


dest_folder = 'temp_folder'


def handle_error(file_path, err, code):
    '''
    Extracts from a given error message the undefined type
    '''
    err_pattern = {'*': r'(\w+)\W*$',
                    '=': r'^\W*(\w+)'}

    match = re.search(r'.*:(\d+):(\d+): before: (.*)', err)
    code_buf = code.split('\n')

    if match is None:
        return
    
    line, pos, param = int(match.group(1)), int(match.group(2)), match.group(3)
    sub_line = code_buf[line-1][: pos-1]
    param = param.strip()

    code_segment = "\n".join(code_buf[line-2:line+1])
    utils.log('more_errors.txt', f'{file_path}\n{err}\n{code_segment}\n =========================\n')

    if param in err_pattern:
        match = re.search(err_pattern[param], sub_line)

        if match is not None:
            # log('typedefs.h', f'typedef int {match.group(1)};')
            return match.group(1)
    # else:
        # code_segment = "\n".join(code_buf[line-2:line+1])
        # log('more_errors.txt', f'{err}\n{code_segment}')

        
class CLoopParser(Parser):
    def __init__(self, repo_path, parsed_path):
        super().__init__(repo_path, parsed_path, ['.c'])

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


    def create_ast(self, file_path, code, result):
        with open('ENV.json', 'r') as f:
            vars = json.loads(f.read())

        repo_name = file_path[len(self.repo_path + self.root_dir) + 2:]
        repo_name = repo_name[:repo_name.find('/') ]
        cpp_args = ['-nostdinc', '-w', '-E', r'-I' + vars["FAKE_DIR"]]

        _, headers, _ = fake.get_headers(vars['REPOS_DIR'], repo_name)

        # create empty headers
        os.makedirs(dest_folder)
        fake.create_empty_headers(file_path, dest_folder)
        cpp_args.append(r'-I' + dest_folder)

        for header in list(headers)[:50]:
            cpp_args.append(r'-I' + os.path.join(vars['REPOS_DIR'], repo_name, header))

        try:
            with tempfile.NamedTemporaryFile(suffix='.c', mode='w+') as tmp:    
                tmp.write(code)
                tmp.seek(0)
                ast = pycparser.parse_file(tmp.name, use_cpp=True, cpp_path='mpicc', cpp_args = cpp_args)
                result['ast'] = ast

        except pycparser.plyparser.ParseError as e:  
            # utils.log('error_logger.txt', f'Parser Error: {file_path} ->\n {e}\n')
            handle_error(file_path, str(e), code)

        except Exception as e:
            # utils.log('error_logger.txt', f'Unexpected Error: {file_path} ->\n {e}\n')
            pass

        finally:
            shutil.rmtree(dest_folder)

    def parse(self, file_path, code_buf):
        manager = Manager()
        return_dict = manager.dict()
        t = Process(target=self.create_ast, args=(file_path, code_buf, return_dict), daemon=True)

        t.start()
        t.join(120.0)

        if t.is_alive():
            t.terminate()
            shutil.rmtree(dest_folder)
            return
        elif 'ast' in return_dict:
            return return_dict['ast']

    def parse_file(self, root_dir, file_name, exclusions):
        '''
        Parse the given file into ast and extract the loops associated with omp pargma (or without)
        '''
        pos, neg = 0, 0
        file_path = os.path.join(root_dir, file_name)
        save_dir = os.path.join(self.parsed_path, root_dir[self.split_idx: ])
        name = os.path.splitext(file_name)[0]

        pfv = PragmaForVisitor()
        verify_loops = ForLoopChecker()
        func_call_checker = FuncCallChecker()

        with open(file_path, 'r+') as f:
            
            try:
                code = f.read()
            except UnicodeDecodeError:
                return 0, 0, False

            code = utils.update_code_pipline(code)    # remove unparsable code
            asts = list(map(lambda code_permutation: self.parse(file_path, code_permutation), utils.get_if_permutations(code)))

            for copy_idx, ast in enumerate(asts):
                if ast is None:                 # file parsing failed
                    return 0, 0, False

                pfv.visit(ast)
                pragmas = pfv.pragmas + len(pfv.neg_nodes) * [None]
                nodes = pfv.pos_nodes + pfv.neg_nodes

                for idx, (pragma, loop) in enumerate(zip(pragmas, nodes)):
                    verify_loops.reset()
                    func_call_checker.reset()

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
                    self.save(os.path.join(save_dir, f"{name}{'_neg_' if pragma is None else '_pos_'}{idx}_{copy_idx}.pickle"), pragma, loop, code)

                    if pragma is None:
                        neg += 1
                    else:
                        pos += 1

            return pos, neg, True

 
# files processed:         19778   |   failed to parse:           1664
# pos examples:            13893   |   neg examples:             37521
# exclusions: {'bad_case': 1261656, 'empty': 2779, 'duplicates': 25767368, 'func_calls': 23763}
