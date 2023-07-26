from .parse_tools import parse


def iterate_tree(node):
    
    if node.children:
        return list(map(iterate_tree, node.children))
    else:
        return [node.text.decode()]
    

def flatten_list(nested_list):
    flattened_list = []

    for item in nested_list:
        if isinstance(item, list):
            flattened_list.extend(flatten_list(item))
        else:
            flattened_list.append(item)

    return flattened_list


def lexicalize(code, lang='c'):
    tree = parse(code, lang=lang)

    try:
        code = flatten_list(iterate_tree(tree.root_node))
    except RecursionError:
        return ''

    return ' '.join(code)
