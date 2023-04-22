import pickle 
import json
import os
import sys 
from tqdm import tqdm

sys.path.extend(['.','/home/talkad/Downloads/thesis/data_gathering_script'])

from pycparser import c_generator
from downstream_tasks.augmentation.replace_vars import CounterIdVisitor, ReplaceIdsVisitor



def load_sample(sample_path):
    omp_pragma = None

    with open(os.path.join(sample_path, 'ast.pickle'), 'rb') as f:
        asts = pickle.load(f)

    with open(os.path.join(sample_path, f"code.{'cpp' if 'cpp_loops' in sample_path else 'c'}"), 'r') as f:
        code = f.read()

    try:
        with open(os.path.join(sample_path, 'pragma.pickle'), 'rb') as f:
            omp_pragma = pickle.load(f)
    except FileNotFoundError:
        pass

    return omp_pragma, asts.ast_loop, code


def create_sample(sample_path, update_ratio=0):
    '''
        given path to sample folder and the ratio of the variable names that will be replaced
        return tuple (code, label)
    '''
    find_variables = CounterIdVisitor()
    generator = c_generator.CGenerator()

    omp_pragma, ast_loop, code = load_sample(sample_path)
    find_variables.visit(ast_loop)

    replacor = ReplaceIdsVisitor(find_variables.ids, find_variables.array, find_variables.struct, find_variables.func, update_ratio)
    replacor.visit(ast_loop)
    
    try:
        code = generator.visit(ast_loop)
    except:
        pass
        
    return code, 0 if omp_pragma is None else 1

