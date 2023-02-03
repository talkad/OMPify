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
        p = Popen([self.metadata['script_path'], root, file_name, f'{name}.ll'], stdin=PIPE, stdout=PIPE, stderr=PIPE)
        res, err = p.communicate()

    def create_env(self, repo_path, temp_path):
        shutil.copytree(repo_path, temp_path)

        # copy the header patch into every inner directory
        shutil.copyfile(self.metadata['patch_path'], os.path.join(temp_path, 'patch.h'))
        for idx, (root, dirs, files) in enumerate(os.walk(temp_path)):
            for d in dirs:
                shutil.copyfile(self.metadata['patch_path'], os.path.join(root, d, 'patch.h'))

    def keep_cbe(self, temp_path):
        shutil.move(temp_path, self.metadata['cbe_dir'])

        # remove unrelevant files
        for idx, (root, dirs, files) in enumerate(os.walk(self.metadata['cbe_dir'])):
            for file in files:
                # print('remove', file)
                if not file.endswith('.cbe.c'):
                    # print('yes')
                    os.remove(os.path.join(root, file))

    def convert2cbe(self, repo_path, repo_name):
        temp_path = os.path.join(self.metadata['temp_dir'], repo_name)

        # create repo env
        self.create_env(repo_path, temp_path)

        for idx, (root, dirs, files) in enumerate(os.walk(temp_path)):
            for file_name in files:
                ext = os.path.splitext(file_name)[1].lower()

                if ext in self.langs:
                    file_path = os.path.join(root, file_name)

                    with open(file_path, 'r+') as f:
                        original_code = f.read()
                        updated_code = self.inject_code(original_code)
                        f.truncate(0)
                        f.seek(0)
                        f.write(updated_code)

                    # compile
                    self.run_script(root, file_name)

                    with open(file_path, 'w') as f:
                        f.truncate(0)
                        f.seek(0)
                        f.write(original_code)

        # keep just the cbe.c files
        self.keep_cbe(temp_path)
        shutil.rmtree(self.metadata['temp_dir'])

    def scan_dir(self):
        # create cbe directory 
        if os.path.exists(self.metadata['cbe_dir']):
            shutil.rmtree(self.metadata['cbe_dir'])
        os.mkdir(self.metadata['cbe_dir'])

        for repo in os.listdir(self.dir_path):
            print('repo:', repo)
            self.convert2cbe(os.path.join(self.dir_path, repo), repo)


cc = C2CBE('/home/talkad/OpenMPdb/asd', '/home/talkad/OpenMPdb/parsers/parsing_utils/clang/llvm_metadata.json', langs=['.cpp'])
cc.scan_dir()
# cc.convert2cbe('/home/talkad/Downloads/thesis/data_gathering_script/asd')

# os.chdir('/home/talkad/OpenMPdb/temp/folder')       
# cc.run_script('/home/talkad/OpenMPdb/temp/folder', 'f.cpp')

