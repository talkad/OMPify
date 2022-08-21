from pycparser.c_ast import *
import os
import pickle
import xml.etree.ElementTree as ET

class XMLParsingException(Exception):
    '''
    Exception raised for unhandeled xml tags
    '''
    def __init__(self, tag, message="unrecognized xml tag"):
        self.tag = tag
        self.message = f'{message}: {self.tag}'
        super().__init__(self.message)

class ParserVisitor:
    '''
    Parse xml format recursively into ast (defined by c_ast)
    '''

    # implement jumping table

    def __init(self):
        self.irrelevant_tags = ['end-do-stmt', 'end-do', 'block-do-construct', 'do-construct']

    def visit(self, xml_tree):
        tag = xml_tree.tag
        
        if tag == 'pragma':             return self.visit_pragma(xml_tree)
        elif tag == 'loop':             return self.visit_loop(xml_tree)
        elif tag in self.irrelevant_tags:
        else:                           raise XMLParsingException(tag)

    def visit_pragma(self, xml_tree):
        indexer, lower, upper, step = 'i', 0, 0, 1

        for elm in xml_tree:
            if elm.tag == 'do-variable':
                indexer = elm.attrib['id']
            elif elm.tag == 'lower-bound':
                for child in elm: 
                    lower = self.visit(child)
            elif elm.tag == 'upper-bound':
                for child in elm: 
                    upper = self.visit(child)

    def visit_loop(self, xml_tree):
        indexer, lower, upper, step = 'i', 0, 0, 1
        body = ''

        for elm in xml_tree:
            if elm.tag == 'header':
                for idx in elm:
                    if idx.tag == 'index-variable'
                        indexer, lower, upper, step = self.get_for_info(idx)
            elif elm.tag == 'body':
                pass

        

class OmpLoop:
    def __init__(self, omp_pragma, loop):
        self.omp_pragma = omp_pragma # omp pragma associated with the given loop
        self.loop = loop             # xml format representing AST structure of loop


class fASTParser:
    def __init__(self, omp_repo, ast_repo):
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

    def create_Pragma(self, pragma):
        '''
        Create c_ast pragma struct
        '''
        return Pragma(omp.omp_pragma[2:])

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


parser = fASTParser('../asdasd', '../asdasd2')
# parser = CLoopParser('../asd', '../c_loops2')

# data = parser.load('c_loops/123/example_pos_0.pickle')
# print(data.loop)
omp = parser.load('/home/talkad/Downloads/thesis/data_gathering_script/fortran_parser/Refine_Derefine_pos_0.pickle')
# with open('text.xml', 'wb') as a:
#     ET.ElementTree(omp.loop).write(a)

xml_parser = ParserVisitor()
xml_parser.visit(omp.loop)

# print(omp.omp_pragma)

# total = parser.scan_dir()
# print(total)