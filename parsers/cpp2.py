import os
import re
from enum import Enum
from parser import Parser
from visitors import *
import pycparser
from pycparser.c_parser import CParser
from pycparser.c_ast import For, Pragma
from multiprocessing import Process, Manager


LINE_COMMENT_RE = re.compile("//.*?\n" )
MULTILINE_COMMENT_RE = re.compile("/\*.*?\*/", re.DOTALL)


def log(file_name, msg):
    with open(file_name, 'a') as f:
        f.write(f'{msg}\n')


class State(Enum):
    '''
    Enum represets the state of the for loops extraction
    '''
    start = 1
    for_stmt = 2
    for_body = 3
    for_body_found = 4


class LoopExtractor:
    '''
    Implements the for-loop extraction functionality 
    '''
    def __init__(self):
        self.loops = []
        self.omp_pragmas = []

    def is_for_pragma(self, line):
        '''
        Returns true if the given line is an OMP pragma

        Parameters:
            line (str) - a single line from source coded 
        '''
        sub_line = line.lstrip() # remove redundant white spaces

        return sub_line.startswith('#pragma ') and ' omp' in line and ' for' in line

    def skip_lines(self, code):
        '''
        skip lines that are not part of a for loop
        '''
        pragma = None
        code_lines = code.split('\n')

        for idx, line in enumerate(code_lines):
            if self.is_for_pragma(line):
                pragma =  line
            elif line.lstrip().startswith('for'):
                code = '\n'.join(code_lines[idx:])
                return code, pragma

        return '', None

    # def extract_loops(self, code, is_nested=False):
    #     '''
    #     Extract all loops and pragmas from given program by parentheses balancing
    #     '''         
    #     code, pragma = self.skip_lines(code)

    #     idx = 0
    #     loop = ''
    #     body = ''

    #     num_paren = 0
    #     state = State.start

    #     for idx, ch in enumerate(code):
    #         if state == State.start:
    #             loop += ch

    #             if ch == '(':
    #                 state = State.for_stmt
    #                 num_paren += 1

    #         elif state == State.for_stmt:
    #             loop += ch

    #             if ch == '(':
    #                 num_paren += 1
    #             elif ch == ')':
    #                 num_paren -= 1

    #             if num_paren == 0:
    #                 state = State.for_body

    #         elif state == State.for_body or state == State.for_body_found:
    #             loop += ch

    #             if num_paren > 0:
    #                 body += ch

    #             if num_paren == 0 and ch == ';': # single line loop (no curly brackets)
    #                 # print('a\n', loop)
    #                 self.loops.append(loop)
    #                 self.omp_pragmas.append(pragma)
    #                 break

    #             if ch == '{':
    #                 num_paren += 1
    #                 state = State.for_body_found
    #             elif  ch == '}':
    #                 num_paren -= 1

    #             if num_paren == 0 and state == State.for_body_found:
    #                 # print('c\n', loop)
    #                 self.loops.append(loop)
    #                 self.omp_pragmas.append(pragma)
    #                 self.extract_loops(body, is_nested=True)
    #                 break

    #     if not is_nested and len(code) > 0:
    #         # print('d\n', code[idx + 1:])
    #         self.extract_loops(code[idx + 1:])


    def extract_loops(self, code):
        '''
        Extract all loops and pragmas from given program by parentheses balancing
        '''      
        idx = 0

        while len(code) > 0: 
            code, pragma = self.skip_lines(code)

            loop = ''
            body = ''

            num_paren = 0
            state = State.start

            # extract next loop
            for ch in code:
                if state == State.start:
                    loop += ch

                    if ch == '(':
                        state = State.for_stmt
                        num_paren += 1

                elif state == State.for_stmt:
                    loop += ch

                    if ch == '(':
                        num_paren += 1
                    elif ch == ')':
                        num_paren -= 1

                    if num_paren == 0:
                        state = State.for_body

                elif state == State.for_body or state == State.for_body_found:
                    loop += ch

                    if num_paren > 0:
                        body += ch

                    if num_paren == 0 and ch == ';': # single line loop (no curly brackets)
                        self.loops.append(loop)
                        self.omp_pragmas.append(pragma)
                        break

                    if ch == '{':
                        num_paren += 1
                        state = State.for_body_found
                    elif  ch == '}':
                        num_paren -= 1

                    if num_paren == 0 and state == State.for_body_found:
                        self.loops.append(loop)
                        self.omp_pragmas.append(pragma)
                        self.extract_loops(body)
                        break

                idx += 1

            code = code[idx + 1:]

class CppLoopParser(Parser):
    def __init__(self, repo_path, parsed_path):
        super().__init__(repo_path, parsed_path, ['.cpp'])

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
        try:
            parser = CParser()
            loop = 'int main() {\n' + code_buf + '\n}'
            ast = parser.parse(loop)
            result['ast'] = ast.ext[0].body.block_items[0]

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
        Extract for loops from code and parse them into AST
        '''
        pos, neg = 0, 0
        file_path = os.path.join(root_dir, file_name)
        print(file_path)
        save_dir = os.path.join(self.parsed_path, root_dir[self.split_idx: ])
        name = os.path.splitext(file_name)[0]

        extractor = LoopExtractor()
        verify_loops = ForLoopChecker()
        func_call_checker = FuncCallChecker()

        with open(file_path, 'r+') as f:
            
            try:
                code = f.read()
            except UnicodeDecodeError:
                return 0, 0, False

            code = LINE_COMMENT_RE.sub('\n', code)
            code = MULTILINE_COMMENT_RE.sub('', code)

            extractor.extract_loops(code)

            pragmas = list(map(lambda pragma: None if pragma is None else Pragma(pragma), extractor.omp_pragmas))
            loops = list(map(lambda loop: self.parse(file_path, loop), extractor.loops))

            for idx, (pragma, loop) in enumerate(zip(pragmas, loops)):
                verify_loops.reset()
                func_call_checker.reset()

                if loop is None:
                    log('fail.txt', f'file: {file_path}\nPragma: {pragmas[idx]}\n{extractor.loops[idx]}\n==========\n')
                    continue

                verify_loops.visit(loop)
                if verify_loops.found:  # undesired tokens found
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
                print(code + "\n===========\n")
                self.save(os.path.join(save_dir, f"{name}{'_neg_' if pragma is None else '_pos_'}{idx}.pickle"), pragma, loop, code)

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
            
            for root, dirs, files in os.walk(os.path.join(omp_repo, repo_name)):
                for file_name in files:
                    file_path = os.path.join(root, file_name)
                    ext = os.path.splitext(file_name)[1].lower()
                    
                    if ext in self.file_extensions:
                        pos, neg, is_parsed = self.parse_file(root, file_name, exclusions)

                        if pos is not None:
                            total_pos += pos
                            total_neg += neg

                        if not is_parsed:
                            num_failed += 1

                        total_files += 1

            if idx % (5) == 0:
                log('success_logger.txt', "{:20}{:10}   |   {:20} {:10}\n\n".format("files processed: ", total_files, "failed to parse: ", num_failed))
                print("{:20}{:10}   |   {:20} {:10}".format("files processed: ", total_files, "failed to parse: ", num_failed))
                print("{:20}{:10}   |   {:20} {:10}".format("pos examples: ", total_pos, "neg examples: ", total_neg))
                print(f'exclusions: {exclusions}\n')

        return total_pos, total_neg, exclusions, total_files, num_failed





# parser = CppLoopParser('../repositories_openMP', '../cpp_loops')
parser = CppLoopParser('../asd', 'c_loops2')

# data = parser.load('/home/talkad/Downloads/thesis/data_gathering_script/parsers/c_loops2/1/canny_pos_0.pickle')
# print(f'pragma: {data.omp_pragma}')
# print('code:\n')
# print(data.textual_loop)

total = parser.scan_dir()
print(total)












# extractor = LoopExtractor()
# with open('/home/talkad/Downloads/thesis/data_gathering_script/asd/1/canny.cpp', 'r') as f:
#     code = f.read()
#     code = LINE_COMMENT_RE.sub('\n', code)
#     code = MULTILINE_COMMENT_RE.sub('', code)

#     extractor.extract_loops(code)

#     pragmas = extractor.omp_pragmas
#     for pragma, loop in zip(pragmas, extractor.loops):
#         print("Pragma: " + ('None' if pragma is None else pragma))
#         print(loop + "\n==================\n")




# with open('/home/talkad/Downloads/thesis/data_gathering_script/asd/1/canny.cpp', 'r') as f:
#     code = f.read()
#     code = LINE_COMMENT_RE.sub('\n', code)
#     code = MULTILINE_COMMENT_RE.sub('', code)

#     extract_for(code)
#     parser = CParser()
#     for loop in loops:

#         try:
#             loop = 'int main() {\n' + loop + '\n}'
#             ast = parser.parse(loop)
#             print('yeah')
#             # ast.show()
#             print(ast.ext[0].body.block_items[0])
#             break
#         except pycparser.plyparser.ParseError as e: 
#             print('aaa')
#             print(loop)
#             print(e)
#             # print(ast)
#     # print(loops)



# src = r'''
# int main(void)
# {
# for (int row = 0; row < height; row++)
#   function("hello world");
# }
# '''

# parser = CParser()
# ast = parser.parse(src)
# ast.show()



# import pickle
# def load(file_path):
#     '''
#     Load OmpLoop structure from file_path
#     '''
#     with open(file_path, 'rb') as f:
#         return pickle.load(f)

# data = load('/home/talkad/Downloads/thesis/data_gathering_script/c_loops/357r4bd/2d-heat/src/openmp-2dheat_pos_0.pickle')
# from pycparser.c_ast import Pragma

# print(Pragma('hi there'))
# print(f'pragma: {data.omp_pragma}')
# # print('code:\n')
# # print(data.textual_loop)
