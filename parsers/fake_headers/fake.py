import os
import re
from functools import reduce


REPOS_DIR = '/home/talkad/Downloads/thesis/data_gathering_script/git_repos'
# REPOS_DIR = '/home/talkad/Downloads/thesis/data_gathering_script/asd'
REPOS_OMP_DIR = '/home/talkad/Downloads/thesis/data_gathering_script/repositories_openMP'
FAKE_DEFINES  = '_fake_defines.h'
FAKE_TYPEDEFS = '_fake_typedefs.h'
COMMON_FAKE_DEFINES  = '_common_fake_defines.h'
COMMON_FAKE_TYPEDEFS = '_common_fake_typedefs.h'
FAKE_INCLUDE  = f'#include \"{FAKE_DEFINES}\"\n#include \"{FAKE_TYPEDEFS}\"'

FAKE_DIR = '/home/talkad/Downloads/thesis/data_gathering_script/parsers/fake_headers/utils'


def get_headers(repo_dir, repo_name):
    '''
    For a given repo, return all headers file relative path 
    '''
    headers, file_names = [], []
    path_length = len(repo_dir) + len (repo_name) + 2              # remove two '/'
    omp_repo = os.path.join(repo_dir, repo_name)

    for idx, (root, dirs, files) in enumerate(os.walk(omp_repo)):
        for file_name in files:
            ext = os.path.splitext(file_name)[1].lower()
            
            if ext == '.h':
                file_names.append(file_name)
                headers.append(os.path.join(root, file_name)[path_length: ])

    return headers, file_names


def join_splited_lines(code_buf, delimiter='\\'):
    '''
    Several c files are splitting lines of code using \ token. For instance:
        #pragma omp parallel for private(i, test, x, y) \
            default(shared) schedule(dynamic)
    pycparser fail to process this files. So we update this lines to be a single line.
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


def get_directives(code):
    '''
    Extract all 'define' and 'typedef' directives from a given code
    '''
    defines =  set()
    typedefs = set()

    # remove comments
    LINE_COMMENT_RE = re.compile("//.*?\n" )
    MULTILINE_COMMENT_RE = re.compile("/\*.*?\*/", re.DOTALL)
    code = LINE_COMMENT_RE.sub("", code)
    code  = MULTILINE_COMMENT_RE.sub("", code)

    # join splitted lines in code
    code = join_splited_lines(code)

    for directive in re.findall(r'#(\s*)define(\s*)(\w+)(\s)', code):
        defines.add(f' {directive[2]} 1')

    for directive in re.findall(r'#(\s*)define(\s*)(\w+)\((.*?)\)(.*)\n', code):
        defines.add(f' {directive[2]} 1')
        # defines.add(f' {directive[2]}({directive[3]}) 1')

    for directive in re.findall(r'(\s*)typedef(\s*)(\w+)(\s*)(\w+)(\s*);', code):
        typedefs.add(f' int {directive[4]}')

    for directive in re.findall(r'(\s*)typedef(\s*)struct(\s*){(.+?)}(.+?);', code):
        typedefs.add(f' int {directive[4]}')

    return defines, typedefs



def get_repo_directives(repo_dir, repo_name):
    '''
    Extract all 'define' and 'typedef' directives from a given repo

    Paramenters:
        repo_name   - the name of the repo all the directives extracted from
    '''

    headers, _ = get_headers(repo_dir, repo_name)
    define_set =  set()
    typedef_set = set()

    for header in headers:
        # open file and extract #define and typedef
        with open(os.path.join(repo_dir, repo_name, header), 'r')  as f:
            try:
                code = f.read()
            except UnicodeDecodeError:
                continue

            defines, typedefs = get_directives(code)
            define_set |= defines
            typedef_set |= typedefs
            
    return define_set, typedef_set


def extract_all_directives(repo_name):
    '''
    Create fake headers that contain all defines and typedefs in a given repo
    '''
    if not os.path.exists(FAKE_DIR):
        os.makedirs(FAKE_DIR)

    with open(os.path.join(FAKE_DIR, FAKE_DEFINES), 'w+') as define_file, open(os.path.join(FAKE_DIR, FAKE_TYPEDEFS), 'w+') as typedef_file:
        defines, typedefs = get_repo_directives(REPOS_DIR, repo_name)

        for define in defines:
            define_file.write(f'#define {define}\n')

        for typedef in typedefs:
            typedef_file.write(f'typedef {typedef};\n')


def extract_includes(file_path):
    '''
    Extract all #include directive from given file
    '''
    includes =  []

    with open(file_path, 'r')  as f:
        try:
            code = f.read()
        except UnicodeDecodeError:
            return []

        for directive in re.findall(r'#(\s*)include(\s*)("|<)(.*)("|>)', code):
            includes.append(directive[3])

    return includes


def find(name, path='/usr/include'):
    for root, dirs, files in os.walk(path):
        if name in files:
            return os.path.join(root, name)


def get_common_directive(file_name):
    path = find(file_name)
    
    if path is None:
        return set(), set()

    with open(path, 'r') as f:
        try:
            code = f.read()
        except UnicodeDecodeError:
            return set(), set()

        return get_directives(code)


def create_common_fake(includes):

    if not os.path.exists(FAKE_DIR):
        os.makedirs(FAKE_DIR)

    with open(os.path.join(FAKE_DIR, COMMON_FAKE_DEFINES), 'w+') as define_file, open(os.path.join(FAKE_DIR, COMMON_FAKE_TYPEDEFS), 'w+') as typedef_file:
        for include in includes:

            defines, typedefs = get_common_directive(include)

            for define in defines:
                define_file.write(f'#define {define}\n')

            for typedef in typedefs:
                typedef_file.write(f'typedef {typedef};\n')
