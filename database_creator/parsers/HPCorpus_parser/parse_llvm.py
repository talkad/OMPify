import os
import json
from tqdm import tqdm
from pqdm.processes import pqdm
from joblib import Parallel, delayed
import preprocess
import parse_tools 
import shutil
import tempfile
import logging
import subprocess
from subprocess import Popen, PIPE
import time


download_path = '/mnt/c/Users/tal74/Downloads/'
folders = ['cpp1','cpp2','cpp3','cpp4','cpp5']


logging.basicConfig(filename='llvm.log', format='%(asctime)s - %(levelname)s - %(name)s -   %(message)s',datefmt='%d/%m/%Y %H:%M:%S',level=logging.INFO)
logger = logging.getLogger(__name__)


C_TO_IR_COMMAND = ["clang", "-c", "-emit-llvm", "-S", "-g1", "-Oz", "code.c", "-o", "code.ll", "-std=c17", "-Xclang", "-disable-O0-optnone", "-Wno-narrowing"]
CPP_TO_IR_COMMAND =  ["clang++", "-c", "-emit-llvm", "-S", "-g1", "-Oz", "code.cpp", "-o", "code.ll", "-std=c++17", "-Xclang", "-disable-O0-optnone", "-Wno-narrowing"]
Fortran_TO_IR_COMMAND = ["flang"] # not working

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
        filename = f"code.{'f90' if lang=='fortran' else lang}"

        with open(filename, "w") as code_f:
            code = preprocess.add_headers(code, lang=lang)
            code_f.write(code)

        subprocess.run(Code2IR[lang], stderr=subprocess.DEVNULL)
        os.remove(filename)
        
        llvm_ir = ''

        if os.path.exists('code.ll'):
            with open('code.ll', 'r') as f:
                llvm_ir = f.read()

            os.remove('code.ll')
        
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
        info = {'total': 0, 'total_llvm': 0}

        def parse_json(json_file, info, lang='cpp'):             

            # read json and create process the data
            with open(os.path.join(self.data_dir, json_file), 'r') as f:
                for line in tqdm(f):
                    js = json.loads(line.strip())

                    if 'content' not in js:
                        continue
                    
                    repo = js['repo_name'].split('/')
                    file = js['path']

                    funcs = preprocess.extract_code_struct(js['content'])
                    dataset = []

                    for curr_idx, func in enumerate(funcs):
                        func_name, func_code = func
                        func_code = preprocess.remove_comments(func_code)
                        # logger.info(f'parse function {func_name} at {repo} - {file}')

                        info['total'] += 1

                        # append all function declaration into the current function code
                        code = ''
                        for _, other_func in funcs[:curr_idx]+funcs[curr_idx+1:]:
                            decl = preprocess.get_func_declaration(other_func)

                            if len(decl.split('\n')) > 2:
                                continue

                            code += preprocess.get_func_declaration(other_func) + '\n'
                        code += func_code

                        mem_usage = self.get_mem_usage(func_code)
                        llvm = self.get_llvm_ir(code, lang)

                        if llvm:
                            info['total_llvm'] += 1

                        dataset.append({'username': repo[0],
                                        'repo': repo[1],
                                        'path': file,
                                        'function': func_name,
                                        'code': func_code,
                                        'llvm': llvm,
                                        'hash': preprocess.get_hash(func_code),
                                        'memory': mem_usage
                        })

                    # write the dataset into json
                    with open(os.path.join(self.save_dir, f"{preprocess.get_filename(json_file)}_{preprocess.get_filename(file.split('/')[-1])}.json"), 'w') as data_f:
                        for sample in dataset:
                            data_f.write(json.dumps(sample) + '\n')
                    print(info)
            
        samples = os.listdir(self.data_dir)
        for sample in tqdm(samples):
            parse_json(sample, info)

        # pqdm(samples, parse_json, n_jobs=1)

        print(info)



code = "\nstruct piw::ranger_t::impl_t: pic::lckobject_t\n{\n    impl_t(piw::clockdomain_ctl_t *clock_domain, const piw::cookie_t &output): ctl_(output), data_(&ctl_,clock_domain)\n    {\n    }\n\n    virtual ~impl_t()\n    {\n    }\n\n    ctl_filter_t ctl_;\n    data_filter_t data_;\n}"
with open('code.cpp', 'w') as f:
    f.write(code)

# parser = LLVMParser('/mnt/c/Users/tal74/Downloads/cpp1', '/home/talkad/OpenMPdb/database_creator/asd/cpp1')
# parser.iterate_corpus()
