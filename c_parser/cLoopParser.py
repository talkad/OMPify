import pycparser
from pycparser import parse_file
import pickle
import os
# from pycparser import CParser
# from pycparser.c_ast import NodeVisitor, Constant, For
import re
from visitors import *
from functools import reduce
import helper

# ignore unsuported directives
# INCLUDES_RE = re.compile("^#include(.+)$|^#define(.+)$|^#ifdef(.+)$|^#endif(.*)$", re.MULTILINE)
# ignore one line comment
LINE_COMMENT_RE = re.compile("//.*?\n" )
# ignore multiline comment
MULTILINE_COMMENT_RE = re.compile("/\*.*?\*/", re.DOTALL)


class OmpLoop:
    def __init__(self, omp_pragma, loop):
        self.omp_pragma = omp_pragma # omp pragma associated with the given loop
        self.loop = loop             # xml format representing AST structure of loop


class CLoopParser:
    def __init__(self, omp_repo, ast_repo):
        self.fortran_extensions = ['.c', '.cc', '.h']
        # self.parser = CParser()
        self.root_dir = os.getcwd()
        self.omp_repo = omp_repo
        self.ast_repo = ast_repo
        self.split_idx = len(os.path.join(self.root_dir, self.omp_repo)) + 1
        self.memory = []


    def create_directory(self, dirs):
        '''
        Create directories if not exists
        '''
        if not os.path.exists(dirs):
            os.makedirs(dirs)

    def save(self, file_path, pragma, loop):
        '''
        Save OmpLoop structure into file_path
        '''
        parsed_loop = OmpLoop(pragma, loop)

        with open(file_path, 'wb') as f:
            pickle.dump(parsed_loop, f)

    def load(self, file_path):
        '''
        Load OmpLoop structure from file_path
        '''
        with open(file_path, 'rb') as f:
            return pickle.load(f)

    def create_ast(self, filepath, repo_name, code_buf):
        cpp_args = ['-nostdinc',  '-E'] # , r'-I' + FAKE_HEADER_PATH]  check it

        headers = helper.get_headers(repo_name)
        print(repo_name)

        for header in headers:
            cpp_args.append(r'-I' + header)

        try:
            return parse_file(file, use_cpp=True, cpp_path='mpicc', cpp_args = cpp_args)
            # return self.parser.parse(code_buf)
        except pycparser.plyparser.ParseError:  
            # print(f'Parser Error: {filepath}')
            return
        except:  
            # print(f'Unexpected Error: {filepath}')
            return

    def remove_directives(self, code):
        return reduce(lambda acc, cur: acc if cur.lstrip().lower().startswith(('#if', '#elif', '#else')) else f'{acc}\n{cur}', code.split('\n'))


    def join_splited_lines(self, code):
        '''
        Several c files are splitting lines of code using \ token. For instance:
            #pragma omp parallel for private(i, test, x, y) \
                default(shared) schedule(dynamic)
        pycparser fail to process this files. So we update this lines to be a single line.
        '''
        return reduce(lambda acc, cur: f'{acc}\n{cur[: -1]}' if len(cur) > 0 and cur[-1] == '\\' else f'{acc} {cur}\n', code.split('\n'))

    def join_funcDecl(self, code):
        '''
        Several c files define the function return type in separate line. For instance:
            int
            check_omp_lock (FILE * logFile) {...}
        pycparser fail to process this files. So we join this lines to proper func. declaration.
        '''
        return_types = ['char', 'short', 'int', 'long', 'float', 'double']

        return reduce(lambda acc, cur: f'{acc}\n{cur}' if cur.lstrip() in return_types else f'{acc} {cur}\n', code.split('\n'))

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

    def parse(self, root_dir, file_name, exclusions):
        '''
        Parse the given file into ast and extract to loops associated with omp pargma (or without)
        '''
        pos, neg = 0, 0
        file_path = os.path.join(root_dir, file_name)
        save_dir = os.path.join(self.ast_repo, root_dir[self.split_idx: ])
        name = os.path.splitext(file_name)[0]

        last_idx = root_dir.rfind('/') if len(self.root_dir + self.omp_repo) + 2 < root_dir.rfind('/') else len(root_dir)
        repo_name = root_dir[len(self.root_dir + self.omp_repo) + 2: last_idx]
        print(repo_name)

        pfv = PragmaForVisitor()
        verify_loops = ForLoopChecker()
        func_call_checker = FuncCallChecker()

        with open(file_path, 'r') as f:
            
            try:
                code = f.read()
            except UnicodeDecodeError:
                return 0, 0, False

            # code = INCLUDES_RE.sub("", code)
            code = LINE_COMMENT_RE.sub("", code)
            code  = MULTILINE_COMMENT_RE.sub("", code)
            code = self.remove_directives(code)
            code = self.join_splited_lines(code)
            code = self.join_funcDecl(code)
            ast = self.create_ast(file_path, repo_name, code)

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
                self.save(os.path.join(save_dir, f"{name}{'_neg_' if pragma is None else '_pos_'}{idx}.pickle"), pragma, loop)

                if pragma is None:
                    neg += 1
                else:
                    pos += 1

            return pos, neg, True

    def scan_dir(self):
        # check if ortran file
        total_files = 0
        num_failed = 0
        total_pos = 0
        total_neg = 0
        exclusions = {'bad_case': 0, 'empty': 0, 'duplicates': 0, 'func_calls':0}
        omp_repo = os.path.join(self.root_dir, self.omp_repo)

        for idx, (root, dirs, files) in enumerate(os.walk(omp_repo)):
            for file_name in files:
                ext = os.path.splitext(file_name)[1].lower()
                
                if ext in self.fortran_extensions:
                    pos, neg, is_parsed = self.parse(root, file_name, exclusions)

                    if pos is not None:
                        total_pos += pos
                        total_neg += neg

                    if not is_parsed:
                        num_failed += 1
                    total_files += 1

            if idx % (10**2) == 0:
                print(f'Message {idx // (10**2)}) {total_files} {num_failed} {total_pos} {total_neg}\n{exclusions}\n\n')

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