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


INCLUDES_RE = re.compile("^#include(.+)$", re.MULTILINE)


class CLoopParser(Parser):
    def __init__(self, repo_path, parsed_path):
        super().__init__(repo_path, parsed_path, ['.c', '.h'])

    def join_funcDecl(self, code):
        '''
        Several c files define the function return type in separate line. For instance:
            int
            check_omp_lock (FILE * logFile) {...}
        pycparser fail to process this files. So we join this lines to proper func. declaration.
        '''
        return_types = ['char', 'short', 'int', 'long', 'float', 'double']

        return reduce(lambda acc, cur: f'{acc}\n{cur}' if cur.lstrip() in return_types else f'{acc} {cur}\n', code.split('\n'))

    def update_include(self, file_path):
        with open(file_path, "r+") as f:
            code = f.read()
            code = INCLUDES_RE.sub("", code)
            code = f'#include \"_fake_handcrafted.h\"\n#include \"{fake.COMMON_FAKE_DEFINES}\"\n#include \"{fake.COMMON_FAKE_TYPEDEFS}\"\n#include \"{fake.FAKE_DEFINES}\"\n#include \"{fake.FAKE_TYPEDEFS}\"\n{code}'

            # f.truncate(0)
            # f.seek(0)
            # f.write(code)           
        return code

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

    def code_preprocess_pipline(self, code):
        code = self.join_splited_lines(code)
        return self.join_funcDecl(code)

    def parse(self, file_path, code_buf):
        repo_name = file_path[len(self.repo_path + self.root_dir) + 2:]
        repo_name = repo_name[:repo_name.find('/') ]
        cpp_args = ['-nostdinc',  '-E', r'-I' + os.path.join(self.root_dir, 'fake_headers', 'utils')]

        code = self.update_include(file_path)

        try:
            with tempfile.NamedTemporaryFile(suffix='.c', mode='w+t') as tmp:
                # for idx, line in enumerate(code.split('\n')):
                #     print(idx, line)
                tmp.writelines(code)
                tmp.seek(0)
                ast = pycparser.parse_file(tmp.name, use_cpp=True, cpp_path='mpicc', cpp_args = cpp_args)
                # generator = pycparser.c_generator.CGenerator()
                # print(generator.visit(ast))
                return ast
        except pycparser.plyparser.ParseError as e:  
            print(f'Parser Error: {file_path} ->\n {e}')
            for idx in re.findall(r'(.*?):(\d+):(\d+)(.*)', str(e)):
                print("line:", code.split('\n')[int(idx[1]) - 1])
            return
        except Exception as e:  
            print(f'Unexpected Error: {file_path} ->\n {e}')
            return

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
                if verify_loops.found:  # undesired tokens found
                    exclusions['bad_case'] += 1
                    continue

                code = str(loop)
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

                generator = pycparser.c_generator.CGenerator()
                self.save(os.path.join(save_dir, f"{name}{'_neg_' if pragma is None else '_pos_'}{idx}.pickle"), pragma, loop, generator.visit(loop))

                if pragma is None:
                    neg += 1
                else:
                    pos += 1

            return pos, neg, True

    def scan_dir(self):
        total_files, num_failed = 0, 0
        total_pos, total_neg = 0, 0
        omp_repo = os.path.join(self.root_dir, self.repo_path)
        exclusions = {'bad_case': 0, 'empty': 0, 'duplicates': 0, 'func_calls':0}

        # iterate over repos
        for idx, repo_name in enumerate(os.listdir(omp_repo)):
            fake.extract_all_directives(repo_name)
            _, headers = fake.get_headers(fake.REPOS_DIR, repo_name)

            for root, dirs, files in os.walk(os.path.join(omp_repo, repo_name)):
                for file_name in files:
                    ext = os.path.splitext(file_name)[1].lower()
                    
                    if ext in self.file_extensions:

                        includes = fake.extract_includes(os.path.join(root, file_name))
                        fake.create_common_fake([itm for itm in includes if itm not in headers])

                        pos, neg, is_parsed = self.parse_file(root, file_name, exclusions)

                        if pos is not None:
                            total_pos += pos
                            total_neg += neg

                        if not is_parsed:
                            num_failed += 1
                        total_files += 1

            if idx % (10**2) == 0:
                print("{:20}{:10}   |   {:20} {:10}".format("files processed: ", total_files, "failed to parse: ", num_failed))
                print("{:20}{:10}   |   {:20} {:10}".format("pos examples: ", total_pos, "neg examples: ", total_neg))
                print(f'exclusions: {exclusions}\n')

        return total_pos, total_neg, exclusions, total_files, num_failed


# parser = CLoopParser('../repositories_openMP', '../c_loops')
parser = CLoopParser('../asd', 'c_loops2')

# data = parser.load('/home/talkad/Downloads/thesis/data_gathering_script/c_parser/c_loops2/123/threadGauss_pos_0.pickle')
# ast = data.loop
# print(f'pragma: {data.omp_pragma}')
# print('code:\n')
# ast.show()

total = parser.scan_dir()
print(total)

# (5176, 6829, {'bad_case': 1988, 'empty': 131, 'duplicates': 53288, 'func_calls': 5907}, 21814, 10042)
