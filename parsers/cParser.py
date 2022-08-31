import os
import pycparser
from parser import Parser
from pycparser.c_ast import For
import pickle
from visitors import *
from functools import reduce
from fake_headers import fake
import re
import tempfile
from multiprocessing import Process, Manager
import tempfile



def log(file_name, msg):
    with open(file_name, 'a') as f:
        f.write(f'{msg}\n')


class CLoopParser(Parser):
    def __init__(self, repo_path, parsed_path):
        super().__init__(repo_path, parsed_path, ['.c', '.h'])

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
        repo_name = file_path[len(self.repo_path + self.root_dir) + 2:]
        repo_name = repo_name[:repo_name.find('/') ]
        cpp_args = ['-nostdinc', '-w', '-E', r'-I' + os.path.join(self.root_dir, 'fake_headers', 'utils')]

        _, headers, _ = fake.get_headers(fake.REPOS_DIR, repo_name)
        for header in list(headers)[:150]:
            cpp_args.append(r'-I' + os.path.join(fake.REPOS_DIR, repo_name, header))

        try:
            with tempfile.NamedTemporaryFile(suffix='.c', mode='w+') as tmp, open(file_path, 'r') as f:    
                code = f.read() 
                tmp.write(code)
                tmp.seek(0)
                ast = pycparser.parse_file(tmp.name, use_cpp=True, cpp_path='mpicc', cpp_args = cpp_args)
                result['ast'] = ast

        except pycparser.plyparser.ParseError as e:  
            log('error_logger.txt', f'Parser Error: {file_path} ->\n {e}\n')
            return
        except Exception as e:  
            # log('error_logger.txt', f'Unexpected Error: {file_path} ->\n {e}\n')
            return   

    def parse(self, file_path, code_buf):
        manager = Manager()
        return_dict = manager.dict()
        t = Process(target=self.create_ast, args=(file_path, code_buf, return_dict), daemon=True)

        t.start()
        t.join(60.0)

        if t.is_alive():
            t.terminate()
            return
        elif len(return_dict) == 0:
            return
        else:
            return return_dict['ast']


    def parse_file(self, root_dir, file_name, exclusions):
        '''
        Parse the given file into ast and extract to loops associated with omp pargma (or without)
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

            ast = self.parse(file_path, code)

            if ast is None:                 # file parsing failed
                return 0, 0, False

            pfv.visit(ast)
            pragmas = pfv.pragmas + len(pfv.neg_nodes) * [None]
            nodes = pfv.pos_nodes + pfv.neg_nodes

            for idx, (pragma, loop) in enumerate(zip(pragmas, nodes)):
                verify_loops.reset()
                func_call_checker.reset()

                verify_loops.visit(loop)
                if verify_loops.found:  # undesired     tokens found
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

    def is_cpp_header(self, file_path):
        code = ''
        includes = fake.extract_includes(file_path)

        if any([True for include in includes if not include.endswith('.h')]):
            return True

        try:
            with open(file_path, 'r') as f:
                code = f.read()
        except:
            return False
        
        return any([True for line in code.split('\n') if 'using' in line.lower() and 'namespace' in line.lower()])


    def scan_dir(self):
        LOGGER = 'cpp_headers.txt'

        total_files, num_failed = 0, 0
        total_pos, total_neg = 0, 0
        omp_repo = os.path.join(self.root_dir, self.repo_path)
        exclusions = {'bad_case': 0, 'empty': 0, 'duplicates': 0, 'func_calls':0}

        # iterate over repos
        for idx, repo_name in enumerate(os.listdir(omp_repo)):
            
            for root, dirs, files in os.walk(os.path.join(omp_repo, repo_name)):
                for file_name in files:
                    file_path = os.path.join(root, file_name)
                    ext = os.path.splitext(file_name)[1].lower()
                    
                    if ext in self.file_extensions:
                        if ext == '.h' and self.is_cpp_header(file_path):
                            log('cpp_header.txt', file_path)
                            continue

                        pos, neg, is_parsed = self.parse_file(root, file_name, exclusions)

                        if pos is not None:
                            total_pos += pos
                            total_neg += neg

                        if not is_parsed:
                            num_failed += 1

                            if ext == '.h':
                                log('cpp_header.txt', file_path)

                        total_files += 1

            if idx % (5) == 0:
                log('success_logger.txt', "{:20}{:10}   |   {:20} {:10}\n\n".format("files processed: ", total_files, "failed to parse: ", num_failed))
                print("{:20}{:10}   |   {:20} {:10}".format("files processed: ", total_files, "failed to parse: ", num_failed))
                print("{:20}{:10}   |   {:20} {:10}".format("pos examples: ", total_pos, "neg examples: ", total_neg))
                print(f'exclusions: {exclusions}\n')

        return total_pos, total_neg, exclusions, total_files, num_failed


parser = CLoopParser('../repositories_openMP', '../c_loops')
# parser = CLoopParser('../asd', 'c_loops2')

# data = parser.load('/home/talkad/Downloads/thesis/data_gathering_script/c_loops/357r4bd/2d-heat/src/openmp-2dheat_pos_0.pickle')
# print(f'pragma: {data.omp_pragma}')
# print('code:\n')
# print(data.textual_loop)


total = parser.scan_dir()
print(total)

# (13238, 36593, {'bad_case': 6385, 'empty': 200, 'duplicates': 127647, 'func_calls': 22421}, 20803, 3861)
