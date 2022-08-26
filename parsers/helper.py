import os
import re
from functools import reduce


REPOS_DIR = '/home/talkad/Downloads/thesis/data_gathering_script/git_repos'
# REPOS_DIR = '/home/talkad/Downloads/thesis/data_gathering_script/asd'
FAKE_DEFINES  = 'fake_headers/_fake_defines.h'
FAKE_TYPEDEFS = 'fake_headers/_fake_typedefs.h'

def get_headers(repo_name):
    '''
    For a given repo, return all headers file relative path 
    '''
    headers = []
    path_length = len(REPOS_DIR) + len (repo_name) + 2              # remove two '/'
    omp_repo = os.path.join(REPOS_DIR, repo_name)

    for idx, (root, dirs, files) in enumerate(os.walk(omp_repo)):
        for file_name in files:
            ext = os.path.splitext(file_name)[1].lower()
            
            if ext in '.h':
                headers.append(os.path.join(root, file_name)[path_length: ])

    return headers


def join_splited_lines(code_buf, delimiter='\\'):
    '''
    Several c files are splitting lines of code using \ token. For instance:
        #pragma omp parallel for private(i, test, x, y) \
            default(shared) schedule(dynamic)
    pycparser fail to process this files. So we update this lines to be a single line.

    OR

    Several Fortran files are splitting lines of code using & token. 
    fparser fails to process this files. So we update this lines to be a single line.
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


def get_directives(repo_name):
    '''
    Extract all 'define' and 'typedef' directives from a given repo
    '''

    headers = get_headers(repo_name)
    defines =  []
    typedefs = []

    for header in headers:
        # open file and extract #define and typedef
        with open(os.path.join(REPOS_DIR, repo_name, header), 'r')  as f:
            try:
                code = f.read()
            except UnicodeDecodeError:
                continue
            
            # remove comments
            LINE_COMMENT_RE = re.compile("//.*?\n" )
            MULTILINE_COMMENT_RE = re.compile("/\*.*?\*/", re.DOTALL)
            code = LINE_COMMENT_RE.sub("", code)
            code  = MULTILINE_COMMENT_RE.sub("", code)

            # join splitted lines in code
            code = join_splited_lines(code)

            for directive in re.findall(r'#(\s*)define (.+?)\n', code):
                defines.append(directive[1])

            for directive in re.findall(r'(\s*)typedef(\s*)(\w+)(\s*)(\w+)(\s*);', code):
                typedefs.append(f' {directive[2]} {directive[4]}')

            for directive in re.findall(r'(\s*)typedef(\s*)struct(\s*){(.+?)}(.+?);', code):
                typedefs.append(f' struct {{ {directive[3]} }} {directive[4]}')

    return defines, typedefs


def create_fake_headers():
    '''
    Create fake headers which contain all defines and typedefs
    '''

    print('========== create fake headers ==========')
    with open(FAKE_DEFINES, 'w') as define_file, open(FAKE_TYPEDEFS, 'w+') as typedef_file:
        # iterate over all the repositories
        for idx, repo_name in enumerate(os.listdir(REPOS_DIR)):
            defines, typedefs = get_directives(repo_name)

            for define in defines:
                define_file.write(f'#define {define}\n')

            for typedef in typedefs:
                typedef_file.write(f'typedef {typedef};\n')

            if idx > 0 and idx % 100 == 0:
                print(f'repos passed: {idx}')

# create_fake_headers()