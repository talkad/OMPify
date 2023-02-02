import json
import os
import pickle


class OmpLoop:
    '''
    Define a class that later will be dumped into pickle file
    '''
    def __init__(self, omp_pragma, ast_loop, inner_functions, textual_loop):
        self.omp_pragma = omp_pragma            # omp pragma associated with the given loop
        self.ast_loop = ast_loop                # ast format representing AST structure of loop
        self.textual_loop = textual_loop        # textual representation of code
        self.inner_functions = inner_functions  # function AST struct. which called inside the given loop (type list[c_ast.FuncDef])


def load(file_path):
    '''
    Load OmpLoop structure from file_path
    '''
    omp_pragma = None

    # with open(os.path.join(file_path, 'ast.pickle'), 'rb') as f:
    #     asts = pickle.load(f)

    with open(os.path.join(file_path, 'code.cpp' if 'cpp_loops' in file_path else 'code.c'), 'r') as f:
        code = f.read()
    try:
        with open(os.path.join(file_path, 'pragma.pickle'), 'rb') as f:
            omp_pragma = pickle.load(f)
    except FileNotFoundError:
        pass

    return OmpLoop(omp_pragma, None, None, code)




updated_data = {}

with open('/home/talkad/Downloads/thesis/data_gathering_script/sample_c.json', 'r') as f:
    data = json.load(f)

    for k,v in data.items():
        omp = load(v)
        # if omp.omp_pragma is not None:
        #     print(omp.omp_pragma.string)
        #     break
        updated_data[int(k)] = {'code': v[len('../../../LIGHTBITS_SHARE/'):] + '/code.c',
                            'pragma': '' if omp.omp_pragma is None else omp.omp_pragma.string,
                            'code_pickle': v[len('../../../LIGHTBITS_SHARE/'):] + '/ast.pickle',
                            'id': k}


with open('/home/talkad/Downloads/thesis/data_gathering_script/sample_c_updated.json', 'w') as f:
    json.dump(updated_data, f, sort_keys=True, indent=4, separators=(',', ': '))