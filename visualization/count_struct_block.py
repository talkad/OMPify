'''
Purpose of this class
1. count the number of total code lines
2. count the number of lines in parallel struct block
3. identify each parallel struct block 
'''

## remove empty lines

import os

def paren_matcher(fid, code, open_paren='{', closing_paren='}'):
    '''
    Parameters:
        fid - the index of opening paren for strcture block
        code - string of code

    Returns:
        the index of the closing paren
    '''
    in_struct = False
    paren_counter = 0
    
    for idx, ch in enumerate(code[fid:]):
        if ch == open_paren:
            paren_counter += 1
            in_struct = True
        elif in_struct and ch == closing_paren:
            paren_counter -= 1

        if paren_counter == 0 and in_struct:
            return idx

    return -1


def get_omp_version(line):
    '''
        For a given pragma, determine the version of OpenMP it is related
    '''
    if 'simd' in line:
        return 3
    elif 'offload' in line:
        return 5
    elif 'task' in line:
        return 3

    return -1


def is_next_char_paren(code):
    return code.lstrip().startswith('{')


def is_single_line_for(code):
    idx = paren_matcher(0, code, open_paren='(', closing_paren=')')
    return is_next_char_paren(code[idx:])


def inspect_file_versions(file_path):
    '''
    Return for a fiven file all openMP verions used
    '''    
    versions = []
   
    with open(file_path, 'r') as f:
        code = f.read()

        code = redundant_line_comments.sub("\n", code)
        code = redundant_multiline_comments.sub("\n", code)
        code = code.lower()
        idx = code.find('#pragma')

        while idx != -1:
            code = code[idx:]
            version = get_omp_version(code[:code.find('\n')])
            
            if version != -1:
                versions.append(version)

            idx = code.find('#pragma')
            
    return versions


def inspect_file_LOC(file_path):
    '''
    Return for a fiven file all openMP verions used
    '''    
    loc = 0
   
    with open(file_path, 'r') as f:
        code = f.read()

        code = redundant_line_comments.sub("\n", code)
        code = redundant_multiline_comments.sub("\n", code)
        code = code.lower()
        idx = code.find('#pragma')

        while idx != -1:
            code = code[idx:]
            end_idx = paren_matcher(0, code)
            # need to include single line strctures

            loc += code[:end_idx].count('\n')
            
            if version != -1:
                versions.append(version)
                
            idx = code[end_idx:].find('#pragma')
            
    return versions



def scan_repos(omp_repo):

    for repo_name in os.listdir(omp_repo):
        for idx, (root, dirs, files) in enumerate(os.walk(os.path.join(omp_repo, repo_name))):
            pass
    #     for file_name in files:
    #         ext = os.path.splitext(file_name)[1].lower()


# scan_repos('/home/talkad/Downloads/thesis/data_gathering_script/repositories_openMP')


print(paren_matcher(0, '  {}   {          aaa{aa}a    }'))