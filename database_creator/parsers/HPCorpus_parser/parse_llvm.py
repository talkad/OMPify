import os
import json
from tqdm import tqdm
import dask
import dask.bag as db
import preprocess
import parse_tools 
import shutil
import tempfile
import logging
import subprocess
from subprocess import Popen, PIPE
import random
import string

download_path = '/mnt/c/Users/tal74/Downloads/'
folders = ['cpp1','cpp2','cpp3','cpp4','cpp5']


logging.basicConfig(filename='llvm.log', format='%(asctime)s - %(levelname)s - %(name)s -   %(message)s',datefmt='%d/%m/%Y %H:%M:%S',level=logging.INFO)
logger = logging.getLogger(__name__)


C_TO_IR_COMMAND = lambda filename: ["clang", "-c", "-emit-llvm", "-S", "-g1", "-Oz", filename, "-std=c17", "-Xclang", "-disable-O0-optnone", "-Wno-narrowing"]
CPP_TO_IR_COMMAND =  lambda filename: ["clang++", "-c", "-emit-llvm", "-S", "-g1", "-Oz", filename, "-std=c++17", "-Xclang", "-disable-O0-optnone", "-Wno-narrowing"]
Fortran_TO_IR_COMMAND = lambda _: ["flang"] # not working

Code2IR = {
    "c": C_TO_IR_COMMAND,
    "cpp": CPP_TO_IR_COMMAND,
    "fortran": Fortran_TO_IR_COMMAND
}



class LLVMParser:
    def __init__(self, data_dir, save_dir, lang='c'):
        self.data_dir = data_dir
        self.save_dir = save_dir
        self.lang = lang

    def get_mem_usage(self, code):
        '''
        get the memory usage of a file containing @param:code
        '''
        with tempfile.NamedTemporaryFile() as temp_file:
            file_path = temp_file.name

            temp_file.write(bytes(code, 'utf-8'))
            temp_file.flush()

            mem_usage = os.path.getsize(file_path)

        return mem_usage

    def get_llvm_ir(self, code, lang):
        '''
        Create the LLVM IR of given code
        '''
        random_string = ''.join(random.choices(string.ascii_letters + string.digits, k=5))
        filename = f"code{random_string}"
        ext = 'f90' if lang=='fortran' else lang

        with open(f'{filename}.{ext}', "w") as code_f:
            code = preprocess.add_headers(code, lang=lang)
            code_f.write(code)

        subprocess.run(Code2IR[lang](f'{filename}.{ext}'), stderr=subprocess.DEVNULL)
        os.remove(f'{filename}.{ext}')
        
        llvm_ir = ''

        if os.path.exists(f'{filename}.ll'):
            with open(f'{filename}.ll', 'r') as f:
                llvm_ir = f.read()

            os.remove(f'{filename}.ll')
        
        return llvm_ir
        
    def iterate_corpus(self):
        '''
        Iterate over the HPCorpus and for each function save the following representations:
            1. username
            2. repo name
            3. path to file
            4. function name
            5. original code
            6. LLVM IR
            7. codes SHA-256
            8. memory usage 

            ---  AST - can be used to produce replaced-tokens, DFG, etc. (will be generated at training time)
        '''

        def parse_json(json_file):             

            # read json and create process the data
            with open(os.path.join(self.data_dir, json_file), 'r') as f:
                for line in tqdm(f):
                    js = json.loads(line.strip())

                    if 'content' not in js:
                        continue
                    
                    repo = js['repo_name'].split('/')
                    file = js['path']

                    funcs = []
                    try:
                        funcs = preprocess.extract_code_struct(js['content'], lang=self.lang)
                    except:    
                        # The C code beneath the tree-sitter throws a memory exception when attempting to allocate memory for excessively long code.
                        continue

                    if not funcs: # if there are no functions
                        continue

                    dataset = []

                    for curr_idx, func in enumerate(funcs):
                        func_name, func_code = func
                        func_code = preprocess.remove_comments(func_code, lang=self.lang)
                        # logger.info(f'parse function {func_name} at {repo} - {file}')

                        # append all function declaration into the current function code
                        # code = ''
                        # for _, other_func in funcs[:curr_idx]+funcs[curr_idx+1:]:
                        #     decl = preprocess.get_func_declaration(other_func)

                        #     if len(decl.split('\n')) > 2:
                        #         continue

                        #     code += decl + '\n'
                        # code += func_code

                        mem_usage = self.get_mem_usage(func_code)
                        # llvm = self.get_llvm_ir(code, lang)

                        dataset.append({'username': repo[0],
                                        'repo': repo[1],
                                        'path': file,
                                        'function': func_name,
                                        'code': func_code,
                                        'llvm': '', # llvm,
                                        'hash': preprocess.get_hash(func_code),
                                        'memory': mem_usage
                        })

                    # write the dataset into json
                    with open(os.path.join(self.save_dir, f"{preprocess.get_filename(json_file)}_{preprocess.get_filename(file.split('/')[-1])}.json"), 'w') as data_f:
                        for sample in dataset:
                            data_f.write(json.dumps(sample) + '\n')
            
        samples = os.listdir(self.data_dir)
        for sample in tqdm(samples):
            parse_json(sample)
        # samples = os.listdir(self.data_dir)
        # samples_bag = db.from_sequence(samples)
        # processed_data = samples_bag.map(parse_json)
        # processed_data.compute()


parser = LLVMParser('/mnt/c/Users/tal74/Downloads/c_0', '/home/talkad/OpenMPdb/database_creator/asd/c', lang='c')
parser.iterate_corpus()



# Fortran - done
# cpp - [1, ]
# c - 12 / 41 c_0
# 2130it [00:17, 136.60it/sfree(): invalid size
# Aborted[00:21, 98.67it/s]