import re
import tree_sitter
from tree_sitter import Language, Parser


redundant_line_comments = re.compile("\/\/.*")
redundant_multiline_comments = re.compile("\/\*.*?\*\/", re.MULTILINE|re.DOTALL)

redundant_includes = re.compile("^\W*#\W*include\W* <\.\..*|^\W*#\W*include\W* \"\.\..*", re.MULTILINE)


def remove_comments(code, c_lang=True):
    code = redundant_line_comments.sub("\n", code)
    code = redundant_multiline_comments.sub("\n", code)

    return code

def add_func_decl(code, c_lang=True):
    headers = """
                    void for_loop_talkad7420();
                    void omp_pragma_talkad7420(char *pragma);\n
            """
    return headers + code
    
def wrap_for_loop(code, c_lang=True):
    updated_code = []
    loop_func = 'for_loop_talkad7420();'

    for line in code.split('\n'):
        l = line.lstrip().lower()

        if l.startswith('for ') or l.startswith('for('):
            updated_code.append(loop_func)
        updated_code.append(line)

    return '\n'.join(updated_code)

def wrap_for_pragma(code, c_lang=True):
    updated_code = []

    for line in code.split('\n'):
        l = line.lstrip().lower()

        if l.lstrip().startswith('#') and 'pragma' in l and 'omp' in l and 'for' in l:
            updated_code.append(f'omp_pragma_talkad7420("{line}");')
        else:
            updated_code.append(line)

    return '\n'.join(updated_code)

def wrap_for_pragma(code, c_lang=True):
    return redundant_includes.sub("", code)

def func_injection(func, c_lang=True):
    code = add_func_decl(func)
    code = wrap_for_pragma(wrap_for_loop(code))

    return code


def node2code(code, node):
    start_byte = node.start_byte
    end_byte = node.end_byte
    func = code[start_byte:end_byte]

    func_name = func[:func.find('(')]
    func_name = func_name.rstrip()
    func_name = func_name[func_name.rfind(' ')+1:]

    return func_name, func

def traverse(code, node):
    if node.type == 'function_definition':
        func_name, code = node2code(code, node)
        return [(func_name, code)]

    nodes = []
    for child in node.children:
        nodes += traverse(code, child)
    return nodes

def extract_funcs(code, c_lang=True):
    parser = Language('/home/talkad/Downloads/thesis/data_gathering_script/database_creator/parsers/HPCorpus_parser/my-languages.so', 'c')

    LANGUAGE = Language('./my-languages.so', 'c')
    parser = Parser()
    parser.set_language(LANGUAGE)

    tree = parser.parse(bytes(code, 'utf8'))

    try:
        result = traverse(code, tree.root_node)
    except:
        result = []

    return result

