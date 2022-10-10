import os
from parsers.parser import Parser
from fparser.two.parser import ParserFactory
from fparser.common.readfortran import FortranStringReader
import fparser.two.Fortran2003 as FortranStructs
from fparser.two.utils import NoMatchError
from functools import reduce
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
    '''
    Several for struct defined by fparser still hold the pragma although it is not part of the for loop
    '''
    code = code_buf.split('\n')
    index = 0

    for line in code:
        l = line.lower()

        if '!$omp ' not in l and 'do ' in l:
            break
        index += 1

    return '\n'.join(code[index:])

def is_code_line(line):
    '''
    check if line is neither a comment nor pragma
    '''
    l = line.lstrip().lower()
    return not l.startswith(('c ','c\t','c\n', '!', '*'))

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
    '''
    Returns true if the given code is a do while
    fparser converts 'do' and 'do while' into the same struct
    '''
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

class LoopExtractor:
    '''
    Implements the for-loop extraction functionality 
    '''

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
        '''
        Extract pragma for given sub-program
        '''
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

    def extract_loops(self, prog):
        '''
        Extract all loops from given program
        '''
        children = prog.children

        if not is_iter(children):
            return 

        for sub in children:
            if type(sub) is FortranStructs.Comment and is_for_pragma(str(sub).lower()):
                self.pragma = str(sub)
            elif (type(sub) is FortranStructs.Block_Label_Do_Construct or type(sub) is FortranStructs.Block_Nonlabel_Do_Construct) \
                    and not is_do_while(sub):   # classified as do loop by the parser
                if len(self.pragma) != 0:
                    self.omp_pragmas.append(self.pragma)
                    self.loops.append(sub)
                else:                           # check for pragma
                    pragma = self.get_pragma(sub)
                    
                    if pragma is not None:      # manage to get pragma
                        self.omp_pragmas.append(pragma)
                        self.loops.append(sub)

                self.pragma = ''
            elif not self.is_leaf(sub):
                self.extract_loops(sub)

class FortranLoopParser(Parser):
    def __init__(self, repo_path, parsed_path):
        super().__init__(repo_path, parsed_path, ['.f', '.f90', '.f95'])
        self.parser = ParserFactory().create(std="f2008")


    def create_ast(self, file_path, code_buf, result):
        '''
        This weird module define termination point on failure.
        We will prevent this by using threads.
        '''
        try:
            reader = FortranStringReader(code_buf, ignore_comments=False)
            result.append(self.parser(reader))
        except NoMatchError:  
            # print(f'Parser Error: {file_path}')
            return
        except Exception as e:  
            # print(f'Unexpected Error: {file_path} ->\n {e}')
            return       

    def parse(self, file_path, code_buf):
        future_result = []
        t = Thread(target=self.create_ast, args=(file_path, code_buf, future_result))

        t.start()
        t.join(timeout=60)

        if t.is_alive():
            return
        elif len(future_result) == 0:
            return
        else:
            return future_result[0]

    def load(self, file_path):
        omp_obj = super().load(file_path)
        omp_obj.ast_loop = self.parse(file_path, omp_obj.textual_loop)

        return omp_obj

    def code_preprocess_pipline(self, code):
        '''
        fix code structure so in can be parsed by fparser
        '''
        code = remove_empty_lines(code)
        code = add_omp_identifier(code)
        code = remove_omp_identifier(code)
        code = self.join_splited_lines(code, delimiter='&')
        return remove_fortran_comments(code)

    def parse_file(self, root_dir, file_name, exclusions):
        '''
        Parse the given file into ast and extract to loops associated with omp pargma (or without)
        '''
        pos, neg = 0, 0
        file_path = os.path.join(root_dir, file_name)
        save_dir = os.path.join(self.parsed_path, root_dir[self.split_idx: ])
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

            code = self.code_preprocess_pipline(code)
            ast = self.parse(file_path, code)

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

                ast_loop = self.parse(file_path, textual_loop)
                if ast_loop is None:
                    continue

                self.memory.append(textual_loop)

                # the fparser doesn't define __new__ function for its ast
                if pragma == '':
                    neg += 1
                    self.save(os.path.join(save_dir, f'{name}_neg_{idx}.pickle'), None, None, textual_loop)
                else:
                    pos += 1
                    self.save(os.path.join(save_dir, f'{name}_pos_{idx}.pickle'), pragma, None, textual_loop)

            return pos, neg, True


# parser = FortranLoopParser('../repositories_openMP', '../fortran_loops')
# parser = FortranLoopParser('../asd', 'fortran_example')

# data = parser.load('/home/talkad/Downloads/thesis/data_gathering_script/parsers/fortran_example/123/a_pos_0.pickle')
# print(f'pragma: {data.omp_pragma}\n')
# print('code:')
# print(data.ast_loop)

# total = parser.scan_dir()
# print(total)

# pos - 2753 
# neg - 3331 

# (2753, 3331, {'bad_case': 682, 'empty': 1045, 'duplicates': 15622}, 6174, 1099)