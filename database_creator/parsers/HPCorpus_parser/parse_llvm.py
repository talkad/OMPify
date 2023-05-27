import os
import json
from tqdm import tqdm
import preprocess
from subprocess import Popen, PIPE


class LLVMParser:
    def __init__(self, data_dir, save_dir, lang='C'):
        self.data_dir = data_dir
        self.save_dir = save_dir
        self.lang = lang

    def save(self, save_dir, sample):
        os.makedirs(save_dir, exist_ok=True)
        os.chdir(save_dir)

        with open(f'code.c', 'w') as f:
            f.write(sample)

    def convert_llvm(self, save_dir):
        script = '/home/talkad/Downloads/thesis/data_gathering_script/database_creator/parsers/HPCorpus_parser/convert_llvm.sh'
        p = Popen([script, save_dir], stdin=PIPE, stdout=PIPE, stderr=PIPE)
        _, error = p.communicate()
        # print("Script error:", error.decode('utf-8'))

    def parse(self, repo, file, code):
        code = preprocess.remove_comments(code)
        funcs = preprocess.extract_funcs(code)

        for func_name, func in funcs:
            code = preprocess.func_injection(func)
            save_dir = os.path.join(self.save_dir, repo, file, func_name)
            self.save(save_dir, code)
            self.convert_llvm(save_dir)
            # return


    def iterate_corpus(self):

        for json_file in tqdm(os.listdir(self.data_dir)):
            with open(os.path.join(self.data_dir, json_file), 'r') as f:
                for line in f:
                    js = json.loads(line.strip())

                    if 'content' not in js:
                        continue

                    repo = js['repo_name'].split('/')
                    repo = f'{repo[1]}##{repo[0]}'
                    file = js['path']
                    file = file.replace('/','##')
                    code = js['content']

                    self.parse(repo, file, code)
                    # return


# parser = LLVMParser('/home/talkad/shared/nadavsc/c', '/home/talkad/shared/LLVM/c')
parser = LLVMParser('/home/talkad/shared/nadavsc/c', '/home/talkad/LIGHTBITS_SHARE/studies/llvm/c')

parser.iterate_corpus()