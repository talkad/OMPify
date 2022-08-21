
from fparser.two.parser import ParserFactory
from fparser.common.readfortran import FortranStringReader
import fparser.two.Fortran2003 as FortranStructs
import pickle
import os
import re
from functools import reduce

# ignore line comment
LINE_COMMENT_RE = re.compile("(^!.*)|(^c.*)|(^C.*)", re.MULTILINE)

def is_for_pragma(line):
    '''
    Returns true if the given line is an OMP pragma

    Parameters:
        line (str) - a single line from source coded 
    '''
    sub_line = line.lstrip() # remove redundant white spaces

    return sub_line.startswith('!$omp ') and ' do' in line and ' end' not in line

def remove_empty_lines(code_buf):
    return reduce(lambda acc, cur: f'{acc}\n{cur}' if len(cur.lstrip()) > 0 else acc, code_buf.split('\n'))

def add_omp_identifier(code_buf):
    return reduce(lambda acc, cur: f'{acc}\n###{cur}' if is_for_pragma(cur.lower()) else f'{acc}\n{cur}', code_buf.split('\n'))

def remove_omp_identifier(code_buf):
    return reduce(lambda acc, cur: f'{acc}\n{cur[3:]}' if cur.startswith('###') else f'{acc}\n{cur}', code_buf.split('\n'))


class OmpLoop:
    def __init__(self, omp_pragma, loop):
        self.omp_pragma = omp_pragma # omp pragma associated with the given loop
        self.loop = loop             # xml format representing AST structure of loop


class FortranLoopParser:
    def __init__(self, omp_repo, ast_repo):
        self.fortran_extensions = ['.f', '.f90']
        self.parser = ParserFactory().create(std="f2008")
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
        reader = FortranStringReader(code_buf, ignore_comments=False)
        return self.parser(reader)
        # try:
        #     return self.parser.parse(code_buf)
        # except pycparser.plyparser.ParseError:  
        #     # print(f'Parser Error: {filepath}')
        #     return
        # except:  
        #     # print(f'Unexpected Error: {filepath}')
        #     return

    def parse_file(self, root_dir, file_name):
        '''
        Parse the given file into ast and extract to loops associated with omp pargma (or without)
        '''
        file_path = os.path.join(root_dir, file_name)
        save_dir = os.path.join(self.ast_repo, root_dir[self.split_idx: ])
        name = os.path.splitext(file_name)[0]
        
        with open(file_path, 'r') as f:
            
            try:
                code = f.read()
            except UnicodeDecodeError:
                return 0, 0

            code = remove_empty_lines(code)
            code = add_omp_identifier(code)
            code = LINE_COMMENT_RE.sub("", code)
            code = remove_omp_identifier(code)
            ast = self.create_ast(file_path, code)

            # print(ast)

            return 0, 0

            # if ast is None:                 # file parsing failed
            #     return 0, 0

            # pfv.visit(ast)

            # for idx, (pragma, loop) in enumerate(zip(pfv.pragmas, pfv.pos_nodes)):
            #     verify_loops.reset()
            #     verify_loops.visit(loop)

            #     if not verify_loops.found: # check for barrier/atomic inside a loop - usually a bad case
            #         self.create_directory(save_dir) 
            #         self.save(os.path.join(save_dir, f'{name}_pos_{idx}.pickle'), pragma, loop)

            # for idx, loop in enumerate(pfv.neg_nodes):
            #     self.create_directory(save_dir) 
            #     self.save(os.path.join(save_dir, f'{name}_neg_{idx}.pickle'), None, loop)

            # return len(pfv.pos_nodes), len(pfv.neg_nodes)

    def scan_dir(self):
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

code = """
PROGRAM Area
IMPLICIT NONE


! Declare local variables
REAL :: radius, Area_Circle
INTEGER :: idx

!$omp parallel do
DO idx=1,3
  print*, idx
END DO

! what
DO 100 idx=1,3
  print*, idx
100 CONTINUE

!$omp parallel do reduction
DO idx=1,3
  print*, idx
END DO

END PROGRAM Area
"""


code = remove_empty_lines(code)
code = add_omp_identifier(code)
code = LINE_COMMENT_RE.sub("", code)
code = remove_omp_identifier(code)
# print(code)

READER = FortranStringReader(code, ignore_comments=False)
F2008_PARSER = ParserFactory().create(std="f2008")
PROGRAM = F2008_PARSER(READER)
# print(PROGRAM)
# PROGRAM


# # parser = FortranLoopParser('repositories_openMP', 'fortran_loops')
# parser = FortranLoopParser('../asd', 'fortran_eample')

# # parser.parse_file('example.f90')
# # data = parser.load('fortran_eample/area_neg_0.pickle')

# total = parser.scan_dir()
# print(total)

def is_leaf(node):
    try:
        ch = node.children
        return False
    except (TypeError, AttributeError) as e:
        return True

loops = []
pragmas = []


class what:
    def __init__(self):
        self.pragma = ''

    def get_pragma(self, prog):
        for sub in prog.children:
            if type(sub) is FortranStructs.Comment and is_for_pragma(str(sub).lower()): 
                return str(sub).lower()
            elif not is_leaf(sub):
                self.get_pragma(sub)

        return ''

    def func(self, prog):
        for sub in prog.children:
            # print(type(sub))
            if type(sub) is FortranStructs.Comment and is_for_pragma(str(sub).lower()):
                self.pragma = str(sub)
                # print(self.pragma )
            elif type(sub) is FortranStructs.Block_Label_Do_Construct or type(sub) is FortranStructs.Block_Nonlabel_Do_Construct:
                loops.append(sub)
                # print('aaaaaaaaaaaaaaaaaaaaaaaaaaa')
                # print(sub)

                if len(self.pragma) != 0:
                    pragmas.append(self.pragma)
                else:  #check for pragma
                     pragmas.append(self.get_pragma(sub))
                    # child = sub.children[1]
                    # print(type(child))
                    # if type(child) is FortranStructs.Comment and is_for_pragma(str(child).lower()): 
                    #     pragmas.append(str(child))
                    # else:
                    #     pragmas.append('a')

                # print(pragmas)

                self.pragma = ''
            elif not is_leaf(sub):
                self.func(sub)


w = what()
w.func(PROGRAM)
a = str(loops[2])
a = LINE_COMMENT_RE.sub("", a)
print(a)
code_format = """
PROGRAM Main
IMPLICIT NONE

{}

END PROGRAM Main
"""
code = code_format.format(a)


READER = FortranStringReader(code, ignore_comments=False)
F2008_PARSER = ParserFactory().create(std="f2008")
PROGRAM = F2008_PARSER(READER)

print(PROGRAM)

# for loop in loops:
#     print('???' ,loop)