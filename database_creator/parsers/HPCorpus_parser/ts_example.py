import tree_sitter
from tree_sitter import Language, Parser

# Initialize the Python language parser
parser = Language('/home/talkad/Downloads/thesis/data_gathering_script/database_creator/parsers/HPCorpus_parser/my-languages.so', 'c')


LANGUAGE = Language('./my-languages.so', 'c')
parser = Parser()
parser.set_language(LANGUAGE) 

# Sample C code
code = '''
int main(){
    printf(aaa);
}

int main2(){
    printf(aaa);
}
'''

# Parse the code using the Python parser
tree = parser.parse(bytes(code, 'utf8'))


def get_code(node):
    start_byte = node.start_byte
    end_byte = node.end_byte
    func = code[start_byte:end_byte]

    func_name = func[:func.find('(')]
    func_name = func_name.rstrip()
    func_name = func_name[func_name.rfind(' ')+1:]

    return func_name, func



def traverse(node):
    if node.type == 'function_definition':
        func_name, code = get_code(node)
        return [(func_name, code)]

    nodes = []
    for child in node.children:
        nodes += traverse(child)
    return nodes

print(traverse(tree.root_node))
