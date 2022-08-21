
from fparser.two.parser import ParserFactory
from fparser.common.readfortran import FortranStringReader
import fparser.two.Fortran2003 as FortranStructs
from fparser.two.utils import NoMatchError
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

def remove_omp(code_buf):
    code_buf = '\n' + code_buf

    return reduce(lambda acc, cur: acc if cur.lstrip().startswith('!') else f'{acc}\n{cur}', code_buf.split('\n'))

def is_iter(obj):
    try:
        iter(obj)
        return True
    except TypeError as te:
        return False


class OmpLoop:
    def __init__(self, omp_pragma, loop):
        self.omp_pragma = omp_pragma # omp pragma associated with the given loop
        self.loop = loop             # xml format representing AST structure of loop

class LoopExtractor:
    def __init__(self):
        self.pragma = ''
        self.loops = []
        self.omp_pragmas = []

    def is_leaf(self, node):
        try:
            ch = node.children
            return False
        except (TypeError, AttributeError) as e:
            return True

    def get_pragma(self, prog):
        children = []

        try:
            for sub in prog.children:
                if type(sub) is FortranStructs.Comment and is_for_pragma(str(sub).lower()): 
                    return str(sub).lower()
                elif not self.is_leaf(sub):
                    self.get_pragma(sub)

            return ''
        except:
            return None

    def extract_loops(self, prog):
        children = prog.children

        if not is_iter(children):
            return 

        for sub in children:

            if type(sub) is FortranStructs.Comment and is_for_pragma(str(sub).lower()):
                self.pragma = str(sub)
            elif type(sub) is FortranStructs.Block_Label_Do_Construct or type(sub) is FortranStructs.Block_Nonlabel_Do_Construct:

                if len(self.pragma) != 0:
                    self.omp_pragmas.append(self.pragma)
                    self.loops.append(sub)
                else:  #check for pragma
                    pragma = self.get_pragma(sub)
                    
                    if pragma is not None: # manage to get pragma
                        self.omp_pragmas.append(pragma)
                        self.loops.append(sub)

                self.pragma = ''
            elif not self.is_leaf(sub):
                self.extract_loops(sub)

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
        try:
            reader = FortranStringReader(code_buf, ignore_comments=False)
            return self.parser(reader)
        except NoMatchError:  
            print(f'Parser Error: {filepath}')
            return
        except:  
            # print(f'Unexpected Error: {filepath}')
            return

    def parse_file(self, root_dir, file_name):
        '''
        Parse the given file into ast and extract to loops associated with omp pargma (or without)
        '''
        neg = 0
        file_path = os.path.join(root_dir, file_name)
        save_dir = os.path.join(self.ast_repo, root_dir[self.split_idx: ])
        name = os.path.splitext(file_name)[0]

        extractor = LoopExtractor()
        
        code_format = """
            PROGRAM Main

            {}

            END PROGRAM Main
            """
        
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

            if ast is None:                 # file parsing failed
                return 0, 0

            extractor.extract_loops(ast)
            pragmas, loops = extractor.omp_pragmas, extractor.loops

            for idx, (pragma, loop) in enumerate(zip(pragmas, loops)):
                self.create_directory(save_dir) 
                loop = str(loop)
                loop = remove_omp(loop)
                # print(f'===============\n{loop}\n=================')

                loop = code_format.format(loop)

                if pragma == '':
                    neg += 1
                    self.save(os.path.join(save_dir, f'{name}_neg_{idx}.pickle'), None, loop)
                else:
                    self.save(os.path.join(save_dir, f'{name}_pos_{idx}.pickle'), pragma, loop)

            return len(pragmas) - neg, neg

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

parser = FortranLoopParser('../repositories_openMP', '../fortran_loops')
# parser = FortranLoopParser('../asd', 'fortran_eample')

# data = parser.load('/home/talkad/Downloads/thesis/data_gathering_script/fortran_loops/bgin/Guided_Missile_Simulation/FFT/FFTE/dzfft2d_pos_0.pickle')
# data = parser.load('fortran_eample/123/a_pos_4.pickle')
# print(f'{data.omp_pragma}\n')
# print(parser.create_ast('', data.loop))

total = parser.scan_dir()
print(total)

# pos - 10402 
# neg - 8103 