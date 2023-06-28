

STATEMENT_ENDING_STRINGS = {
    'c': ['statement', 'expression', 'declaration'],
    'cpp': ['statement', 'expression', 'declaration'],
    'fortran': ['statement', 'expression', 'declaration']
}


def is_statement_node(node, lang):
    """
    Return whether the node is a statement level node.

    Args:
        node (tree_sitter.Node): Node to be queried
        lang (str): Source code language

    Returns:
        bool: True if given node is a statement node

    """
    endings = STATEMENT_ENDING_STRINGS[lang]
    end = node.type.split('_')[-1]
    if end in endings:
        return True
    else:
        return False


def get_node_type(node, lang):
    """
    Return the type of node

    Args:
        node (tree_sitter.Node): Node to be queried
        lang (str): Source code language

    Returns:
        str: Type of the node

    """
    return node.type


def __statement_xsbt(node, lang):
    """
    Method used to generate X-SBT recursively.

    Args:
        node (tree_sitter.Node): Root node to traversal
        lang (str): Source code language

    Returns:
        list[str]: List of strings representing node types

    """
    xsbt = []

    if len(node.children) == 0:
        if is_statement_node(node, lang):
            xsbt.append(get_node_type(node, lang))
    else:
        if is_statement_node(node, lang):
            xsbt.append(f'{get_node_type(node, lang)}__')
        len_before = len(xsbt)
        for child in node.children:
            xsbt += __statement_xsbt(node=child, lang=lang)
        if len_before == len(xsbt) and len_before != 0:
            xsbt[-1] = get_node_type(node, lang)
        elif is_statement_node(node, lang):
            xsbt.append(f'__{get_node_type(node, lang)}')

    return xsbt


def generate_statement_xsbt(node, lang):
    """
    Generate X-SBT string.

    Args:
        node (tree_sitter.Node): Root node to traversal
        lang (str): Source code language

    Returns:
        str: X-SBT string

    """
    tokens = __statement_xsbt(node=node, lang=lang)
    return ' '.join(tokens)


def generate_naive_ast(node, lang):
    """
    Generate AST string by wrapping compound structures with brackets
    """
    if node.children:
        return f"{node.type} ( {' '.join([generate_naive_ast(child, lang) for child in node.children])} )"
    else:
        return node.text.decode()