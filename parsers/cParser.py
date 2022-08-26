import os
import pycparser
from parser import Parser
import pickle
from visitors import *
from functools import reduce
import helper


class CLoopParser(Parser):
    def __init__(self, repo_path, parsed_path):
        super().__init__(repo_path, parsed_path, ['.c', '.cc', '.h'])

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

    def code_preprocess_pipline(self, code):
        code = helper.join_splited_lines(code)
        return self.join_funcDecl(code)

    def parse(self, file_path, code_buf):
        repo_name = file_path[len(self.repo_path + self.root_dir) + 2:]
        repo_name = repo_name[:repo_name.find('/') ]

        cpp_args = ['-nostdinc',  '-E', r'-I' + helper.FAKE_DEFINES, r'-I' + helper.FAKE_TYPEDEFS]

        headers = helper.get_headers(repo_name)
        for header in headers:
            cpp_args.append(r'-I' + header)

        try:
            return pycparser.parse_file(file, use_cpp=True, cpp_path='mpicc', cpp_args = cpp_args)
            # return self.parser.parse(code_buf)
        except pycparser.plyparser.ParseError:  
            # print(f'Parser Error: {file_path}')
            return
        except:  
            # print(f'Unexpected Error: {file_path}')
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

        with open(file_path, 'r') as f:
            
            try:
                code = f.read()
            except UnicodeDecodeError:
                return 0, 0, False

            code = self.code_preprocess_pipline(code)
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
                self.save(os.path.join(save_dir, f"{name}{'_neg_' if pragma is None else '_pos_'}{idx}.pickle"), pragma, loop)

                if pragma is None:
                    neg += 1
                else:
                    pos += 1

            return pos, neg, True

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