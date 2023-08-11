import re
import json
import os
import logging
from tqdm import tqdm
from .asts import parse_tools, convert_representation as cr
import enums
from .asts.tokenizer import Tokompiler
from data.asts.lexicalization import lexicalize

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


def remove_comments(code, lang):
    if lang == enums.LANG_FORTRAN:
        code = redundant_line_comments_fortran.sub("\n", code)
        code = redundant_multiline_comments_c.sub("\n", code)
    elif lang in [enums.LANG_C, enums.LANG_CPP]:
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
        (list[str]):
            - List of source codes
            - List of replaced codes

    """
    # tokenizer = Tokompiler()
    sources = []
    replaced = []

    with open(file, encoding='utf-8') as f:
        for line in f.readlines():
            data = json.loads(line.strip())
            source = data['code'].strip()
            source = remove_comments(source, lang)

            ast = parse_tools.parse(source, lang)
            replaced_code = cr.generate_replaced(ast)

            if not replaced_code:   # parsing failed
                continue
            
            source = replace_string_literal(source)

            sources.append(source)
            replaced.append(replaced_code)

    return sources, replaced


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


def parse_for_pragma_gen(source_path, source_lang, target_path, target_lang):
    """
    Load and parse for code translation.

    Args:
        source_path (str): Path of source dataset
        source_lang (str): Source language
        target_path (str): Path of target dataset
        target_lang (str): Target language

    Returns:
        (list[str], list[str], list[str], list[str]):
            - List of tokenized code strings
            - List of linearized AST strings
            - List of tokenized target code strings

    """
    tokenizer = Tokompiler()

    logger.info(f'    Source file: {source_path}')
    sources = load_lines(source_path)
    logger.info(f'    Target file: {target_path}')
    targets = load_lines(target_path)   # xxxxxxxxxxxxxxxxxxxxx update

    new_sources = []
    new_targets = []
    asts = []
    for source, target in tqdm(zip(sources, targets), desc='Parsing', leave=False, total=len(sources)):
        try:
            source = remove_comments(source, lang=source_lang)

            # target = remove_comments_and_docstrings(target, lang=target_lang)
            # target = replace_string_literal(target) # xxxxxxxxxxxxxxxxxxx update

            ast = parse_tools.parse(source, source_lang)
            source = cr.generate_replaced(ast)

            ast = cr.code2xsbt(source, lang=source_lang)
            code = tokenizer.tokenize(trim_spaces(source), lang=source_lang)

            tokenized_target = []
            # tokenized_target = tokenize_source(source=target, lang=target_lang, use_regular=True) # xxxxxxxxxxxxxxxxxxxxxxxx update

            new_sources.append(code)
            asts.append(ast)
            new_targets.append(tokenized_target)
        except Exception:
            continue

    return new_sources, asts, new_targets


def load_clone_mapping(dataset_root):
    """
    Load json file and transfer to a mapping from code id to source code.

    Args:
        dataset_root (str): Root of the dataset

    Returns:
        dict: Mapping from code id to source code

    """
    path = os.path.join(dataset_root, 'fine_tune', enums.TASK_CLONE_DETECTION, 'data.jsonl')
    if not os.path.exists(path):
        return None
    mapping = dict()
    with open(path, encoding='utf-8') as f:
        for line in f.readlines():
            data = json.loads(line.strip())
            code_id = data['idx']
            source = data['func'].strip()
            mapping[code_id] = source
    return mapping

