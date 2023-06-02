import re
import random
import tree_sitter
import textwrap
from tree_sitter import Language, Parser

RE_NUMBERS = re.compile(r"(?<![_a-zA-Z])-?\b[0-9]+(?:\.[0-9]+)?\b(?![0-9.-])")

VAR, ARR, FUNC, NUM = 1, 2, 3, 4
replaced_prefixes = { VAR: 'var_',
                      ARR: 'arr_',
                      FUNC: 'func_',
                      NUM: 'num_'
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


def replace_vars(code, vars, arrays, functions, name_map):
    updated_code = ''
    prev_idx = 0
    offset = 1 # count_newlines(code)

    vars = vars+arrays+functions
    vars.sort(key=lambda tup: tup[1])
    for var, start, end in vars:
        updated_code += code[prev_idx:start-offset].decode() + str(name_map[var])
        print(updated_code)
        print('=====')
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
        return ([(node.text, node.start_byte, node.end_byte)],[],[]) if kind=='' else ([],[(node.text, node.start_byte, node.end_byte)],[]) if kind=='arr' else ([],[],[(node.text, node.start_byte, node.end_byte)])

    vars, arrays, funcs = [], [], []
    for child in node.children:
        # print(child.type, ':', child.text)
        va, ar, fu = get_identifiers(child, kind=(kind if len(kind)>0 else 
                                                  'arr' if child.type == 'array_declarator' else
                                                  'func' if child.type in ['call_expression', 'function_declarator'] else ''))
        vars, arrays, funcs = vars+va, arrays+ar, funcs+fu

    return vars, arrays, funcs


def generate_serial_numbers(N):
    numbers = list(range(N))
    random.shuffle(numbers)

    return numbers


# def update_var_names(ast, num_generator):
#     name_map = {}
#     vars, arrays, functions = get_identifiers(ast)
#     print(vars, arrays, functions)
#     unique_vars, unique_arrays, unique_funcs = list(set([var[0] for var in vars])), list(set([arr[0] for arr in arrays])), list(set([func[0] for func in functions]))

#     random_numbers_vars = num_generator(len(unique_vars))
#     random_numbers_arrays = num_generator(len(unique_arrays))
#     random_numbers_functions = num_generator(len(unique_funcs))

#     for var, num in zip(unique_vars, random_numbers_vars):
#         name_map[var] = f'var_{num}'

#     for var, num in zip(unique_arrays, random_numbers_arrays):
#         name_map[var] = f'arr_{num}'

#     for var, num in zip(unique_funcs, random_numbers_functions):
#         name_map[var] = f'func_{num}'

#     updated_code = replace_vars(ast.text, vars, arrays, functions, name_map)

#     return updated_code


def replace_numbers(code, num_generator):
    matches = RE_NUMBERS.findall(code)
    random_numbers = num_generator(len(matches))
    matches = RE_NUMBERS.finditer(code)

    offset = 0
    for match, num in zip(matches, random_numbers):
        # print(f'{replaced_prefixes[NUM]}{num}', f'{match.start()}-{match.end()}')
        start = match.start() + offset
        end = match.end() + offset
        code = code[:start] + f'{replaced_prefixes[NUM]}{num}' + code[end:]
        offset += len(f'{replaced_prefixes[NUM]}{num}') - len(match.group())

    return code


def update_var_names(ast, num_generator):
    name_map = {}
    vars, arrays, functions = get_identifiers(ast)

    for type, identifiers in zip([VAR, ARR, FUNC], [vars, arrays, functions]):
        unique_vars= list(set([var[0] for var in identifiers]))
        random_numbers_vars = num_generator(len(unique_vars))

        for var, num in zip(unique_vars, random_numbers_vars):
            name_map[var] = f'{replaced_prefixes[type]}{num}'

    updated_code = replace_vars(ast.text, vars, arrays, functions, name_map)
    updated_code= replace_numbers(updated_code, num_generator)

    return updated_code


def generate_replaced(code, num_generator=generate_serial_numbers):
    tree = parse(code, 'c')
    updated_code = update_var_names(tree.root_node, num_generator)

    return updated_code




code = '''
static long num_steps = 100000; 
double step;

int shit(){
    return 0;
}

int main ()
{
    double pi, sum = 0.0;
    int arr[100][100];
    step = 1.0/(double) num_steps;

        for (int i=0;i< num_steps; i++){
            double x = (i+0.5)*step;
            sum = sum + 4.0/(1+x*x);
        }

    pi = step * sum;
    shit();
    return pi;
}
'''

code = """
#include <stdio.h>

int main() {
    int r[2800 + 1];
    int i, k;
    int b, d;
    int c = 0;

}

"""

# print(generate_replaced(code))
