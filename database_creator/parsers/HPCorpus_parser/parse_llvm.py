import os
import json
from tqdm import tqdm
from pqdm.processes import pqdm
from joblib import Parallel, delayed
import preprocess
import parse_tools 
import shutil
import pickle
import logging
from subprocess import Popen, PIPE


logging.basicConfig(filename='llvm.log', format='%(asctime)s - %(levelname)s - %(name)s -   %(message)s',datefmt='%d/%m/%Y %H:%M:%S',level=logging.INFO)
logger = logging.getLogger(__name__)


C_TO_IR_COMMAND = "clang -c -emit-llvm -S -g1 -Oz code.c -o code.ll -std=c17 -Xclang -disable-O0-optnone -Wno-narrowing"
CPP_TO_IR_COMMAND = "clang++ -c -emit-llvm -S -g1 -Oz code.c -o code.ll -std=c++17 -Xclang -disable-O0-optnone -Wno-narrowing"
Fortran_TO_IR_COMMAND = "flang" # not really working

Code2IR = {
    "c": C_TO_IR_COMMAND,
    "cpp": CPP_TO_IR_COMMAND,
    "fortran": Fortran_TO_IR_COMMAND
}




class LLVMParser:
    def __init__(self, data_dir, save_dir, lang='C'):
        self.data_dir = data_dir
        self.save_dir = save_dir
        self.lang = lang

    def save(self, save_dir, sample):
        '''
        Save @sample and its AST in @save_dir
        '''
        os.makedirs(save_dir, exist_ok=True)
        os.chdir(save_dir)

        with open(f'code.c', 'w') as code_f, open('ast.pkl', 'wb') as ast_f:
            ast = parse_tools.parse(sample, lang='c')

            code_f.write(sample)
            # pickle.dump(ast, ast_f)    # tree_sitter object cannot be pickled

    def convert_llvm(self, save_dir, lang='c'):
        '''
        Execute the clang compiler and save the intermediate representation
        '''
        os.chdir(save_dir)
        p = Popen(Code2IR[lang], stdin=PIPE, stdout=PIPE, stderr=PIPE)
        _, error = p.communicate()

        if error:
            logger.error(f'{save_dir} error:\n{error}')
        else:
            logger.info(save_dir)

    def parse(self, repo, file, func_name, code):
        '''
        Remove irrelevant parts of code (comments and includes) and then parse it
        '''
        code = preprocess.remove_comments(code)
        code = preprocess.remove_headers(code)
        code = preprocess.add_headers(code, lang='c')

        save_dir = os.path.join(self.save_dir, repo, file, func_name)

        try:
            self.save(save_dir, code)
            self.convert_llvm(save_dir)
        except Exception as e:
            # shutil.rmtree(save_dir)
            logger.error(f'file at {save_dir} failed to parse\nerror: {e}')
    
    def iterate_corpus(self):
        '''
        Iterate over the HPCorpus and for each function save the following representations:
            1. original code
            2. IR
            3. AST - can be used to produce replaced-tokens, DFG, etc.
        '''
        def parse_json(json_file): 
            with open(os.path.join(self.data_dir, json_file), 'r') as f:
                for line in f:
                    js = json.loads(line.strip())

                    if 'content' not in js:
                        continue

                    repo = js['repo_name'].split('/')
                    repo = f'{repo[1]}#{repo[0]}'
                    file = js['path']
                    code = js['content']

                    for func_name, func in preprocess.extract_code_struct(code):
                        logger.info(f'parse function {func_name} at {repo} - {file}')
                        self.parse(repo, file, func_name, func)
                        exit()

        # pqdm(os.listdir(self.data_dir), parse_json, n_jobs=1)

        # sequential
        for json_file in tqdm(os.listdir(self.data_dir)):
            parse_json(json_file)


# parser = LLVMParser('/home/talkad/shared/nadavsc/c', '/home/talkad/LIGHTBITS_SHARE/studies/llvm/c')
# parser = LLVMParser('/home/talkad/shared/nadavsc/c', '/home/talkad/Downloads/thesis/data_gathering_script/database_creator/asd/c_llvm')
parser = LLVMParser('/home/talkad/OpenMPdb/tokenizer/HPCorpus', '/home/talkad/OpenMPdb/database_creator/asd/c_llvm')


parser.iterate_corpus()
