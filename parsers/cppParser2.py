import clang.cindex as cl
# libclang bindings => https://github.com/mapbox/cncc/issues/6#issuecomment-278010018

def ast2code(cursor):
    '''
    Creates textual code for a given cursor (node)
    '''
    code = []
    line = ""
    prev_token = None

    for tok in cursor.get_tokens():
        if prev_token is None:
            prev_token = tok

        prev_location = prev_token.location
        prev_token_end_col = prev_location.column + len(prev_token.spelling)
        cur_location = tok.location
        if cur_location.line > prev_location.line:
            code.append(line)
            line = " " * (cur_location.column - 1)
        else:
            if cur_location.column > (prev_token_end_col):
                line += " "

        line += tok.spelling
        prev_token = tok

    if len(line.strip()) > 0:
        code.append(line)

    return '\n'.join(code)

def print_ast_nodes(cursor, depth=0):
    for ch in cursor.get_children():
        s = ast2code(ch)

        print("  " * depth + str(ch.kind))
        print(s if "omp" in s else '')
        print_ast_nodes(ch, depth + 1)


index = cl.Index.create()
tu = index.parse("/home/kali/Desktop/OpenMPdb/asd/123/par_omp_sort.cpp")
# node = tu.cursor

# print(tu)
# print_ast_nodes(node)
