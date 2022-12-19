import os
import re
import json

redundant_line_comments = re.compile("\/\/.*")
redundant_multiline_comments = re.compile("\/\*.*?\*\/", re.MULTILINE|re.DOTALL)


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


def count_pragmas(code):
    counter = 0

    for line in code.split('\n'):
        l = line.lstrip().lower()

        if l.startswith('#pragma') and 'omp' in l:
            counter += 1

    return counter


def is_next_char_paren(code):
    return code.lstrip().startswith('{')


def is_single_line_for(code):
    idx = paren_matcher(0, code, open_paren='(', closing_paren=')')
    return not is_next_char_paren(code[idx + 1:])


def inspect_file_LOC(file_path):
    '''
    Return for a fiven file all openMP verions used
    '''    
   
    with open(file_path, 'r') as f:
        code = f.read()
        loc = count_pragmas(code)

        code = redundant_line_comments.sub("\n", code)
        code = redundant_multiline_comments.sub("\n", code)
        code = '\n'.join(filter(lambda line: len(line.lstrip()) > 0, code.split('\n')))
        code = code.lower()
        
        total_lines = code.count('\n')+1
        idx = code.find('#pragma')

        while idx != -1:
            code = code[idx:]
            pragma = code[: code.find('\n')]
            pragma = pragma[:pragma.find('{')] if '{' in pragma else pragma

            if 'omp' in pragma and 'parallel' in pragma:
                if is_next_char_paren(code[len(pragma):]):
                    paren_idx = paren_matcher(0, code)
                    if paren_idx != -1:
                        loc += code[:paren_idx].count('\n')
                        code = code[paren_idx + 1:]
                    else:
                        return (total_lines, loc)
                elif 'for' in pragma and is_single_line_for(code[len(pragma): ]):
                    loc += 2
                    code = code[len(pragma):]
                elif 'for' in pragma:
                    paren_idx = paren_matcher(0, code)
                    if paren_idx != -1:
                        loc += code[:paren_idx].count('\n')
                        code = code[paren_idx + 1:]
                    else:
                        return (total_lines, loc)
                else:
                    code = code[len(pragma):]
            else:
                code = code[len(pragma):]

            idx = code.find('#pragma')

    return (total_lines, loc)


def scan_repos(omp_repo):
    repo2lines = {}

    for user_name in os.listdir(omp_repo):
        for repo_name in os.listdir(os.path.join(omp_repo, user_name)):
            repo2lines[f'{user_name}/{repo_name}'] = {}

            for idx, (root, dirs, files) in enumerate(os.walk(os.path.join(omp_repo, user_name, repo_name))):
                for file in files:
                    ext = file[file.rfind('.')+1:]

                    if ext in ['c', 'cpp']:
                        repo2lines[f'{user_name}/{repo_name}'][file] = inspect_file_LOC(os.path.join(root, file))
    
    with open("repo2lines.json", "w") as f:
        json.dump(repo2lines, f, sort_keys=True, indent=4, separators=(',', ': '))


def loc_stats():
    total_lines = 0
    prl_lines = 0

    with open("repo2lines.json", "r") as f:
        repo2lines = json.load(f)

    for repo in repo2lines:
        for file in repo2lines[repo]:

            total_lines += repo2lines[repo][file][0]
            prl_lines += repo2lines[repo][file][1]

    return total_lines, prl_lines

# scan_repos('/home/talkad/Downloads/thesis/data_gathering_script/repositories_openMP')
# print(inspect_file_LOC('/home/talkad/Downloads/thesis/data_gathering_script/a.c'))
print(loc_stats()) # (4335645, 1150409) 37.6%