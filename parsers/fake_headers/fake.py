import os
import re
from functools import reduce


REPOS_DIR = '/home/talkad/Downloads/thesis/data_gathering_script/git_repos'
# REPOS_DIR = '/home/talkad/Downloads/thesis/data_gathering_script/asd'
REPOS_OMP_DIR = '/home/talkad/Downloads/thesis/data_gathering_script/repositories_openMP'
FAKE_DIR = '/home/talkad/Downloads/thesis/data_gathering_script/parsers/fake_headers/utils'
FAKE_DEFINES  = '_fake_defines.h'
FAKE_TYPEDEFS = '_fake_typedefs.h'
FAKE_INCLUDE  = f'#include \"{FAKE_DEFINES}\"\n#include \"{FAKE_TYPEDEFS}\"'



def get_headers(repo_dir, repo_name):
    '''
    For a given repo, return all headers file relative path 
    '''
    headers, file_names = [], []
    root_dirs = set()
    path_length = len(repo_dir) + len (repo_name) + 2              # remove two '/'
    omp_repo = os.path.join(repo_dir, repo_name)

    for idx, (root, dirs, files) in enumerate(os.walk(omp_repo)):
        for file_name in files:
            ext = os.path.splitext(file_name)[1].lower()
            
            if ext == '.h':
                file_names. append(file_name)
                root_dirs.add(root)
                headers.append(os.path.join(root, file_name)[path_length: ])

    return headers, root_dirs, file_names


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
    func_defines =  set()
    typedefs = set()
    func_names = set()

    # remove comments
    LINE_COMMENT_RE = re.compile("//.*?\n" )
    MULTILINE_COMMENT_RE = re.compile("/\*.*?\*/", re.DOTALL)
    code = LINE_COMMENT_RE.sub("", code)
    code  = MULTILINE_COMMENT_RE.sub("", code)

    # join splitted lines in code
    code = join_splited_lines(code)

    for directive in re.findall(r'#(\s*)define(\s*)(\w+)(\s)', code):
        defines.add(directive[2])
        # defines.add(f' {directive[2]} 1')

    for directive in re.findall(r'#(\s*)define(\s*)(\w+)\((.*?)\)(.*)\n', code):
        if directive[2] not in func_names:
            func_defines.add(f'{directive[2]}({directive[3]})')
            func_names.add(directive[2])
        # defines.add(f' {directive[2]}({directive[3]}) 1')

    for directive in re.findall(r'(\s*)typedef(\s*)(\w+)(\s*)(\w+)(\s*);', code):
        typedefs.add(directive[4])
        # typedefs.add(f' int {directive[4]}')

    for directive in re.findall(r'(\s*)typedef(\s*)struct(\s*){(.+?)}(.+?);', code):
        typedefs.add(directive[4])
        # typedefs.add(f' int {directive[4]}')

    return defines, func_defines, func_names, typedefs


def get_repo_directives(repo_dir, repo_name):
    '''
    Extract all 'define' and 'typedef' directives from a given repo

    Paramenters:
        repo_name   - the name of the repo all the directives extracted from
    '''

    headers, _, _ = get_headers(repo_dir, repo_name)
    define_set =  set()
    func_defines_set = set()
    func_names_set = set()
    typedef_set = set()

    for header in headers:
        # open file and extract #define and typedef
        with open(os.path.join(repo_dir, repo_name, header), 'r')  as f:
            try:
                code = f.read()
            except UnicodeDecodeError:
                continue

            defines, func_defines, func_names, typedefs = get_directives(code)
            define_set |= defines
            func_defines_set |= func_defines
            func_names_set |= func_names
            typedef_set |= typedefs
            
    return define_set, func_defines_set, func_names_set, typedef_set


def get_all_directives():
    '''
    iterate over all repos and /usr/include and extract all directives
    '''
    print('extract common headers directives')
    define_set, func_defines_set, func_names_set, typedef_set = get_repo_directives('/usr', 'include')
    print('finished to extract common headers directives')

    for idx, repo in enumerate(os.listdir(REPOS_OMP_DIR)):
        defines, func_defines, func_names, typdefs = get_repo_directives(REPOS_DIR, repo)

        define_set |= defines
        func_defines_set |= func_defines
        func_names_set |= func_names
        typedef_set |= typdefs

        if idx > 0 and idx % 100 == 0:
            print(f'repo: {idx}')

    return define_set, func_defines_set, func_names_set, typedef_set


def extract_all_directives():
    '''
    Create fake headers that contain all defines and typedefs
    '''
    if not os.path.exists(FAKE_DIR):
        os.makedirs(FAKE_DIR)

    with open(os.path.join(FAKE_DIR, FAKE_DEFINES), 'w+') as define_file, open(os.path.join(FAKE_DIR, FAKE_TYPEDEFS), 'w+') as typedef_file:
        defines, func_defines, func_names, typedefs = get_all_directives()

        for define in defines:
            define_file.write(f'#define {define} 1\n')

        for name, func_def in zip(func_names, func_defines):
            if name not in defines:
                define_file.write(f'#define {func_def} 1\n')

        for typedef in typedefs:
            typedef_file.write(f'typedef int {typedef};\n')


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


def create_fake_header(header):
    '''
    Creates a header that contain only the fake includes
    '''
    path = os.path.join(FAKE_DIR, header)
    dir_path = path[:path.rfind('/')]

    if not os.path.exists(dir_path):
        os.makedirs(dir_path)

    with open(path, 'w') as f:
        f.write(FAKE_INCLUDE)


