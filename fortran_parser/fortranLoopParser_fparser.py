
from fparser.two.parser import ParserFactory
from fparser.common.readfortran import FortranStringReader
import fparser.two.Fortran2003 as FortranStructs
from fparser.two.utils import NoMatchError
import pickle
import os
import re
from functools import reduce
from collections import defaultdict
from threading import Thread


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
    return reduce(lambda acc, cur: f'{acc}\n###{cur}' if cur.lstrip().lower().startswith('!$omp ') else f'{acc}\n{cur}', code_buf.split('\n'))

def remove_omp_identifier(code_buf):
    return reduce(lambda acc, cur: f'{acc}\n{cur[3:]}' if cur.startswith('###') else f'{acc}\n{cur}', code_buf.split('\n'))

def remove_omp(code_buf):
    code = code_buf.split('\n')
    index = 0

    for line in code:
        l = line.lower()

        if '!$omp ' not in l and 'do ' in l:
            break
        index += 1

    return '\n'.join(code[index:])
    # return reduce(lambda acc, cur: acc if cur.lstrip().startswith('!') else f'{acc}\n{cur}', code_buf.split('\n'))

def is_code_line(line):
    '''
    check if line is neither a comment nor pragma
    '''
    l = line.lstrip().lower()
    return not l.startswith(('c ','c\t','c\n', '!', '*'))

def join_splited_lines(code_buf):
    '''
    Several Fortran files are splitting lines of code using & token. 
    fparser fails to process this files. So we update this lines to be a single line.
    '''
    code = []
    splitted_line = False

    for line in code_buf.split('\n'):
        if not splitted_line and len(line) > 0 and line[-1] == '&' and is_code_line(line):
            code.append(line[:-1])
            splitted_line = True
        elif splitted_line and len(line) > 0 and line[-1] == '&' and is_code_line(line):
            code[-1] += line[:-1]
        elif splitted_line:
            code[-1] += line
            splitted_line = False
        else:
            code.append(line)

    return '\n'.join(code)

def remove_fortran_comments(code_buf):
    code = reduce(lambda acc, cur: acc if cur.lstrip().startswith('!') and not cur.lstrip().lower().startswith('!$omp') else f'{acc}\n{cur}', code_buf.split('\n'))
    code = reduce(lambda acc, cur: acc if cur.lstrip().startswith('*') else f'{acc}\n{cur}', code.split('\n')) 
    return reduce(lambda acc, cur: acc if cur.lstrip().lower().startswith(('c ','c\t','c\n')) else f'{acc}\n{cur}', code.split('\n'))

def exclusion_case(code_buf):
    '''
    loop that contains critical or atomic are usually a bad scenario
    '''
    for line in code_buf.split('\n'):
        l = line.lower()

        if '!$omp ' in l and ('atomic' in l or 'barri' in l or 'critical' in l):
            return True
    
    return False

def is_do_while(node):
    code = str(node)
    
    for line in code.split('\n')[:2]:
        l = line.lower()
        if 'do ' in l and ' while' in l:
            return True
    
    return False

def is_iter(obj):
    try:
        iter(obj)
        return True
    except TypeError as te:
        return False

class OmpLoop:
    def __init__(self, omp_pragma, textual_loop):
        self.omp_pragma = omp_pragma         # omp pragma associated with the given loop
        # self.ast_loop = ast_loop             # ast format representing AST structure of loop
        self.textual_loop = textual_loop     # textual representation of code

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

    def is_empty_loop(self, code):
        in_do = False

        for line in code.split('\n'):
            l = line.lower()
            if 'do ' in l:
                in_do = True
            elif 'end do' in l or 'continue' in l:
                return True
            elif in_do and len(l.lstrip()) > 0:
                return False

                
        #### alot of casess to cover :(
        # for ch in ast_loop.children:
        #     if type(ch) is FortranStructs.Main_Program or type(ch) is FortranStructs.Execution_Part or \
        #         type(ch) is FortranStructs.Block_Label_Do_Construct or type(ch) is FortranStructs.Block_Nonlabel_Do_Construct or \
        #         type(ch) is FortranStructs.Label_Do_Stmt or type(ch) is FortranStructs.Nonlabel_Do_Stmt or \
        #         type(ch) is FortranStructs.End_Do_Stmt:
        #         self.is_empty_loop(ch)
        #     # ignore redundant blocks
        #     elif type(ch) is not FortranStructs.Program_Stmt and type(ch) is not FortranStructs.Specification_Part and \
        #         type(ch) is not FortranStructs.Comment and type(ch) is not FortranStructs.End_Program_Stmt and type(ch) is not str:
        #         print(type(ch))
        #         return True

        # return False

    def extract_loops(self, prog):
        children = prog.children

        if not is_iter(children):
            return 

        for sub in children:
            if type(sub) is FortranStructs.Comment and is_for_pragma(str(sub).lower()):
                self.pragma = str(sub)
            elif (type(sub) is FortranStructs.Block_Label_Do_Construct or type(sub) is FortranStructs.Block_Nonlabel_Do_Construct) \
                    and not is_do_while(sub):  # classified as do loop by the parser
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
        self.fortran_extensions = ['.f', '.f90', '.f95']
        self.parser = ParserFactory().create(std="f2008")
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

    def save(self, file_path, pragma, ast_loop, textual_loop):
        '''
        Save OmpLoop structure into file_path
        '''
        parsed_loop = OmpLoop(pragma, textual_loop)

        with open(file_path, 'wb') as f:
            pickle.dump(parsed_loop, f)

    def load(self, file_path):
        '''
        Load OmpLoop structure from file_path
        '''
        with open(file_path, 'rb') as f:
            return pickle.load(f)

    def parse(self, code_buf, result):
        '''
        This weird modeule define termination point on failure.
        We will prevent this behavior using threads.
        '''
        try:
            reader = FortranStringReader(code_buf, ignore_comments=False)
            result.append(self.parser(reader))
        except NoMatchError:  
            # print(f'Parser Error: {filepath}')
            return
        except Exception as e:  
            # print(f'Unexpected Error: {filepath} ->\n {e}')
            return       

                # try:
        #     reader = FortranStringReader(code_buf, ignore_comments=False)
        #     return self.parser(reader)
        # except NoMatchError:  
        #     # print(f'Parser Error: {filepath}')
        #     return
        # except Exception as e:  
        #     # print(f'Unexpected Error: {filepath} ->\n {e}')
        #     return

    def create_ast(self, filepath, code_buf):
        result = []
        threads = []

        threads.append(Thread(target=self.parse, args=(code_buf, result)))

        for t in threads:
            t.start()
        for t in threads:
            t.join(timeout=30)

        if t.is_alive():
            return
        elif len(result) == 0:
            return
        else:
            return result[0]

    def parse_file(self, root_dir, file_name, exclusions):
        '''
        Parse the given file into ast and extract to loops associated with omp pargma (or without)
        '''
        pos, neg = 0, 0
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
                return 0, 0, False

            code = remove_empty_lines(code)
            code = add_omp_identifier(code)
            code = remove_omp_identifier(code)
            code = join_splited_lines(code)
            code = remove_fortran_comments(code)
            ast = self.create_ast(file_path, code)

            if ast is None:                 # file parsing failed
                return 0, 0, False

            extractor.extract_loops(ast)
            pragmas, loops = extractor.omp_pragmas, extractor.loops

            for idx, (pragma, loop) in enumerate(zip(pragmas, loops)):
                self.create_directory(save_dir) 
                textual_loop = str(loop)
                textual_loop = remove_omp(textual_loop)
                textual_loop = code_format.format(textual_loop)

                # check for exclusion cases:
                if exclusion_case(textual_loop):
                    exclusions['bad_case'] += 1
                    continue
                elif extractor.is_empty_loop(textual_loop):
                    exclusions['empty'] += 1
                    continue
                elif textual_loop in self.memory:
                    exclusions['duplicates'] += 1
                    continue

                ast_loop = self.create_ast(file_path, textual_loop)

  
                if ast_loop is None:
                    continue

                self.memory.append(textual_loop)

                if pragma == '':
                    neg += 1
                    self.save(os.path.join(save_dir, f'{name}_neg_{idx}.pickle'), None, ast_loop, textual_loop)
                else:
                    pos += 1
                    self.save(os.path.join(save_dir, f'{name}_pos_{idx}.pickle'), pragma, ast_loop, textual_loop)

            return pos, neg, True

    def scan_dir(self):
        total_files = 0
        num_failed = 0
        total_pos = 0
        total_neg = 0
        omp_repo = os.path.join(self.root_dir, self.omp_repo)
        exclusions = {'bad_case': 0, 'empty': 0, 'duplicates': 0}

        for idx, (root, dirs, files) in enumerate(os.walk(omp_repo)):
            for file_name in files:
                # print(os.path.join(root, file_name))
                ext = os.path.splitext(file_name)[1].lower()
                
                if ext in self.fortran_extensions:
                    pos, neg, is_parsed = self.parse_file(root, file_name, exclusions)

                    if pos is not None:
                        total_pos += pos
                        total_neg += neg

                    if not is_parsed:
                        num_failed += 1
                    total_files += 1

            if idx % (10**2) == 0:
                print(f'Message {idx // (10**2)}) {total_files} {total_pos} {total_neg} \n {exclusions} \n\n')

        return total_pos, total_neg, exclusions, total_files, num_failed

# parser = FortranLoopParser('../repositories_openMP', '../fortran_loops')
parser = FortranLoopParser('../asd', 'fortran_example')

# data = parser.load('/home/talkad/Downloads/thesis/data_gathering_script/fortran_loops/AbbasHub/AMR-LBM-OpenMP-2D/Fill_Ghost_Cells_C_neg_1.pickle')
# print(f'pragma: {data.omp_pragma}\n')
# print('code:')
# print(parser.create_ast('', data.textual_loop))

total = parser.scan_dir()
print(total)

# pos - 2753 
# neg - 3331 

# (2753, 3331, {'bad_case': 682, 'empty': 1045, 'duplicates': 15622}, 6174, 1099)