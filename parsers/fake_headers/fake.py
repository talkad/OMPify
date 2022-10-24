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


def get_directives(file_path):
    '''
    Extract all 'typedef' from given file
    '''
    typedefs = set()

    with open(file_path, 'r') as f:
        try:
            code = f.read()
        except UnicodeDecodeError as e:
            return typedefs

    # remove comments
    LINE_COMMENT_RE = re.compile("//.*?\n" )
    MULTILINE_COMMENT_RE = re.compile("/\*.*?\*/", re.DOTALL)
    code = LINE_COMMENT_RE.sub("", code)
    code  = MULTILINE_COMMENT_RE.sub("", code)

    # string matching for 'typedef'
    for directive in re.findall(r'(\s*)typedef(\s*)(.*?);', code):
        typedefs.add(directive[2].split(" ")[-1])

    for directive in re.findall(r'(\s*)typedef(\s*)struct(\s*){(.|\n)+?}(.+?);', code):
        typedefs.add(directive[4])

    return typedefs


def get_repo_directives(repo_name):
    '''
    Extract all 'typedef' directives from a given repo

    Paramenters:
        repo_name - the name of the repo all the directives extracted from
    '''
    typedef_set = set()

    for root, dirs, files in os.walk(os.path.join(omp_repo, repo_name)):
        for file_name in files:
            file_path = os.path.join(root, file_name)
            typedefs = get_directives(file_path)
            typedef_set |= typedefs
            
    return  typedef_set


def extract_typedef_directives(repo_name, fake_header_path):
    '''
    Create fake header that contains typedefs (without duplicates - with respect to the fake_header.h that can be found in utils)

    Parameters:
        repo_name        - the repo the typedefs will be extracted from
        fake_header_path - the path to the generated fake header
    '''

    general_typedefs = get_directives('path/to/file/utils')
    current_typedefs = get_directives(repo_name)

    with open(fake_header_path, 'w+') as typedef_file:
        for typedef in current_typedefs:
            if ')' not in typedef and '}' not in typedef and \
                not typedef in general_typedefs:
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


# def create_fake_header(header):
#     '''
#     Creates a header that contains only the fake includes
#     '''
#     path = os.path.join(FAKE_DIR, header) # check that
#     dir_path = path[:path.rfind('/')]
#     try:

#         if not os.path.exists(dir_path):
#             os.makedirs(dir_path)

#         with open(path, 'w') as f:
#             f.write(FAKE_INCLUDE)
#     except:
#         return


# def create_not_exists_headers(repo_dir, repo_name):
#     '''
#     Create fake headers for headers that don't exist in the given repo
#     '''
#     paths, _, _ = get_headers(REPOS_DIR, repo_name)

#     for root, dirs, files in os.walk(os.path.join(repo_dir, repo_name)):
#         for file_name in files:
#             ext = os.path.splitext(file_name)[1].lower()
            
#             if ext in ['.h', '.c']:
#                 includes = extract_includes(os.path.join(root, file_name))
#                 for include in includes:
#                     # if all([False for path in paths if path.endswith('/' + include)]):
#                     create_fake_header(include)


def create_empty_header(header, dest_folder):
    '''
    Create an empty header file inside a folder
    '''
    path = os.path.join(dest_folder, header)
    dir_path = path[:path.rfind('/')]

    try:
        if not os.path.exists(dir_path):
            os.makedirs(dir_path)

        with open(path, 'w') as f:
            f.write('')

    except:
        return


def create_empty_headers(file_path, dest_folder):
    '''
    Create all the header files mentioned in @file_path src code
    '''
    headers = extract_includes(file_path)

    for header in headers:
        if '..' not in header:
            create_empty_header(header, dest_folder)


