import os
import re
import json
import shutil
from functools import reduce


with open('ENV.json', 'r') as f:
    vars = json.loads(f.read())

REPOS_DIR = vars['REPOS_DIR']
REPOS_OMP_DIR = vars['REPOS_OMP_DIR']
FAKE_DIR = vars['FAKE_DIR']


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
