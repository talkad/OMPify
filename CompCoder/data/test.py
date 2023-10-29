import re
import json
import os
import logging
from tqdm import tqdm
from asts import parse_tools, convert_representation as cr
from asts.lexicalization import lexicalize


logger = logging.getLogger(__name__)

STRING_MATCHING_PATTERN = re.compile(r'([bruf]*)(\"\"\"|\'\'\'|\"|\')(?:(?!\2)(?:\\.|[^\\]))*\2')
NON_SPACE_MATCHING_PATTERN = re.compile(r'\S')

redundant_line_comments_c = re.compile("\/\/.*")
redundant_line_comments_fortran = re.compile("!.*$|^c.*$|^C.*$|^d.*$|^D.*$|^\*.*$", re.MULTILINE)
redundant_multiline_comments_c = re.compile("\/\*.*?\*\/", re.MULTILINE|re.DOTALL)

def load_eval_lines(path):
    """
    Load and eval lines from given path.

    Args:
        path (str): Dataset file path

    Returns:
        list: List of lines

    """
    with open(path, encoding='utf-8') as f:
        lines = [eval(line.strip()) for line in f]
    return lines


def load_eval_list_lines(path):
    """
    Load and eval lines from given path, each line is a list that will be convert into a string.

    Args:
        path (str): Dataset file path

    Returns:
        list: List of lines
    """
    lines = []
    with open(path, encoding='utf-8') as f:
        for line in f.readlines():
            tokens = eval(line.strip())
            string = ' '.join(tokens)
            string = re.sub(r'\s+', ' ', string)
            lines.append(string)
    return lines


def load_lines(path):
    """
    Load lines from given path.

    Args:
        path (str): Dataset file path

    Returns:
        list: List of lines

    """
    with open(path, encoding='utf-8') as f:
        lines = [line.strip() for line in f]
    return lines


# def remove_comments(code, lang):
#     if lang == enums.LANG_FORTRAN:
#         code = redundant_line_comments_fortran.sub("\n", code)
#         code = redundant_multiline_comments_c.sub("\n", code)
#     elif lang in [enums.LANG_C, enums.LANG_CPP]:
#         code = redundant_line_comments_c.sub("\n", code)
#         code = redundant_multiline_comments_c.sub("\n", code)

#     return code


def remove_comments(code, lang):
    if lang == 'fortran':
        code = redundant_line_comments_fortran.sub("\n", code)
        code = redundant_multiline_comments_c.sub("\n", code)
    elif lang in ['c', 'cpp']:
        code = redundant_line_comments_c.sub("\n", code)
        code = redundant_multiline_comments_c.sub("\n", code)

    return code

def replace_string_literal(source):
    """
    Replace the string literal in source code with ``<STR>``.

    Args:
        source (str): Source code in string

    Returns:
        str: Code after replaced

    """
    return re.sub(pattern=STRING_MATCHING_PATTERN, repl='___STR', string=source)


def parse_json_file(file, lang):
    """
    Parse a dataset file where each line is a json string representing a sample.

    Args:
        file (str): The file path
        lang (str): Source code language

    Returns:
        - List of source codes
        - List of replaced codes

    """

    sources = []
    replaced = []

    with open(file, encoding='utf-8') as f:
        for line in f.readlines():
            data = json.loads(line.strip())
            source = data['code'].strip()
            source = remove_comments(source, lang)

            ast = parse_tools.parse(source, lang)
            replaced_code, mapping = cr.generate_replaced(ast)
            
            if not replaced_code:   # parsing failed
                continue
            
            source = replace_string_literal(source)

            sources.append(source)
            replaced.append(replaced_code)

    return sources, replaced


def parse_openmp_pragma_replace(pragma, mapping):
    pragma = f'{pragma} '
    pattern = r'(\w+(\s*\(.*?\)|\s))'
    matches = re.findall(pattern, pragma)
    clauses = []
    
    for match in matches:
        clause = match[0].strip()

        if '(' in clause:
            clause = clause[:clause.find('(')].strip()
            args = match[1].strip()[1:-1]

            args = sorted([mapping[arg] for arg in args.split()])
    
            clauses.append((clause, ' '.join(args)))
        else:
            clauses.append((clause, ''))
    
    return clauses


def parse_json_file_pragma(file, lang):

    sources = []
    replaced = []
    pragmas = []
    replaced_pragmas = []

    with open(file, encoding='utf-8') as f:
        for line in f.readlines():
            data = json.loads(line.strip())
            source = data['code'].strip()
            source = remove_comments(source, lang)

            ast = parse_tools.parse(source, lang)
            replaced_code, mapping = cr.generate_replaced(ast)
            mapping = {k:v for k,v,_ in mapping}
            
            if not replaced_code:   # parsing failed
                continue
            
            source = replace_string_literal(source)

            sources.append(source)
            replaced.append(replaced_code)
            pragmas.append(data['pragma'])
            replaced_pragmas.append(parse_openmp_pragma(data['pragma'], mapping))

    return sources, replaced, pragmas, replaced_pragmas


def iter_all_files(base):
    """
    Iterator for all file paths in the given base path.

    Args:
        base (str): Path like string

    Returns:
        str: Path of each file
    """
    for root, ds, fs in os.walk(base):
        for f in fs:
            yield os.path.join(root, f)


def iter_pre_train_dataset_files(args, lang_dir, lang):
    """
    Get files for pre-training, all files with extension ``jsonl`` will be included.

    Args:
        lang_dir (str): Path of language dir
        lang (str): Source code language

    Returns:
        list[str]: List of paths of files

    """
    if lang in args.langs:
        return [file for file in iter_all_files(base=lang_dir) if file.endswith('.jsonl')]
    return []


def load_pre_train_dataset(args, file, lang):
    """
    Load json dataset from given file.

    Args:
        file (str): Path of dataset file
        lang (str): Source code language

    Returns:
        (list[str], list[str], list[str], list[str], list[str]):
            - List of source code strings
            - List of replaced code strings

    """
    if lang in args.langs:
        sources, replaced = parse_json_file(file, lang=lang)
        return sources, replaced


def load_dataset_from_dir(args, dataset_dir):
    """
    Load all files in the given dir, only for pre-training.

    Args:
        dataset_dir (str): Root directory

    Returns:
        (dict, list[str], list[str]):
            - Dict of paths: key is the dataset group, value is the path
            - List of str: languages for each line
            - List of str: source code
    """
    paths = {}
    languages = []
    all_sources = []
    all_source_tokens = []
    all_replaced = []
    all_replaced_tokens = []
    all_asts = []

    for file in os.listdir(dataset_dir):

        path = os.path.join(dataset_dir, file)
        if os.path.isfile(path):
            continue

        lang = file
        dataset_files = iter_pre_train_dataset_files(args, path, lang=lang)
        if len(dataset_files) > 0:
            logger.info(f'  Language: {lang}')
            paths[lang] = dataset_files
            n_sample = 0
            for dataset_file_path in tqdm(dataset_files):
                sources, replaced = load_pre_train_dataset(args, file=dataset_file_path, lang=lang)

                new_sources = []
                new_source_tokens = []
                new_replaced = []
                new_replaced_tokens = []
                asts = []
                for source, code in tqdm(zip(sources, replaced),desc=f'Parsing {os.path.basename(dataset_file_path)}',
                                                             leave=False,
                                                             total=len(sources)):
                    try:
                        if not source or not code: 
                            continue

                        ast = cr.code2xsbt(source, lang=lang)

                        if not ast:
                            continue

                        new_sources.append(source)
                        new_source_tokens.append(lexicalize(source, lang=lang))

                        new_replaced.append(code)
                        new_replaced_tokens.append(lexicalize(code, lang=lang, replaced=True))

                        asts.append(ast)
                    except Exception as e:
                        print(e)
                        continue

                all_sources += new_sources
                all_source_tokens += new_source_tokens

                all_replaced += new_replaced
                all_replaced_tokens += new_replaced_tokens

                all_asts += asts

                n_line = len(new_sources)
                languages += [lang for _ in range(n_line)]
                n_sample += n_line

                logger.info(f'    File: {dataset_file_path}, {n_line} samples')

            logger.info(f'  {lang} dataset size: {n_sample}')
            logger.info(f'  {lang} list size: {len(all_asts)}')

    assert len(languages) == len(all_sources) == len(new_replaced) == len(all_asts)
    return paths, languages, all_sources, all_source_tokens, all_replaced, all_replaced_tokens, all_asts


def trim_spaces(string):
    """
    Replace consecutive spaces with a single whitespace.

    Args:
        string (str): String

    Returns:
        str: Replaced string
    """
    return re.sub(r'\s+', ' ', string).strip()


def count_non_space_chars(s):
    """
    Count the non-space characters.

    Args:
        s (str): String to be counted

    Returns:
        int: Number of non-space characters

    """
    matches = re.findall(NON_SPACE_MATCHING_PATTERN, s)
    return len(matches)


def align_source_code(former_source, code):
    """
    Align former source to code token string and split code into former one and latter one.

    Args:
        former_source (str): Former part of the source
        code (str): Tokenized source code

    Returns:
        (str, str):
            - Former part of tokenized code
            - Latter part of tokenized code

    """
    former_count = count_non_space_chars(former_source)
    total = 0
    code_tokens = code.split(' ')
    token_index = 0
    while total < former_count:
        total += count_non_space_chars(code_tokens[token_index])
        token_index += 1
    former_code = ' '.join(code_tokens[:token_index])
    latter_code = ' '.join(code_tokens[token_index:])
    return former_code, latter_code


def parse_openmp_pragma(pragma):
    '''
    parse OpenMP pragma into meaningful representation

    Parameters:
        pragma: str - string indicating the pragma
    Result:
        A list containing tuples representing the clause and their arguments

    Example:
        input: '#pragma omp for private  (a,b,c) lastprivate(d) schedule(static:8)'
        output: [('pragma', ''), ('omp', ''), ('for', ''), ('private', 'a,b,c'), ('lastprivate', 'd'), ('schedule', 'static:8')]

    '''
    pragma = f'{pragma} '
    pattern = r'(\w+(\s*\(.*?\)|\s))'
    matches = re.findall(pattern, pragma)
    clauses = []
    
    for match in matches:
        clause = match[0].strip()

        if '(' in clause:
            clause = clause[:clause.find('(')].strip()
            args = match[1].strip()[1:-1]
            clauses.append((clause, args))
        else:
            clauses.append((clause, ''))
    
    return clauses


def replace_pragma(pragma, mappings):
    clauses = parse_openmp_pragma(pragma)
    replaced_pragma = []

    for clause,vars in clauses:
        if not vars:
            replaced_pragma.append((clause,vars))
        elif clause in ['reduction', 'map']:
            func = vars[:vars.find(':')]
            func_vars = vars[vars.find(':')+1:].split()

            func_vars = ' '.join(sorted([mappings[var] for var in func_vars]))
            replaced_pragma.append((clause,f'{func} : {func_vars}'))
        else:
            func_vars = vars.split()

            func_vars = ' '.join(sorted([mappings[var] for var in func_vars]))
            replaced_pragma.append((clause,func_vars))

    return ' '.join(clause[0] if not clause[1] else f'{clause[0]} ( {clause[1]} )' for clause in replaced_pragma)


def parse_for_pragma_gen(dataset_path, lang, split, is_replaced):
    """
    Load and parse for pragma generation.

    Args:
        dataset_path (str): Path of pramga gen dataset

    Returns:
    self.sources, self.source_tokens, self.replaced, self.replaced_tokens, self.pragma, self.pragma_tokens, \
                    self.replaced_pragma, self.replaced_pragma_tokens, self.asts

        (list[str], list[str], list[str], list[str], list[str]):
            - List of code tokens 
            - List of replaced code tokens
            - List of pragma tokens
            - List of replaced pragma tokens
            - List of asts

    """

    logger.info(f'    dataset: {dataset_path}')

    sources = []
    pragmas = []
    asts = []

    num_augment = 4 if is_replaced else 1

    # with open(os.path.join(dataset_path, lang, 'replaced' if is_replaced else 'source', split, 'total.jsonl'), 'r') as f:
    with open(os.path.join(dataset_path, lang, 'source', split, 'total.jsonl'), 'r') as f:

        for line in tqdm(f, desc='Parsing'):
            for _ in range(num_augment):
                try:
                    js = json.loads(line.strip())

                    code = js['code']
                    pragma = js['pragma']
                
                    if is_replaced:
                        ast = parse_tools.parse(code, lang)
                        code, mapping = cr.generate_replaced(ast)
                        mapping = {k:v for k,v in mapping.items()}
                        
                        if not code:   # parsing failed
                            continue
                        
                        pragma = replace_pragma(pragma, mapping)


                    source = lexicalize(code, lang=lang, replaced=is_replaced, partial=True)
                    pragma = pragma.replace('_', ' ')
                        
                    sources.append(source)
                    pragmas.append(pragma)                    

                except Exception as e:
                    continue

    return sources, pragmas

# import pdb; pdb.set_trace()
a = parse_for_pragma_gen('/home/talkad/LIGHTBITS_SHARE/CompCoder_datasets/OMP_Dataset', 'c', 'train', True)
print('aa')
print('bb')