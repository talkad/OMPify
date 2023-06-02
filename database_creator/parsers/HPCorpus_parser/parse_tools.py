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
    # LANGUAGE = Language('./my-languages.so', lang.lower())
    LANGUAGE = Language('/home/talkad/OpenMPdb/database_creator/parsers/HPCorpus_parser/my-languages.so', lang.lower())
    parser = Parser()
    parser.set_language(LANGUAGE)

    return parser


def parse(code, lang):
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


def replace_vars(code, vars, arrays, functions, fields, name_map):
    updated_code = ''
    prev_idx = 0
    offset = count_newlines(code)

    vars = fields+vars+arrays+functions
    vars.sort(key=lambda tup: tup[1])
    for var, start, end in vars:
        updated_code += code[prev_idx:start-offset].decode() + str(name_map[var])
        # print(updated_code)
        # print('=====')
        prev_idx = end - offset

    updated_code += code[prev_idx:].decode()

    return updated_code


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
        # print('-----', node.text, f'kind {kind}')
        return ([],[],[(node.text, node.start_byte, node.end_byte)],[]) if kind=='func' else ([],[(node.text, node.start_byte, node.end_byte)],[],[]) if kind=='arr' else ([(node.text, node.start_byte, node.end_byte)],[],[],[])
    elif node.type == 'field_identifier':
        # print('aaaaaaaaaaaaa')
        return ([],[],[],[(node.text, node.start_byte, node.end_byte)])

    vars, arrays, funcs, fields = [], [], [], []
    for child in node.children:
        # print(child.type, ':', child.text)
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

    updated_code = replace_vars(ast.text, vars, arrays, functions, fields, name_map)

    for r_token, regex in zip(['STR', 'STR', 'CHAR', 'NUM', 'NUM'], [RE_STR, RE_STR_MULTI_LINE, RE_CHARS, RE_NUMBERS, RE_HEXA]):
        updated_code = replace_constants(updated_code, r_token, regex)

    return updated_code


def generate_replaced(code, num_generator=generate_serial_numbers):
    tree = parse(code.lstrip(), 'c')
    updated_code = update_var_names(tree.root_node, num_generator)

    return updated_code




code = '''

	balance_gap = min(low_wmark_pages(zone),
		(zone->managed_pages + KSWAPD_ZONE_BALANCE_GAP_RATIO-1) /
			KSWAPD_ZONE_BALANCE_GAP_RATIO);
	watermark = high_wmark_pages(zone) + balance_gap + (2UL << sc->order);
	watermark_ok = zone_watermark_ok_safe(zone, 0, watermark, 0, 0);

'''


# with open('WTF.c', 'w') as f, open('/home/talkad/OpenMPdb/tokenizer/aa.c', 'r') as f2:
#     f.write(generate_replaced(code))
