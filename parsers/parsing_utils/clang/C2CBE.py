from injector import Injector

class C2CBE:
    '''
    Convert c-like codes to llvm-IR and back to C
    '''
    def __init__(self, dir_path, lang='C'):
        self.dir_path = dir_path
        self.lang = lang
        self.injector = Injector()

    def inject_code(self, code):
        code += '#include \"patch.h\"\n
        return self.injector.inject(code)

    def convert2cbe(self, repo_path):
        #
        pass

    def scan_dir(self):
        pass
