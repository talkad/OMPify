import re
import os 
import json
from tqdm import tqdm
import tree_sitter
from tree_sitter import Language, Parser


redundant_line_comments = re.compile("\/\/.*")
redundant_multiline_comments = re.compile("\/\*.*?\*\/", re.MULTILINE|re.DOTALL)

redundant_includes = re.compile("^\W*#\W*include.*$", re.MULTILINE)


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

def remove_headers(code, c_lang=True):
    code = redundant_includes.sub("", code)
    return code

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

def get_func_calls(node, func_call=False):
	# print(node.type, node.text)
	if func_call and node.type == 'identifier':
		return [node.text.decode()]

	func_calls = []
	for child in node.children:
		func_calls += get_func_calls(child, func_call=(node.type=='call_expression'))

	return func_calls


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



code = """
int main(int argc, char[] argv*){
	func_2(555);
	return 0;
}

int func_2(type a){
	printf("Hello world");
	}
"""

# with open('a.c', 'w') as f:
#     f.write(remove_headers(code))




json_dir = '/home/talkad/Downloads/thesis/data_gathering_script/tokenizer/HPCorpus'
occurrences = {}

LANGUAGE = Language('./my-languages.so', 'c')
parser = Parser()
parser.set_language(LANGUAGE)

for json_file in os.listdir(json_dir):
	with open(os.path.join(json_dir, json_file), 'r') as f:
		for idx, line in tqdm(enumerate(f)):
			# if idx > 2000:
			# 	break

			js = json.loads(line.strip())

			if 'content' not in js:
				continue

			tree = parser.parse(bytes(js['content'], 'utf8'))
			try:
				result = get_func_calls(tree.root_node)
			except:
				result = []

			for func in result:
				occurrences[func] = 1 if func not in occurrences else occurrences[func]+1


sorted_data = sorted(occurrences.items(), key=lambda x: x[1], reverse=True)
sorted_dict = {k: v for k, v in sorted_data}

with open("function_usage.json", "w") as outfile:
    json.dump(sorted_dict, outfile, indent=4)
