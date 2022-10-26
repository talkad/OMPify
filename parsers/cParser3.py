import os
import pycparser
from parser import Parser
from pycparser.c_ast import For
import pickle
from visitors import *
from functools import reduce
from fake_headers import fake
from parsing_utils import utils
import re
import json
import tempfile
from multiprocessing import Process, Manager
import tempfile

missed = 0

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

    def create_ast(self, file_path, code_buf, temp_header_path, result):
        with open('../ENV.json', 'r') as f:
            vars = json.loads(f.read())

        repo_name = file_path[len(self.repo_path + self.root_dir) + 2:]
        repo_name = repo_name[:repo_name.find('/') ]

        # define execution argument
        cpp_args = ['-nostdinc', '-w', '-E', r'-I' + vars["FAKE_DIR"], r'-I' + '/home/talkad/Downloads/thesis/data_gathering_script/asd/1']

        _, headers, _ = fake.get_headers(vars['REPOS_DIR'], repo_name)
        print('asdf', headers)

        for header in list(headers)[:150]:
            cpp_args.append(r'-I' + os.path.join(vars['REPOS_DIR'], repo_name, header))

        try:
            with tempfile.NamedTemporaryFile(suffix='.c', mode='w+') as tmp_file, open(file_path, 'r') as f:                
                code = f.read()
                tmp_file.write('#include {}\n'.format(temp_header_path[temp_header_path('/') + 1 :]))
                cpp_args.append(r'-I' + temp_header_path)

                tmp_file.write(code)
                tmp_file.seek(0)
                ast = pycparser.parse_file(tmp_file.name, use_cpp=True, cpp_path='mpicc', cpp_args = cpp_args)
                result['ast'] = ast

        except pycparser.plyparser.ParseError as e:  
            log('error_logger.txt', f'Parser Error: {file_path} ->\n {e}\n')
            return
        except Exception as e:
            log('error_logger.txt', f'Unexpected Error: {file_path} ->\n {e}\n')

            if str(e).startswith('Command'): # Capture failures caused by missing headers
                print(f'aaaaaaaaaaaaa {utils.count_for(file_path)} -> {file_path}')
                result['missed'] = utils.count_for(file_path)

            return   

    def parse(self, file_path, code_buf, opt=None):
        manager = Manager()
        return_dict = manager.dict()
        t = Process(target=self.create_ast, args=(file_path, code_buf, opt, return_dict), daemon=True)

        t.start()
        t.join(60.0)

        if t.is_alive():
            t.terminate()
            return
        elif len(return_dict) == 0:
            return
        elif 'missed' in return_dict:
            global missed
            missed += return_dict['missed']
        else:
            return return_dict['ast']


    def parse_file(self, root_dir, file_name, exclusions, opt=None):
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

            ast = self.parse(file_path, code, opt=opt)

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

        # loop_missed = 0

        # iterate over repos
        for idx, repo_name in enumerate(os.listdir(omp_repo)):
            
            # create fake header
            with tempfile.NamedTemporaryFile(suffix='.h', mode='w+') as tmp_header:
                fake.extract_typedef_directives(repo_name, tmp_header.name)
                
                for root, dirs, files in os.walk(os.path.join(omp_repo, repo_name)):
                    for file_name in files:
                        file_path = os.path.join(root, file_name)
                        ext = os.path.splitext(file_name)[1].lower()
                        
                        if ext in self.file_extensions:
                            if ext == '.h' and self.is_cpp_header(file_path):
                                log('cpp_header.txt', file_path)
                                continue

                            pos, neg, is_parsed = self.parse_file(root, file_name, exclusions, opt=tmp_header.name)
                            
                            # if not is_parsed:
                            #     loop_missed += utils.count_for(os.path.join(root, file_name))

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
                    
                    # print(f'loop missed {missed}')

        return total_pos, total_neg, exclusions, total_files, num_failed


# parser = CLoopParser('../repositories_openMP', '../c_loops2')
parser = CLoopParser('../asd', 'c_loops2')

# data = parser.load('/home/talkad/Downloads/thesis/data_gathering_script/c_loops/357r4bd/2d-heat/src/openmp-2dheat_pos_0.pickle')
# print(f'pragma: {data.omp_pragma}')
# print('code:\n')
# print(data.textual_loop)

parser.scan_dir()



# loop missed 20518!!!!!!!!!!!!