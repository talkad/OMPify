from abc import ABC, abstractmethod
from pycparser import c_ast
import os
import json
import pickle
from typing import Union
from tqdm import tqdm
# import ray


def log(file_name, msg):
    with open(file_name, 'a') as f:
        f.write(f'{msg}\n')


class ASTs:
    '''
    Define a class that later will be dumped into pickle file
    '''
    def __init__(self, ast_loop, inner_functions):
        self.ast_loop = ast_loop 
        self.inner_functions = inner_functions


class OmpLoop:
    '''
    Define a class that later will be dumped into pickle file
    '''
    def __init__(self, omp_pragma: Union[str, c_ast.Pragma], ast_loop: Union[None, c_ast.For], inner_functions: list, textual_loop: str):
        self.omp_pragma = omp_pragma            # omp pragma associated with the given loop
        self.ast_loop = ast_loop                # ast format representing AST structure of loop
        self.textual_loop = textual_loop        # textual representation of code
        self.inner_functions = inner_functions  # function AST struct. which called inside the given loop (type list[c_ast.FuncDef])


class Parser(ABC):
    '''
    Abstract class that defines the general behavior of our parsers
    '''

    def __init__(self, repo_path, parsed_path, file_extensions):
        self.data_indexer = 0
        self.id2path = {}
        self.root_dir = os.getcwd()
        self.repo_path = repo_path
        self.parsed_path = parsed_path
        self.file_extensions = file_extensions
        self.split_idx = len(os.path.join(self.root_dir, self.repo_path)) + 1
        self.memory = []
 
    def create_directory(self, dirs):
        '''
        Create directories if not exists
        '''
        if not os.path.exists(dirs):
            os.makedirs(dirs)

    def save(self, file_path: os.path, omp_loop):
        '''
        Save OmpLoop structure into file_path:
            textual file containing the code
            pickle file containing the ast loop and the relevant func. declerations
            pickle file contatining the pragma only if exists

        save unique_id for each example
        '''
        self.id2path[self.data_indexer] = file_path
        self.data_indexer += 1

        with open(os.path.join(file_path, 'ast.pickle'), 'wb') as f:
            asts = ASTs(omp_loop.ast_loop, omp_loop.inner_functions)
            pickle.dump(asts, f)

        with open(os.path.join(file_path, f'code{self.file_extensions[0]}'), 'w') as f:
            f.write(omp_loop.textual_loop)

        if omp_loop.omp_pragma is not None:
            with open(os.path.join(file_path, 'pragma.pickle'), 'wb') as f:
                pickle.dump(omp_loop.omp_pragma, f)


    def load(self, file_path):
        '''
        Load OmpLoop structure from file_path
        '''
        omp_pragma = None

        with open(os.path.join(file_path, 'ast.pickle'), 'rb') as f:
            asts = pickle.load(f)

        with open(os.path.join(file_path, f'code{self.file_extensions[0]}'), 'r') as f:
            code = f.read()

        try:
            with open(os.path.join(file_path, 'pragma.pickle'), 'rb') as f:
                omp_pragma = pickle.load(f)
        except FileNotFoundError:
            pass

        return OmpLoop(omp_pragma, asts.ast_loop, asts.inner_functions, code)

    
    def join_splited_lines(self, code_buf, delimiter='\\'):
        '''
        Several c files are splitting lines of code using \ token. For instance:
            #pragma omp parallel for private(i, test, x, y) \
                default(shared) schedule(dynamic)
        pycparser fail to process this files. So we update this lines to be a single line.

        OR

        Several Fortran files are splitting lines of code using & token. 
        fparser fails to process this files. So we update this lines to be a single line.
        '''
        code = []
        splitted_line = False

        for line in code_buf.split('\n'):
            if not splitted_line and len(line) > 0 and line[-1] == delimiter:
                code.append(line[:-1])
                splitted_line = True
            elif splitted_line and len(line) > 0 and line[-1] == delimiter:
                code[-1] += line[:-1]
            elif splitted_line:
                code[-1] += line
                splitted_line = False
            else:
                code.append(line)

        return '\n'.join(code)

    @abstractmethod
    def parse(self, file_path, code_buf, opt=None):
        '''
        Create the AST of a given code snippet

        Parameters:
            file_path - the path of the file to be parsed
            code_buf  - textual representation of the code
            future    - a mutable struct that on success will contain the AST
        Returns:
            Return the AST of a given code. None on failure
        '''
        pass

    @abstractmethod
    def parse_file(self, root_dir, file_name, exclusions, opt=None):
        '''
        Parse file and save for loops into pickles.

        Parameters:
            root_dir - the path absolute path the directory that contains file_name
            file_name - the name of the file to be parsed
            exclusions - dictionary that contains stats about excluded examples
            opt - optional argument
        Returns:
            returns a sequence of three values <num_positive_examples>, <num_negative_examples>, <parsing_status>
        '''
        pass

    def scan_dir(self):
        '''
        Loop over all files in repo and parse them.

        Returns:
            1. the amount of positive examples found
            2. the amount of negative examples found
            3. exclusion stats
            4. number of relevant (with respect to the file extension) files
            total_pos, total_neg, exclusions, total_files, num_failed
        '''

        total_files, num_failed = 0, 0
        total_pos, total_neg = 0, 0
        omp_repo = os.path.join(self.root_dir, self.repo_path)
        exclusions = {'bad_case': 0, 'empty': 0, 'duplicates': 0, 'func_calls':0}

        for repo in tqdm(os.listdir(omp_repo)):
            print(repo)

            for idx, (root, dirs, files) in enumerate(os.walk(os.path.join(omp_repo, repo))):
                for file_name in files:
                    ext = os.path.splitext(file_name)[1].lower()

                    if ext in self.file_extensions:
                        pos, neg, is_parsed = self.parse_file(root, file_name, exclusions)

                        if pos is not None:
                            total_pos += pos
                            total_neg += neg

                        if not is_parsed:
                            num_failed += 1
                        total_files += 1

                if idx % (10**2) == 0: 
                    print("{:20}{:10}   |   {:20} {:10}".format("files processed: ", total_files, "failed to parse: ", num_failed))
                    print("{:20}{:10}   |   {:20} {:10}".format("pos examples: ", total_pos, "neg examples: ", total_neg))
                    print(f'exclusions: {exclusions}\n')


        # write ids into json
        with open("sample_cpp.json", "w") as f:
            json.dump(self.id2path, f, sort_keys=True, indent=4, separators=(',', ': '))

        print("{:20}{:10}   |   {:20} {:10}".format("files processed: ", total_files, "failed to parse: ", num_failed))
        print("{:20}{:10}   |   {:20} {:10}".format("pos examples: ", total_pos, "neg examples: ", total_neg))
        print(f'exclusions: {exclusions}\n')

        return total_pos, total_neg, exclusions, total_files, num_failed
