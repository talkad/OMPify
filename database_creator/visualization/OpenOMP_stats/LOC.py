from parsers.cParser import CLoopParser
from parsers.cppParser import CppLoopParser
from parsers.fortranParser import FortranLoopParser
import matplotlib.pyplot as plt 
import numpy as np
import json

def get_parer(lang='c'):
    if lang == 'c':
        return CLoopParser('', '')
    if lang == 'cpp':
        return CppLoopParser('','')
    return FortranLoopParser('','')


def get_lines_dist(lang='c', diagram=True):
    parser = get_parer(lang)
    code_length_omp = {}
    code_length = {}

    # load all folder path (each folder represents OmpLoop object)
    with open(f'/home/talkad/Downloads/thesis/data_gathering_script/sample_{lang}.json', 'r') as f:
        database = json.load(f)

    for data_path in database.values():
        data = parser.load(data_path)

        idx = int(len(data.textual_loop.split('\n'))/10)
        # code_length[idx] = (code_length[idx] if idx in code_length else 0) + 1

        if data.omp_pragma is not None:
            code_length_omp[idx] = (code_length_omp[idx] if idx in code_length_omp else 0) + 1
        else:
            code_length[idx] = (code_length[idx] if idx in code_length else 0) + 1

    # print(code_length)
    # print(code_length_omp)


    upper_limit = 10

    # code_length =  {2: 342, 0: 770, 4: 51, 1: 1974, 11: 7, 5: 33, 9: 9, 3: 126, 18: 3, 10: 4, 7: 12, 13: 2, 6: 17, 14: 2, 8: 6, 16: 1, 24: 1, 17: 2, 29: 2, 15: 2, 20: 1, 12: 1}
    # code_length_omp = {1: 1567, 2: 250, 0: 458, 3: 148, 4: 97, 12: 11, 5: 40, 6: 38, 14: 12, 11: 13, 15: 9, 16: 6, 7: 25, 9: 16, 19: 6, 8: 11, 30: 2, 10: 13, 22: 1, 28: 3, 23: 1, 29: 3, 13: 4, 37: 1, 38: 1, 27: 2, 35: 1, 36: 1, 17: 1, 34: 2, 20: 2, 21: 1, 24: 1, 46: 1, 39: 1}

    code_length = {k: code_length[k] for k in sorted(code_length)}
    code_length_omp = {k: code_length_omp[k] for k in sorted(code_length_omp)}

    code_upper = sum([code_length[k] for k in code_length if k>=upper_limit])
    omp_upper = sum([code_length_omp[k] for k in code_length_omp if k>=upper_limit])

    X = [f'{i*10}-{i*10+10}' for i in range(upper_limit)] + [f'<{upper_limit*10}']
    Ypos = list(code_length.values())[:upper_limit] + [code_upper]
    Zneg = list(code_length_omp.values())[:upper_limit] + [omp_upper]

    print([i+j for i,j in zip(Ypos, Zneg)])
    
    if diagram: 
        X_axis = np.arange(len(X))
        
        plt.title('Distribution of Number of Lines')
        plt.xlabel('# lines')
        plt.ylabel('# examples')

        plt.bar(X_axis - 0.2, Ypos, 0.4, label = 'without omp')
        plt.bar(X_axis + 0.2, Zneg, 0.4, label = 'with omp')

        plt.xticks(X_axis, X) #, rotation = 100)

        plt.legend()
        plt.show()
        plt.savefig('dist.png')



# # c
# {2: 888, 0: 10300, 9: 59, 7: 32, 1: 4905, 6: 67, 4: 218, 8: 56, 3: 374, 12: 6, 13: 52, 28: 8, 236: 8, 237: 8, 86: 4, 30: 5, 5: 123, 16: 10, 11: 12, 29: 1, 69: 1, 21: 3, 20: 7, 15: 5, 14: 4, 10: 12, 26: 1, 35: 1, 27: 2, 17: 3, 18: 3, 19: 3, 24: 1, 22: 2, 54: 3, 40: 1, 96: 1, 25: 1, 44: 1, 32: 1, 34: 1}
# {1: 4812, 0: 7536, 5: 139, 2: 1223, 3: 399, 6: 120, 4: 227, 10: 39, 12: 30, 9: 41, 8: 66, 13: 13, 11: 38, 7: 79, 41: 6, 14: 20, 19: 1, 26: 7, 15: 7, 16: 5, 20: 8, 17: 6, 22: 6, 31: 4, 96: 1, 81: 1, 24: 1, 23: 5, 28: 6, 290: 1, 65: 1, 21: 5, 18: 8, 32: 1, 43: 2, 33: 2, 45: 1, 38: 1, 29: 3, 39: 1, 100: 1, 119: 1, 101: 5, 121: 1, 25: 2, 30: 2, 70: 1, 36: 1, 51: 2, 37: 1, 52: 1, 34: 1, 49: 1, 35: 2, 50: 1, 57: 1, 76: 1, 54: 3, 83: 4, 47: 2}
# [17836, 9717, 2111, 773, 445, 262, 187, 111, 122, 100, 435]

# # cpp 
# {0: 9288, 2: 691, 3: 261, 1: 3801, 5: 51, 7: 21, 6: 41, 4: 112, 8: 16, 15: 6, 16: 6, 10: 6, 13: 6, 17: 2, 9: 5, 14: 2, 43: 1, 21: 2, 11: 3, 12: 1, 18: 1}
# {1: 2301, 2: 543, 0: 4833, 3: 212, 4: 112, 5: 56, 6: 52, 8: 23, 10: 8, 9: 13, 13: 3, 7: 39, 32: 1, 15: 8, 11: 8, 18: 1, 28: 2, 12: 9, 24: 4, 17: 5, 16: 3, 14: 1, 25: 1, 40: 1, 23: 1, 37: 1}
# [14121, 6102, 1234, 473, 224, 107, 93, 60, 39, 18, 93]

# # fortran
# {2: 342, 0: 770, 4: 51, 1: 1974, 11: 7, 5: 33, 9: 9, 3: 126, 18: 3, 10: 4, 7: 12, 13: 2, 6: 17, 14: 2, 8: 6, 16: 1, 24: 1, 17: 2, 29: 2, 15: 2, 20: 1, 12: 1}
# {1: 1567, 2: 250, 0: 458, 3: 148, 4: 97, 12: 11, 5: 40, 6: 38, 14: 12, 11: 13, 15: 9, 16: 6, 7: 25, 9: 16, 19: 6, 8: 11, 30: 2, 10: 13, 22: 1, 28: 3, 23: 1, 29: 3, 13: 4, 37: 1, 38: 1, 27: 2, 35: 1, 36: 1, 17: 1, 34: 2, 20: 2, 21: 1, 24: 1, 46: 1, 39: 1}
