from injector import Injector
from subprocess import Popen, PIPE
import shutil
import json
import os

class C2CBE:
    '''
    Convert c-like codes to llvm-IR and back to C
    '''
    def __init__(self, dir_path, langs=['c', 'h']):
        self.current_dir = os.getcwd()
        self.dir_path = dir_path
        self.langs = langs
        self.injector = Injector()
        self.metadata = self.read_metadata()

    def read_metadata(self):
        with open('/home/talkad/Downloads/thesis/data_gathering_script/parsers/parsing_utils/clang/llvm_metadata.json', 'r') as f:
            metadata = json.load(f)

        return metadata

    def inject_code(self, code):
        code += '#include \"patch.h\"\n'
        return self.injector.inject(code)

    def run_script(self, file_name):

        name = file_name[:file_name.rfind('.')]
        p = Popen([self.metadata['script_path'], file_name, f'{name}.ll'], stdin=PIPE, stdout=PIPE, stderr=PIPE)
        _, err = p.communicate()

        return err

    def convert2cbe(self, repo_path):
        temp_path = self.metadata['temp_dir']

        # create repo env
        shutil.copytree(repo_path, temp_path)

        for idx, (root, dirs, files) in enumerate(os.walk(temp_path)):
            for file_name in files:
                ext = os.path.splitext(file_name)[1].lower()

                if ext in self.langs:
                    file_path = os.path.join(root, file_name)

                    with open(file_path, 'w+') as f:
                        original_code = f.read()
                        updated_code = self.inject_code(original_code)
                        f.seek(0)
                        f.write(updated_code)

                        # compile
                        os.chdir(root)       
                        self.run_script(file_name)

                        f.seek(0)
                        f.write(original_code)

                os.chdir(self.current_dir)

        # keep cbe.c files
        # remove all the rest

    def scan_dir(self):

        for repo in os.listdir(self.dir_path):
            print('repo:', repo)
            self.convert2cbe(os.path.join(self.dir_path, repo))


cc = C2CBE('/home/talkad/Downloads/thesis/data_gathering_script/asd')
cc.scan_dir()
# cc.convert2cbe('/home/talkad/Downloads/thesis/data_gathering_script/asd')