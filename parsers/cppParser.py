import os
from parsers.parser import Parser
from parsers.fake_headers import fake
import pickle
import re
from threading import Thread
import tempfile
import clang.cindex as clang
from functools import reduce

FUNC_NAME = 'my_awesome_function'
# INCLUDES_RE = re.compile("^#include(.+)$", re.MULTILINE)
LINE_COMMENT_RE = re.compile("//.*?\n" )
MULTILINE_COMMENT_RE = re.compile("/\*.*?\*/", re.DOTALL)

def remove_whitespaces(line):
    flag = True

    while flag:
        if line.startswith((' ', '\t')):
            line = line[1:]
        elif line.startswith('\\t'):
            line = line[2:]
        else:
            flag = False

    return line
    # return reduce(lambda acc,cur: acc + cur if cur not in [' ', '\t'] else acc, line)

def is_omp_pragma(line):
    '''
    Returns true if the given line of code is pragma
    '''
    sub_line = line.lstrip().lower()

    return sub_line.startswith('#pragma ') and ' omp' in line


class LoopExtractor:
    '''
    Implements the for-loop extraction functionality 
    '''

    def __init__(self):
        self.loops = []
        self.omp_pragmas = []
        self.pragma = None

    def is_unique_node(self, node):
        '''
        Structure of the generated node:
            CursorKind.UNEXPOSED_EXPR
                CursorKind.DECL_REF_EXPR
                    CursorKind.OVERLOADED_DECL_REF  ==  @param{FUNC_NAME}
                CursorKind.STRING_LITERAL  ==  "#pragma omp ..."

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
                        return remove_whitespaces(ch.spelling[1:][:-1])
                else:
                    return None

    def extract_loops(self, cursor):
        '''
        Extract all loops and pragmas from given program
        '''         
        if cursor.location.file is not None and not cursor.location.file.name.endswith(".cpp"):
            return      # ignore includes code

        for ch in cursor.get_children():
            literal = self.is_unique_node(ch)

            if literal is not None and len(literal) > 2 and is_omp_pragma(literal) and " for" in literal:
                self.pragma = literal

            elif ch.kind is clang.CursorKind.FOR_STMT:
                self.omp_pragmas.append(self.pragma)
                self.loops.append(ch)
                self.pragma = None
                
            self.extract_loops(ch)

    def ast2code(self, cursor):
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

        if len(code) > 0 and '}' not in code[-1] and not code[-1].endswith(';'):
            code[-1] += ';'

        return '\n'.join(code)

    def print_ast_nodes(self, cursor, depth=0):
        if cursor.location.file is not None and not cursor.location.file.name.endswith(".cpp"):
            return      # ignore includes code

        print("  " * depth + str(cursor.kind) + str(cursor.location) + '\n')

        for ch in cursor.get_children():
            self.print_ast_nodes(ch, depth + 1)

class CppLoopParser(Parser):
    def __init__(self, repo_path, parsed_path):
        super().__init__(repo_path, parsed_path, ['.cpp']) # the .h files will be gathered from cpp_header.txt

    def load(self, file_path):
        '''
        Load OmpLoop structure from file_path
        the AST property saves as None because it cannot be pickled, so the textual code will be parsed again
        '''

        pass
        # with open(file_path, 'rb') as f:
        #     return pickle.load(f)
        return

    def is_bad_case(self, node):
        '''
        Returns true if the node contains atomic or critical section
        '''
        extractor = LoopExtractor()

        children = node.get_children()
        pragma = extractor.is_unique_node(node)
        if pragma is not None:

            if len(pragma) > 2 and is_omp_pragma(pragma):

                if "atomic" in pragma or "barri" in pragma or "critical" in pragma:
                    return True
         
        return any([self.is_bad_case(ch) for ch in children])

    def is_empty_loop(self, code):
        '''
        when the libclang parser fail to parse code structure it just doesnt include it in the AST
        '''
        # num = 0
        # last_child = None

        # for ch in node.get_children():
        #     last_child = ch

        # for _ in last_child.get_children():
        #     num += 1

        # return num == 0

        return code.count(';') < 3

    def pragma2func(self, code):
        '''
        The clang parser ignores the pragmas in code.
        we will bypass this issue by wrapping the pragma with a unique function call.
        '''
        code_buf = []
        
        for line in code.split('\n'):
            if is_omp_pragma(line):
                code_buf.append(f'{FUNC_NAME}(\"{line}\");')
            else:
                code_buf.append(line)

        return '\n'.join(code_buf)

    def func2pragma(self, code):
        '''
        Restore the original code
        '''
        code_buf = []
        RE_FUNC = re.compile(r'my_awesome_function\(\"(.*?)\"\);')
        
        for line in code.split('\n'):
            pragma = re.findall(RE_FUNC, line)

            if len(pragma) == 0:
                code_buf.append(line)
            else:
                code_buf.append(pragma[0])

        return '\n'.join(code_buf)


    def create_ast(self, file_path, code_buf, result):
        index = clang.Index.create()
        
        repo_name = file_path[len(self.repo_path + self.root_dir) + 2:]
        repo_name = repo_name[:repo_name.find('/') ]
        args = ['-nostdinc', '-w', '-E', r'-I' + fake.FAKE_DIR]

        _, headers, _ = fake.get_headers(fake.REPOS_DIR, repo_name)
        for header in list(headers)[:150]:
            args.append(r'-I' + os.path.join(fake.REPOS_DIR, repo_name, header))

        # try:
        with tempfile.NamedTemporaryFile(suffix='.cpp', mode='w+') as tmp, open(file_path, 'r') as f:    
            code = f.read() 
            code = self.pragma2func(code)
            tmp.write(code)
            tmp.seek(0)
            ast = index.parse(tmp.name, args=args)
            result.append(ast.cursor)
            # LoopExtractor().print_ast_nodes(ast.cursor)

        # except pycparser.plyparser.ParseError as e:  
        #     print(f'Parser Error: {file_path} ->\n {e}\n')
        #     return
        # except Exception as e:  
        #     print(f'Unexpected Error: {file_path} ->\n {e}\n')
        #     return   

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

    def parse_file(self, root_dir, file_name, exclusions):
        '''
        Parse the given file into ast and extract to loops associated with omp pargma (or without)
        '''
        pos, neg = 0, 0
        file_path = os.path.join(root_dir, file_name)
        save_dir = os.path.join(self.parsed_path, root_dir[self.split_idx: ])
        name = os.path.splitext(file_name)[0]

        extractor = LoopExtractor()
        

        with open(file_path, 'r+') as f:
            
            try:
                code = f.read()
            except UnicodeDecodeError:
                return 0, 0, False

            ast = self.parse(file_path, code)

            if ast is None:                 # file parsing failed
                return 0, 0, False

            extractor.extract_loops(ast)

            for idx, (pragma, loop) in enumerate(zip(extractor.omp_pragmas, extractor.loops)):

                if self.is_bad_case(loop):  # undesired     tokens found
                    exclusions['bad_case'] += 1
                    continue
                
                code = extractor.ast2code(loop)
                code = LINE_COMMENT_RE.sub('', code)
                code = MULTILINE_COMMENT_RE.sub('', code)
                if self.is_empty_loop(code):
                    exclusions['empty'] += 1
                    continue

                if code in self.memory:
                    exclusions['duplicates'] += 1
                    continue
                
                # print(pragma, '\n', code , '\n================\n')

                self.create_directory(save_dir) 
                self.memory.append(code)
                self.save(os.path.join(save_dir, f"{name}{'_neg_' if pragma is None else '_pos_'}{idx}.pickle"), pragma, None, self.func2pragma(code))

                if pragma is None:
                    neg += 1
                else:
                    pos += 1

            return pos, neg, True

 
# parser = CppLoopParser('../repositories_openMP', '../cpp_loops')
# parser = CppLoopParser('../asd', 'c_loops2')

# data = parser.load('/home/talkad/Downloads/thesis/data_gathering_script/c_loops/357r4bd/2d-heat/src/openmp-2dheat_pos_0.pickle')
# print(f'pragma: {data.omp_pragma}')
# print('code:\n')
# print(data.textual_loop)


# total = parser.scan_dir()
# print(total)

# (6952, 24098, {'bad_case': 2501, 'empty': 2481, 'duplicates': 194998, 'func_calls': 0}, 14421, 248)

