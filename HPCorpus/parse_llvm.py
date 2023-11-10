import os
import json
from tqdm import tqdm
# import dask
# import dask.bag as db
import concurrent.futures
import HPCorpus.preprocess.preprocess as preprocess
import parse_tools 
import shutil
import tempfile
import logging
import subprocess
from subprocess import Popen, PIPE
import random
import string
import multiprocessing


# logging.basicConfig(filename='llvm.log', format='%(asctime)s - %(levelname)s - %(name)s -   %(message)s',datefmt='%d/%m/%Y %H:%M:%S',level=logging.INFO)
# logger = logging.getLogger(__name__)


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
            # logger.info(f'start parsing file {json_file}')
            dataset = []
            # read json and create process the data
            with open(os.path.join(self.data_dir, json_file), 'r') as f:
                for line in tqdm(f):

                    try:
                        js = json.loads(line.strip())
                    except:
                        continue

                    if 'content' not in js:
                        continue
                    
                    repo = js['repo_name'].split('/')
                    file = js['path']
                    
                    # Similarly to Codex and CodeParrot, very large (>1MB) and very short (<100 tokens) files were filtered out
                    # avoid memory issues
                    num_tokens = len(js['content'].split())
                    if num_tokens < 100 or num_tokens > 10**6:
                        continue

                    funcs = []

                    # The C code beneath the tree-sitter throws a memory exception when attempting to allocate memory for excessively long code.
                    manager = multiprocessing.Manager()
                    result_dict = manager.dict()
                    process = multiprocessing.Process(target=preprocess.extract_code_struct, args=(js['content'], self.lang, result_dict))

                    process.start()
                    process.join()
                    if 'result' in result_dict:
                        funcs = result_dict['result']

                    if not funcs: # i6f there are no functions
                        continue

                    for curr_idx, func in enumerate(funcs):
                        func_name, func_code = func
                        func_code = preprocess.remove_comments(func_code, lang=self.lang)
                        # logger.info(f'parse function {func_name} at {repo} - {file}')

                        # append all function declaration into the current function code
                        func_defs = ''
                        for _, other_func in funcs[:curr_idx]+funcs[curr_idx+1:]:
                            decl = preprocess.get_func_declaration(other_func)

                            if len(decl.split('\n')) > 2:
                                continue

                            func_defs += decl + '\n'

                        mem_usage = self.get_mem_usage(func_code)
                        # llvm = self.get_llvm_ir(code, lang)
                        func_hash = preprocess.get_hash(func_code)

                        dataset.append({'username': repo[0],
                                        'repo': repo[1],
                                        'path': file,
                                        'function': func_name,
                                        'code': func_code,
                                        'func_defs': func_defs,
                                        'llvm': '', # llvm,
                                        'hash': func_hash,
                                        'memory': mem_usage
                        })


                # write the dataset into json
                with open(os.path.join(self.save_dir, f"{preprocess.get_filename(json_file)}.jsonl"), 'w') as data_f:
                    for sample in dataset:
                        data_f.write(json.dumps(sample) + '\n')
            
        
        samples = os.listdir(self.data_dir)

        for sample in tqdm(samples):
            print(sample)
            parse_json(sample)

        # with concurrent.futures.ThreadPoolExecutor(max_workers=160) as executor:
        #     executor.map(parse_json, samples)

        # samples = os.listdir(self.data_dir)
        # samples_bag = db.from_sequence(samples)
        # processed_data = samples_bag.map(parse_json)
        # processed_data.compute()


# parser = LLVMParser('/tier2/bgu/bigQuery_repos/c', '/tier2/bgu/HPCorpus/c', lang='c')

parser = LLVMParser('/home/1010/talkad/Downloads/Fortran', '/home/1010/talkad/Downloads/Fortran_HPCorpus', lang='fortran')
parser.iterate_corpus()


