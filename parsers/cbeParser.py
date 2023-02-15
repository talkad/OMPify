import os
import pycparser
from parsers.parser import *
from parsers.parser import OmpLoop
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


class Preprocessor:
    def __init__(self):
        self.redundant_line = re.compile("#line \d+ \".*?\"")
        self.redundant_line_comments = re.compile("\/\/.*")
        self.redundant_multiline_comments = re.compile("\/\*.*?\*\/", re.MULTILINE|re.DOTALL)


    def remove_comments(self, code):
        code = self.redundant_line_comments.sub("\n", code)
        code = self.redundant_multiline_comments.sub("\n", code)
        return code

    def remove_line_mark(self, code):
        return self.redundant_line.sub("\n", code)

    def remove_empty_lines(self, code):
        updated_code = []

        for line in code.split('\n'):
            l = line.strip()

            if len(l) == 0 or l.startswith(';'):
                continue
            updated_code.append(line)

        return '\n'.join(updated_code) 


    def preprocess(self, code):
        process_func = lambda x: self.remove_comments(self.remove_empty_lines(self.remove_line_mark(x)))

        return process_func(code)


class CBELoopParser(Parser):
    def __init__(self, repo_path, parsed_path):
        super().__init__(repo_path, parsed_path, ['.c'])

    def is_empty_loop(self, node):
        '''
        return true if the loop is empty
        '''
        empty_checker = EmptyDoVisitor()
        empty_checker.visit(node)

        return empty_checker.is_empty


    def create_ast(self, file_path, code, result):
        with open('ENV.json', 'r') as f:
            vars = json.loads(f.read())

        repo_name = file_path[len(self.repo_path) + 2:]
        repo_name = repo_name[:repo_name.find('/') ]
        cpp_args = [ '-w', '-E'] # , r'-I' + vars["FAKE_DIR"]] '-nostdinc',
        _, headers, _ = fake.get_headers(vars['REPOS_DIR'], repo_name)

        if os.path.exists(dest_folder):
            shutil.rmtree(dest_folder)

        # create empty headers
        os.makedirs(dest_folder)
        fake.create_empty_headers(file_path, dest_folder)
        # cpp_args.append(r'-I' + dest_folder)

        # for header in list(headers)[:50]:
        #     cpp_args.append(r'-I' + os.path.join(vars['REPOS_DIR'], repo_name, header))
        
        try:
            with tempfile.NamedTemporaryFile(suffix='.c', mode='w+') as tmp:    
                tmp.write(code)
                tmp.seek(0)
                # ast = pycparser.parse_file(tmp.name, use_cpp=True, cpp_path='mpicc', cpp_args = cpp_args)
                ast = pycparser.parse_file(tmp.name, use_cpp=True, cpp_args = cpp_args)
                result['ast'] = ast

        except pycparser.plyparser.ParseError as e:  
            # utils.log('error_logger.txt', f'Parser Error: {file_path} ->\n {e}\n')
            print(e)
        except Exception as e:
            # utils.log('error_logger.txt', f'Unexpected Error: {file_path} ->\n {e}\n')
            print(e)

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
            try:
                shutil.rmtree(dest_folder)
            except:
                return
        elif 'ast' in return_dict:
            return return_dict['ast']

    def extract_func_defs(self, func_calls, func_defs):
        '''
        Return a list of all func defs appearing in func calls
        '''
        def extract_func_name(func_call):
            try:
                return func_call.name.name
            except:
                return ""
        func_names = list(map(lambda func_call: extract_func_name(func_call), func_calls))
        return [func_def for func_def in func_defs if func_def.decl.name in func_names]
        
    def parse_file(self, root_dir, file_name, exclusions):
        '''
        Parse the given file into ast and extract the loops associated with omp pargma (or without)
        '''
        indexer = 0
        pos, neg = 0, 0
        pragma_found = 0
        count_no_pragma = 0
        error = 'missing pragmas'

        file_path = os.path.join(root_dir, file_name)
        log('files.txt', file_path)
        _, pragma_amount = utils.count_for(file_path)
        save_dir = os.path.join(self.parsed_path, root_dir[self.split_idx: ])
        name = os.path.splitext(file_name)[0]

        preprocessor = Preprocessor()
        cbev = CbeDoVisitor()
        verify_loops = DoLoopChecker()
        func_call_checker = FuncCallChecker()
        func_defs_extractor = FuncDefVisitor()

        with open(file_path, 'r+') as f:
            # print(file_path)
            try:
                code = f.read()
            except UnicodeDecodeError:
                utils.log("fail_pragma.txt", f'{file_path}\nUnicodeDecodeError\nfound {pragma_found} | there are {pragma_amount}\n===================')
                return 0, 0, False
            
            code = preprocessor.preprocess(code)
            code = utils.update_code_cbe_pipline(code)    # remove unparsable code
            ast = self.parse(file_path, code)

            if ast is None:                 # file parsing failed
                return 0, 0, False

            cbev.visit(ast)
            pragma_found = len(cbev.pos_nodes)
            pragmas = len(cbev.pos_nodes) * [True] + len(cbev.neg_nodes) * [False]
            nodes = cbev.pos_nodes + cbev.neg_nodes
            func_defs_extractor.visit(ast)

            if pragma_found < pragma_amount:
                return 0, 0, False

            for idx, (pragma, loop) in enumerate(zip(pragmas, nodes)):
                verify_loops.reset()
                func_call_checker.reset()

                generator = pycparser.c_generator.CGenerator()
                code = generator.visit(loop)

                verify_loops.visit(loop)
                if verify_loops.found:  # undesired tokens found
                    exclusions['bad_case'] += 1
                    continue

                if code in self.memory:
                    exclusions['duplicates'] += 1
                    continue

                if self.is_empty_loop(loop):
                    exclusions['empty'] += 1
                    continue

                func_call_checker.visit(loop)
                if func_call_checker.found:
                    exclusions['func_calls'] += 1
            
                saving_path = os.path.join(save_dir, name, str(indexer))
                self.create_directory(saving_path) 
                self.memory.append(code)

                relevant_func_defs = self.extract_func_defs(func_call_checker.func_calls, func_defs_extractor.func_def)
                self.save(saving_path, OmpLoop(None if not pragma else '#pragma omp for', loop, relevant_func_defs, code))
                indexer += 1

                if not pragma:
                    neg += 1
                else:
                    pos += 1

            return pos, neg, True





# p = Preprocessor()

# # with open('/home/talkad/OpenMPdb/asd/1/main.cbe.c' , 'r') as f:
# #     print(p.preprocess(f.read()))


# with open('/home/talkad/OpenMPdb/asd/1/main.cbe.c', 'r') as f:
#     code = f.read()
#     code = p.preprocess(code)

# with tempfile.NamedTemporaryFile(suffix='.c', mode='w+') as tmp:    
#     tmp.write(code)
#     tmp.seek(0)

#     ast = pycparser.parse_file(tmp.name) # , use_cpp=True, cpp_path='mpicc', cpp_args = [])
#     print(ast)