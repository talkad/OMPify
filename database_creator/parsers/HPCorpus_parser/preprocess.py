import re
import hashlib
import parse_tools

redundant_line_comments = re.compile("\/\/.*")
redundant_multiline_comments = re.compile("\/\*.*?\*\/", re.MULTILINE|re.DOTALL)
redundant_includes = re.compile("^\W*#\W*include.*$", re.MULTILINE)


LANG_IMPORTS = {
    "cpp": """#include <iostream>
    #include <cstdlib>
    #include <string>
    #include <vector>
    #include <fstream>
    #include <iomanip>
    #include <bits/stdc++.h>
    using namespace std;""",

    "c":"""#include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    #include <math.h>""", 

    "fortran":""
}


def get_hash(code):
    hash = hashlib.sha256(code.encode('UTF-8'))
    return hash.hexdigest()


def remove_comments(code, c_lang=True):
    code = redundant_line_comments.sub("\n", code)
    code = redundant_multiline_comments.sub("\n", code)

    return code


def remove_headers(code, c_lang=True):
    code = redundant_includes.sub("", code)
    return code


def add_headers(code, lang='c'):
    return f'{LANG_IMPORTS[lang]}\n{code}'


def node2code(code, node):

    start_byte = node.start_byte-1
    end_byte = node.end_byte
    func = code[start_byte:end_byte]

    func_name = func[:func.find('(')]
    func_name = func_name.rstrip()
    func_name = func_name[func_name.rfind(' ')+1:]

    return func_name, func


def get_functions(code, node):
    # print(node.type, node.text)
    if node.type == 'function_definition':
        func_name, code = node2code(code, node)

        return [(func_name, code)]

    nodes = []
    for child in node.children:
        nodes += get_functions(code, child)
    return nodes


def get_for_loops(code, node):
    if node.type == 'for_statement':
        return [(node, node.text)]

    nodes = []
    for child in node.children:
        nodes += get_for_loops(child)
    return nodes


def get_func_declaration(func):
    return func[:func.find(')')+1] + ';'


def get_func_calls(node, func_call=False):
	if func_call and node.type == 'identifier':
		return [node.text.decode()]

	func_calls = []
	for child in node.children:
		func_calls += get_func_calls(child, func_call=(node.type=='call_expression'))

	return func_calls


def extract_code_struct(code, traverse_func=get_functions, lang='c'):

    parser = parse_tools.parsers[lang]

    tree = parser.parse(bytes(code, 'utf8'))

    try:
        result = traverse_func(code, tree.root_node)
    except Exception as e:
        # print('extraction error: ', e)
        result = []

    return result


def get_filename(filename):
     return filename[:filename.rfind('.')]