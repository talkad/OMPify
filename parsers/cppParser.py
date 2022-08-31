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
from multiprocessing import Process, Manager
import tempfile
import clang.cindex as clang

FUNC_NAME = 'my_awesome_function'

class LoopExtractor:
    '''
    Implements the for-loop extraction functionality 
    '''

    def __init__(self):
        self.loops = []
        self.omp_pragmas = []
        self.pragma = ''

    def is_for_pragma(self, line):
        '''
        Returns true if the given line of code is pragma
        '''
        sub_line = line.lstrip().lower()

        return sub_line.startswith('#pragma ') and ' omp ' in line and ' for' in line

    def update_code(self, code):
        '''
        The clang parser ignores the pragmas in code.
        we will bypass this issue by wrapping the pragma with a unique function call.
        '''
        code_buf = []
        
        for line in code.split('\n'):
            if self.is_pragma(line):
                code_buf.append(f'{FUNC_NAME}(\"{line}\");')
            else:
                code_buf.append(line)

        return '\n'.join(code_buf)

    def is_unique_node(self, node):
        '''
        Structure of the generated node:
            CursorKind.UNEXPOSED_EXPR
                CursorKind.DECL_REF_EXPR
                    CursorKind.OVERLOADED_DECL_REF  ==  @param{FUNC_NAME}
                CursorKind.STRING_LITERAL  ==  "#pragma omp for..."

        Returns:
            The string literal if its our function call, None otherwise
        '''
        if node.kind is clang.CursorKind.UNEXPOSED_EXPR:
            children = node.get_children()
            
            for idx, ch in enumerate(children):
                if idx == 0:
                    if ch.kind is clang.CursorKind.DECL_REF_EXPR:
                        
                        for child in ch.get_children():
                            if child.kind is clang.CursorKind.OVERLOADED_DECL_REF:
                                if child.spelling != FUNC_NAME:     # not our function call
                                    return None

                elif idx == 1:
                    if ch.kind is clang.CursorKind.STRING_LITERAL:
                        return ch.spelling
                else:
                    return None

    def extract_loops(self, cursor):
        '''
        Extract all loops and pragmas from given program
        '''
        for ch in cursor.get_children():
            literal = self.is_unique_node(ch)

            if literal is not None and len(literal) > 2 and self.is_for_pragma(literal[1:][:-1]):
                self.pragma = literal[1:][:-1]

            elif ch.kind is clang.CursorKind.FOR_STMT:
                self.omp_pragmas.append(self.pragma)
                self.loops.append(ch)
                self.pragma = ''
                
            self.extract_loops(ch)

        # for sub in children:
        #     if type(sub) is FortranStructs.Comment and is_for_pragma(str(sub).lower()):
        #         self.pragma = str(sub)
        #     elif (type(sub) is FortranStructs.Block_Label_Do_Construct or type(sub) is FortranStructs.Block_Nonlabel_Do_Construct) \
        #             and not is_do_while(sub):   # classified as do loop by the parser
        #         if len(self.pragma) != 0:
        #             self.omp_pragmas.append(self.pragma)
        #             self.loops.append(sub)
        #         else:                           # check for pragma
        #             pragma = self.get_pragma(sub)
                    
        #             if pragma is not None:      # manage to get pragma
        #                 self.omp_pragmas.append(pragma)
        #                 self.loops.append(sub)

        #         self.pragma = ''
        #     elif not self.is_leaf(sub):
        #         self.extract_loops(sub)



# class CppLoopParser(Parser):
#     def __init__(self, repo_path, parsed_path):
#         super().__init__(repo_path, parsed_path, ['.cpp']) # the .h files will be gathered from cpp_header.txt

#     def create_ast(self, file_path, code_buf, result):
#         repo_name = file_path[len(self.repo_path + self.root_dir) + 2:]
#         repo_name = repo_name[:repo_name.find('/') ]
#         cpp_args = ['-nostdinc', '-w', '-E', r'-I' + os.path.join(self.root_dir, 'fake_headers', 'utils')]

#         _, headers, _ = fake.get_headers(fake.REPOS_DIR, repo_name)
#         for header in list(headers)[:150]:
#             cpp_args.append(r'-I' + os.path.join(fake.REPOS_DIR, repo_name, header))

#         try:
#             with tempfile.NamedTemporaryFile(suffix='.c', mode='w+') as tmp, open(file_path, 'r') as f:    
#                 code = f.read() 
#                 tmp.write(code)
#                 tmp.seek(0)
#                 ast = pycparser.parse_file(tmp.name, use_cpp=True, cpp_path='mpicc', cpp_args = cpp_args)
#                 result['ast'] = ast

#         except pycparser.plyparser.ParseError as e:  
#             log('error_logger.txt', f'Parser Error: {file_path} ->\n {e}\n')
#             return
#         except Exception as e:  
#             # log('error_logger.txt', f'Unexpected Error: {file_path} ->\n {e}\n')
#             return   

#     def parse(self, file_path, code_buf):
#         manager = Manager()
#         return_dict = manager.dict()
#         t = Process(target=self.create_ast, args=(file_path, code_buf, return_dict), daemon=True)

#         t.start()
#         t.join(60.0)

#         if t.is_alive():
#             t.terminate()
#             return
#         elif len(return_dict) == 0:
#             return
#         else:
#             return return_dict['ast']


#     def parse_file(self, root_dir, file_name, exclusions):
#         '''
#         Parse the given file into ast and extract to loops associated with omp pargma (or without)
#         '''
#         pos, neg = 0, 0
#         file_path = os.path.join(root_dir, file_name)
#         save_dir = os.path.join(self.parsed_path, root_dir[self.split_idx: ])
#         name = os.path.splitext(file_name)[0]

#         pfv = PragmaForVisitor()
#         verify_loops = ForLoopChecker()
#         func_call_checker = FuncCallChecker()

#         with open(file_path, 'r+') as f:
            
#             try:
#                 code = f.read()
#             except UnicodeDecodeError:
#                 return 0, 0, False

#             ast = self.parse(file_path, code)

#             if ast is None:                 # file parsing failed
#                 return 0, 0, False

#             pfv.visit(ast)
#             pragmas = pfv.pragmas + len(pfv.neg_nodes) * [None]
#             nodes = pfv.pos_nodes + pfv.neg_nodes

#             for idx, (pragma, loop) in enumerate(zip(pragmas, nodes)):
#                 verify_loops.reset()
#                 func_call_checker.reset()

#                 verify_loops.visit(loop)
#                 if verify_loops.found:  # undesired     tokens found
#                     exclusions['bad_case'] += 1
#                     continue
                
#                 generator = pycparser.c_generator.CGenerator()
#                 code = generator.visit(loop)
#                 if code in self.memory:
#                     exclusions['duplicates'] += 1
#                     continue

#                 if self.is_empty_loop(loop):
#                     exclusions['empty'] += 1
#                     continue

#                 func_call_checker.visit(loop)
#                 if func_call_checker.found:
#                     exclusions['func_calls'] += 1
                                   
#                 self.create_directory(save_dir) 
#                 self.memory.append(code)
#                 self.save(os.path.join(save_dir, f"{name}{'_neg_' if pragma is None else '_pos_'}{idx}.pickle"), pragma, loop, code)

#                 if pragma is None:
#                     neg += 1
#                 else:
#                     pos += 1

#             return pos, neg, True

#     def is_cpp_header(self, file_path):
#         code = ''
#         includes = fake.extract_includes(file_path)

#         if any([True for include in includes if not include.endswith('.h')]):
#             return True

#         try:
#             with open(file_path, 'r') as f:
#                 code = f.read()
#         except:
#             return False
        
#         return any([True for line in code.split('\n') if 'using' in line.lower() and 'namespace' in line.lower()])


#     def scan_dir(self):
#         LOGGER = 'cpp_headers.txt'

#         total_files, num_failed = 0, 0
#         total_pos, total_neg = 0, 0
#         omp_repo = os.path.join(self.root_dir, self.repo_path)
#         exclusions = {'bad_case': 0, 'empty': 0, 'duplicates': 0, 'func_calls':0}

#         # iterate over repos
#         for idx, repo_name in enumerate(os.listdir(omp_repo)):
            
#             for root, dirs, files in os.walk(os.path.join(omp_repo, repo_name)):
#                 for file_name in files:
#                     file_path = os.path.join(root, file_name)
#                     ext = os.path.splitext(file_name)[1].lower()
                    
#                     if ext in self.file_extensions:
#                         if ext == '.h' and self.is_cpp_header(file_path):
#                             log('cpp_header.txt', file_path)
#                             continue

#                         pos, neg, is_parsed = self.parse_file(root, file_name, exclusions)

#                         if pos is not None:
#                             total_pos += pos
#                             total_neg += neg

#                         if not is_parsed:
#                             num_failed += 1

#                             if ext == '.h':
#                                 log('cpp_header.txt', file_path)

#                         total_files += 1

#             if idx % (5) == 0:
#                 log('success_logger.txt', "{:20}{:10}   |   {:20} {:10}\n\n".format("files processed: ", total_files, "failed to parse: ", num_failed))
#                 print("{:20}{:10}   |   {:20} {:10}".format("files processed: ", total_files, "failed to parse: ", num_failed))
#                 print("{:20}{:10}   |   {:20} {:10}".format("pos examples: ", total_pos, "neg examples: ", total_neg))
#                 print(f'exclusions: {exclusions}\n')

#         return total_pos, total_neg, exclusions, total_files, num_failed


# # parser = CLoopParser('../repositories_openMP', '../c_loops')
# parser = CLoopParser('../asd', 'c_loops2')

# # data = parser.load('/home/talkad/Downloads/thesis/data_gathering_script/c_loops/357r4bd/2d-heat/src/openmp-2dheat_pos_0.pickle')
# # print(f'pragma: {data.omp_pragma}')
# # print('code:\n')
# # print(data.textual_loop)


# total = parser.scan_dir()
# print(total)

# # (5176, 6829, {'bad_case': 1988, 'empty': 131, 'duplicates': 53288, 'func_calls': 5907}, 21814, 10042)





def code_from_ast(cursor):
    '''
    Creates textual code for a given cursor (node)
    '''
    code = []
    line = ""
    prev_token = None

    for tok in cursor.get_tokens():
        if prev_token is None:
            prev_token = tok

        prev_location = prev_token.location
        prev_token_end_col = prev_location.column + len(prev_token.spelling)
        cur_location = tok.location
        if cur_location.line > prev_location.line:
            code.append(line)
            line = " " * (cur_location.column - 1)
        else:
            if cur_location.column > (prev_token_end_col):
                line += " "

        line += tok.spelling
        prev_token = tok

    if len(line.strip()) > 0:
        code.append(line)

    return '\n'.join(code)


def text(cursor, depth):
    for ch in cursor.get_children():
        print("  " * depth + str(ch.kind))
        text(ch, depth + 1)


index = clang.Index.create()
tu = index.parse("/home/talkad/Downloads/thesis/data_gathering_script/asd/par_omp_sort.cpp")

node = tu.cursor

extractor = LoopExtractor()
extractor.extract_loops(node)
for pragma, loop in zip(extractor.omp_pragmas, extractor.loops):
    print("pragma: ", pragma)
    print(code_from_ast(loop))


# text(node, 0)
# print(code_from_ast(node))

# CursorKind.FOR_STMT    ,     CursorKind.STRING_LITERAL    ,    CursorKind.OVERLOADED_DECL_REF