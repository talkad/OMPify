import pycparser
import pickle
import os
from pycparser import CParser
from pycparser.c_ast import NodeVisitor, Constant
import re
from visitors import *
from functools import reduce

# ignore unsuported directives
INCLUDES_RE = re.compile("^#include(.+)$|^#define(.+)$|^# include(.+)$|^#ifdef(.+)$|^#endif(.*)$", re.MULTILINE)
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
        self.parser = CParser()
        self.root_dir = os.getcwd()
        self.omp_repo = omp_repo
        self.ast_repo = ast_repo
        self.split_idx = len(os.path.join(self.root_dir, self.omp_repo)) + 1


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

    def create_ast(self, filepath, code_buf):
        try:
            return self.parser.parse(code_buf)
        except pycparser.plyparser.ParseError:  
            # print(f'Parser Error: {filepath}')
            return
        except:  
            # print(f'Unexpected Error: {filepath}')
            return

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
    
    def parse_file(self, root_dir, file_name):
        '''
        Parse the given file into ast and extract to loops associated with omp pargma (or without)
        '''
        file_path = os.path.join(root_dir, file_name)
        save_dir = os.path.join(self.ast_repo, root_dir[self.split_idx: ])
        name = os.path.splitext(file_name)[0]

        pfv = PragmaForVisitor()
        verify_loops = ForLoopChecker()
        
        with open(file_path, 'r') as f:
            
            try:
                code = f.read()
            except UnicodeDecodeError:
                return 0, 0

            code = INCLUDES_RE.sub("", code)
            code = LINE_COMMENT_RE.sub("", code)
            code = MULTILINE_COMMENT_RE.sub("", code)
            code = self.join_splited_lines(code)
            code = self.join_funcDecl(code)
            ast = self.create_ast(file_path, code)

            if ast is None:                 # file parsing failed
                return 0, 0

            pfv.visit(ast)

            for idx, (pragma, loop) in enumerate(zip(pfv.pragmas, pfv.pos_nodes)):
                verify_loops.reset()
                verify_loops.visit(loop)

                if not verify_loops.found: # check for barrier/atomic inside a loop - usually a bad case
                    self.create_directory(save_dir) 
                    self.save(os.path.join(save_dir, f'{name}_pos_{idx}.pickle'), pragma, loop)

            for idx, loop in enumerate(pfv.neg_nodes):
                self.create_directory(save_dir) 
                self.save(os.path.join(save_dir, f'{name}_neg_{idx}.pickle'), None, loop)

            return len(pfv.pos_nodes), len(pfv.neg_nodes)

    def scan_dir(self):
        # check if fortran file
        total_pos = 0
        total_neg = 0
        omp_repo = os.path.join(self.root_dir, self.omp_repo)

        for idx, (root, dirs, files) in enumerate(os.walk(omp_repo)):
            for file_name in files:
                ext = os.path.splitext(file_name)[1].lower()
                
                if ext in self.fortran_extensions:
                    pos, neg = self.parse_file(root, file_name)

                    if pos is not None:
                        total_pos += pos
                        total_neg += neg

            if idx % (10**2) == 0:
                print(f'Message {idx // (10**2)}) {total_pos} {total_neg}')

        return total_pos, total_neg


# parser = CLoopParser('../repositories_openMP', '../c_loops')
# parser = CLoopParser('../asd', '../c_loops2')

data = parser.load('/home/talkad/Downloads/thesis/data_gathering_script/c_loops/0xe1d1a/pcs/omp/vec_pos_0.pickle')
print(data.loop)

# total = parser.scan_dir()
# print(total)

# (33420, 30280)