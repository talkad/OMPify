import re
import random
import tree_sitter
import textwrap
from tree_sitter import Language, Parser

RE_NUMBERS = re.compile(r"(?<![_a-zA-Z])\b[0-9]+(?:\.[0-9]+)?(?:f)?\b(?![0-9.-])")
RE_HEXA = re.compile(r"0x[0-9a-fA-F]+")
RE_CHARS = re.compile(r"\'.\'")
RE_STR = re.compile(r"\"(?:\\.|[^\\\"])*\"")
RE_STR_MULTI_LINE = re.compile(r"\"(?:\\.|[^\"\\])*?\"")


VAR, ARR, FUNC, NUM, STRUCT, FIELD = 1, 2, 3, 4, 5, 6
replaced_prefixes = { VAR: 'var_',
                      ARR: 'arr_',
                      FUNC: 'func_',
                      NUM: 'num_',
                      STRUCT: 'struct_',
                      FIELD: 'field_'                    
                    }


def get_parser(lang):
    LANGUAGE = Language('./my-languages.so', lang.lower())
    # LANGUAGE = Language('/home/talkad/OpenMPdb/database_creator/parsers/HPCorpus_parser/my-languages.so', lang.lower())
    parser = Parser()
    parser.set_language(LANGUAGE)

    return parser


def parse(code, lang):
    '''
    Convert @code into an AST according to its programming @lang
    '''
    parser = get_parser(lang)

    tree = parser.parse(bytes(code, 'utf8'))
    return tree


def create_dfg(ast):
    pass


def count_newlines(code):
    counter = 0

    for letter in code:
        if letter == '\n':
            counter += 1
            continue

        return counter
    
    return counter


def replace_vars(code, var_mapping):
    '''
        Create replaced representation 
    '''
    updated_code = ''
    prev_idx = 0
    offset = count_newlines(code)
    updated_mappings = []
    var_offset = 0

    for old_var, new_var, start, end in var_mapping:
        updated_mappings.append((new_var, old_var, start-offset+var_offset, start-offset+var_offset+len(new_var)))
        var_offset += len(new_var)-len(old_var)
        updated_code += code[prev_idx:start-offset] + new_var
        prev_idx = end - offset

    updated_code += code[prev_idx:]

    return updated_code, updated_mappings


def get_identifiers(node, kind=''):
    '''
        Find identifiers names in code

        Parameters:
            node - declaration node in the AST
            kind - the type of  the sub node
        Return:
            list for each replaced variable kind (variable, array, function)
    '''
    if node.type == 'identifier':
        return ([],[],[(node.text, node.start_byte, node.end_byte)],[]) if kind=='func' else ([],[(node.text, node.start_byte, node.end_byte)],[],[]) if kind=='arr' else ([(node.text, node.start_byte, node.end_byte)],[],[],[])
    elif node.type == 'field_identifier':
        return ([],[],[],[(node.text, node.start_byte, node.end_byte)])

    vars, arrays, funcs, fields = [], [], [], []
    for child in node.children:
        va, ar, fu, fi = get_identifiers(child, kind=('arr' if child.type == 'array_declarator' else
                                                  'func' if child.type in ['call_expression', 'function_declarator'] else
                                                  '' if child.type in ['parameter_declaration', 'argument_list', 'field_expression', 'parameter_list', 'compound_statement'] else
                                                  'field' if child.type == 'field_identifier' else
                                                   kind if len(kind)>0 else  ''))
        vars, arrays, funcs, fields = vars+va, arrays+ar, funcs+fu, fields+fi

    return vars, arrays, funcs, fields


def generate_serial_numbers(N):
    numbers = list(range(N))
    random.shuffle(numbers)

    return numbers


def replace_constants(code, replace_token, regex):
    '''
        Replace constatns in code with a given token

        Parameters:
            code - the original code to be updated
            replace_token - the token that will replace the constants
            regex - the regular expression that captures the constants
    '''
    matches = regex.finditer(code)

    offset = 0
    for match in matches:
        start = match.start() + offset
        end = match.end() + offset
        code = code[:start] + replace_token + code[end:]
        offset += len(replace_token) - len(match.group())

    return code


def update_var_names(ast, num_generator):
    name_map = {}
    vars, arrays, functions, fields = get_identifiers(ast)

    for type, identifiers in zip([VAR, ARR, FUNC, FIELD], [vars, arrays, functions, fields]):
        unique_vars= list(set([var[0] for var in identifiers]))
        random_numbers_vars = num_generator(len(unique_vars))

        for var, num in zip(unique_vars, random_numbers_vars):
            name_map[var] = f'{replaced_prefixes[type]}{num}'

    # replace and sort the vars according to their location
    vs = fields+vars+arrays+functions
    vs.sort(key=lambda tup: tup[1])
    var_mapping = [(var.decode(), name_map[var], start, end) for var, start, end in vs]
    
    updated_code, updated_mappings = replace_vars(ast.text.decode(), var_mapping)

    # replace constants
    for r_token, regex in zip(['STR', 'STR', 'CHAR', 'NUM', 'NUM'], [RE_STR, RE_STR_MULTI_LINE, RE_CHARS, RE_NUMBERS, RE_HEXA]):
        updated_code = replace_constants(updated_code, r_token, regex)

    return updated_code


def generate_replaced(code, num_generator=generate_serial_numbers):
    '''
        Main funtion to create the replaced represrntation
    '''
    tree = parse(code.lstrip(), 'c')
    updated_code = update_var_names(tree.root_node, num_generator)

    return updated_code




# code = '''

# int a = 5;
# printf("%d", a);
# String str = "hello world";
# '''


# with open('example.c', 'w') as f:
#     f.write(generate_replaced(code))


