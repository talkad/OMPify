import random
import tree_sitter
from tree_sitter import Language, Parser


def get_parser(lang):
    LANGUAGE = Language('./my-languages.so', lang.lower())
    parser = Parser()
    parser.set_language(LANGUAGE)

    return parser


def parse(code, lang):
    parser = get_parser(lang)

    tree = parser.parse(bytes(code, 'utf8'))
    return tree


def create_dfg(ast):
    pass


def replace_vars(code, vars, arrays, name_map):
    updated_code = ''
    prev_idx = 0

    vars = vars+arrays
    vars.sort(key=lambda tup: tup[1])
    for var, start, end in vars:
        updated_code += code[prev_idx:start-1].decode() + str(name_map[var])
        prev_idx = end - 1

    updated_code += code[prev_idx:].decode()

    return updated_code


def get_identifires(node, is_array=False):
    '''
        Find identifires names in code

        Parameters:
            node - decleration node in the AST
            is_array - wherther the given node is array decleration
        Return:
            Two list, one with variable names and the other with array names
    '''
    if node.type == 'identifier':
        return ([(node.text, node.start_byte, node.end_byte)],[]) if not is_array else ([],[(node.text, node.start_byte, node.end_byte)])

    vars, arrays = [], []
    for child in node.children:
        va, ar = get_identifires(child, is_array=(child.type == 'array_declarator' or is_array))
        vars, arrays = vars+va, arrays+ar

    return vars, arrays


def generate_serial_numbers(N):
    numbers = list(range(N))
    random.shuffle(numbers)

    return numbers


def update_var_names(ast, num_generator):
    name_map = {}
    vars, arrays = get_identifires(ast)
    unique_vars, unique_arrays = list(set([var[0] for var in vars])), list(set([arr[0] for arr in arrays]))

    random_numbers_vars = num_generator(len(unique_vars))
    random_numbers_arrays = num_generator(len(unique_arrays))

    for var, num in zip(unique_vars, random_numbers_vars):
        name_map[var] = f'var_{num}'

    for var, num in zip(unique_arrays, random_numbers_arrays):
        name_map[var] = f'arr_{num}'

    updated_code = replace_vars(ast.text, vars, arrays, name_map)

    return updated_code




code = '''
static long num_steps = 100000; 
double step;

int main ()
{
    double pi, sum = 0.0;
    int arr[100][100];
    step = 1.0/(double) num_steps;

        for (int i=0;i< num_steps; i++){
            double x = (i+0.5)*step;
            sum = sum + 4.0/(1.0+x*x);
        }

    pi = step * sum;

    return pi;
}
'''



tree = parse(code, 'c')
# print(dir(tree))

print(update_var_names(tree.root_node, generate_serial_numbers))
