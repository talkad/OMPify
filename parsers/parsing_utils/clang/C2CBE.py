from injector import Injector
from subprocess import Popen, PIPE
import shutil
import json
import os

class C2CBE:
    '''
    Convert c-like codes to llvm-IR and back to C
    '''
    def __init__(self, dir_path, meta_path, langs=['.c', '.h']):
        self.current_dir = os.getcwd()
        self.dir_path = dir_path
        self.langs = langs
        self.injector = Injector()
        self.metadata = self.read_metadata(meta_path)

    def read_metadata(self, meta_path):
        with open(meta_path, 'r') as f:
            metadata = json.load(f)

        return metadata

    def inject_code(self, code):
        code = '#include \"patch.h\"\n' + code
        return self.injector.inject(code)

    def run_script(self, root, file_name):

        name = file_name[:file_name.rfind('.')]
        # print([self.metadata['script_path'], file_name, f'{name}.ll'])
        p = Popen([self.metadata['script_path'], root, file_name, f'{name}.ll'], stdin=PIPE, stdout=PIPE, stderr=PIPE)
        res, err = p.communicate()
        # print(res)
        # print(err)

    def convert2cbe(self, repo_path):
        temp_path = self.metadata['temp_dir']

        # create repo env
        shutil.copytree(repo_path, temp_path)

        # copy the header patch into every inner directory
        shutil.copyfile(self.metadata['patch_path'], os.path.join(temp_path, 'patch.h'))
        for idx, (root, dirs, files) in enumerate(os.walk(temp_path)):
            for d in dirs:
                shutil.copyfile(self.metadata['patch_path'], os.path.join(root, d, 'patch.h'))

        for idx, (root, dirs, files) in enumerate(os.walk(temp_path)):
            for file_name in files:
                ext = os.path.splitext(file_name)[1].lower()

                if ext in self.langs:
                    file_path = os.path.join(root, file_name)
                    print(file_path)

                    with open(file_path, 'r+') as f:
                        original_code = f.read()
                        updated_code = self.inject_code(original_code)
                        f.truncate(0)
                        f.seek(0)
                        f.write(updated_code)

                    # compile
                    print(root, file_name)
                    self.run_script(root, file_name)

                    with open(file_path, 'w') as f:
                        f.truncate(0)
                        f.seek(0)
                        f.write(original_code)


        # keep cbe.c files
        # remove all the rest

        # shutil.rmtree(temp_path)

    def scan_dir(self):

        for repo in os.listdir(self.dir_path):
            print('repo:', repo)
            self.convert2cbe(os.path.join(self.dir_path, repo))
            break


cc = C2CBE('/home/talkad/OpenMPdb/asd', '/home/talkad/OpenMPdb/parsers/parsing_utils/clang/llvm_metadata.json', langs=['.cpp'])
cc.scan_dir()
# cc.convert2cbe('/home/talkad/Downloads/thesis/data_gathering_script/asd')

# os.chdir('/home/talkad/OpenMPdb/temp/folder')       
# cc.run_script('/home/talkad/OpenMPdb/temp/folder', 'f.cpp')
