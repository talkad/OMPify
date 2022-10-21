import os
import re
import json
import shutil
from functools import reduce

# remove these ..
with open('../ENV.json', 'r') as f:
    vars = json.loads(f.read())

REPOS_DIR = vars['REPOS_DIR']
REPOS_OMP_DIR = vars['REPOS_OMP_DIR']
FAKE_DIR = vars['FAKE_DIR']
FAKE_DEFINES  = '_fake_defines.h'
FAKE_TYPEDEFS = '_fake_typedefs.h'
FAKE_INCLUDE  = f'#include \"{FAKE_DEFINES}\"\n#include \"{FAKE_TYPEDEFS}\"\n'



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
    func_defines =  []
    typedefs = set()
    func_names = []

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
        func_defines.append(f'{directive[2]}({directive[3]})')
        func_names.append(directive[2])
        # defines.add(f' {directive[2]}({directive[3]}) 1')

    for directive in re.findall(r'(\s*)typedef(\s*)(.*?);', code):
        typedefs.add(directive[2].split(" ")[-1])
        # typedefs.add(f' int {directive[4]}')

    for directive in re.findall(r'(\s*)typedef(\s*)struct(\s*){(.|\n)+?}(.+?);', code):
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
    func_defines_set = []
    func_names_set = []
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
            func_defines_set += func_defines
            func_names_set += func_names
            typedef_set |= typedefs
            
    return define_set, func_defines_set, func_names_set, typedef_set


def extract_typedef_directives():
    '''
    Create fake headers contains typedefs
    '''
    typedefs_set = set()
    relevant_repos = os.listdir(REPOS_OMP_DIR)

    if not os.path.exists(FAKE_DIR):
        os.makedirs(FAKE_DIR)

    # iterate over all the relevant repositories
    for idx, repo_name in enumerate(os.listdir(REPOS_DIR)):

        if repo_name not in relevant_repos:
            continue

        _, _, _, typedefs = get_repo_directives(REPOS_DIR, repo_name)

        for typedef in typedefs:
            typedefs_set.add(typedef)

        if idx > 0 and idx % 10**2 == 0:
            print(f'repos passed: {idx}')

    with open(os.path.join(FAKE_DIR, '_fake_types.h'), 'w+') as typedef_file:
        for typedef in typedefs_set:
            if ')' not in typedef and '}' not in typedef:
                typedef_file.write(f'typedef int {typedef};\n')


def create_fake_headers(repo_name):
    '''
    Create fake headers that contain all defines and typedefs
    '''
    if not os.path.exists(FAKE_DIR):
        os.makedirs(FAKE_DIR)

    with open(os.path.join(FAKE_DIR, FAKE_DEFINES), 'w+') as define_file, open(os.path.join(FAKE_DIR, FAKE_TYPEDEFS), 'w+') as typedef_file:
        defines, func_defines, func_names, typedefs = get_repo_directives(REPOS_DIR, repo_name)
        def_func_dict = {k:v for k,v in list(zip(func_names, func_defines))}

        for define in defines:
            define_file.write(f'#define {define} 1\n')

        for def_func in def_func_dict:
            if def_func not in defines:
                define_file.write(f'#define {def_func_dict[def_func]} 1\n')

        for typedef in typedefs:
            if ')' not in typedef and '}' not in typedef:
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
    path = os.path.join(FAKE_DIR, header) # check that
    dir_path = path[:path.rfind('/')]
    try:

        if not os.path.exists(dir_path):
            os.makedirs(dir_path)

        with open(path, 'w') as f:
            f.write(FAKE_INCLUDE)
    except:
        return


def create_not_exists_headers(repo_dir, repo_name):
    '''
    Create fake headers for headers that don't exist in the given repo
    '''
    paths, _, _ = get_headers(REPOS_DIR, repo_name)

    for root, dirs, files in os.walk(os.path.join(repo_dir, repo_name)):
        for file_name in files:
            ext = os.path.splitext(file_name)[1].lower()
            
            if ext in ['.h', '.c']:
                includes = extract_includes(os.path.join(root, file_name))
                for include in includes:
                    # if all([False for path in paths if path.endswith('/' + include)]):
                    create_fake_header(include)


