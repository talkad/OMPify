from parsers.cParser import CLoopParser
import matplotlib.pyplot as plt 
import numpy as np
import json


parser = CLoopParser('', 'c_loops')
code_length_omp = {}
code_length = {}

# # load all folder path (each folder represents OmpLoop object)
# with open('/home/talkad/Downloads/thesis/data_gathering_script/updated_db.json', 'r') as f:
#     database = json.load(f)

# for data_path in database.values():
#     data = parser.load(data_path)

#     idx = int(len(data.textual_loop.split('\n'))/10)
#     # code_length[idx] = (code_length[idx] if idx in code_length else 0) + 1

#     if data.omp_pragma is not None:
#         code_length_omp[idx] = (code_length_omp[idx] if idx in code_length_omp else 0) + 1
#     else:
#         code_length[idx] = (code_length[idx] if idx in code_length else 0) + 1

# print(code_length)
# print(code_length_omp)


upper_limit = 3

code_length = {2: 905, 1: 3598, 0: 8901, 3: 391, 4: 278, 6: 106, 9: 44, 7: 70, 86: 2, 28: 2, 236: 4, 30: 2, 237: 2, 5: 101, 14: 16, 8: 48, 13: 25, 11: 22, 10: 13, 12: 9, 26: 4, 29: 2, 18: 3, 15: 9, 22: 3, 25: 2, 19: 13, 16: 3, 24: 4, 53: 2, 21: 2, 69: 1, 27: 1, 20: 1, 54: 1, 31: 1, 17: 3, 44: 1, 50: 1, 89: 2, 36: 1}
code_length_omp = {1: 4737, 0: 7233, 5: 135, 2: 1200, 3: 395, 6: 120, 4: 224, 10: 39, 12: 30, 9: 41, 8: 62, 13: 13, 11: 36, 7: 79, 41: 6, 14: 20, 19: 1, 26: 7, 15: 7, 16: 5, 20: 8, 17: 6, 22: 6, 31: 4, 96: 1, 81: 1, 24: 1, 23: 5, 28: 6, 290: 1, 65: 1, 21: 5, 18: 8, 32: 1, 43: 2, 33: 2, 45: 1, 38: 1, 29: 3, 39: 1, 100: 1, 119: 1, 101: 5, 121: 1, 25: 2, 30: 2, 70: 1, 36: 1, 51: 2, 37: 1, 52: 1, 34: 1, 49: 1, 35: 2, 50: 1, 57: 1, 76: 1, 54: 3, 83: 4, 47: 2}


code_length = {k: code_length[k] for k in sorted(code_length)}
code_length_omp = {k: code_length_omp[k] for k in sorted(code_length_omp)}

code_upper = sum([code_length[k] for k in code_length if k>=upper_limit])
omp_upper = sum([code_length_omp[k] for k in code_length_omp if k>=upper_limit])


X = [f'{i*10}-{i*10+10}' for i in range(upper_limit)] + [f'<{upper_limit*10}']
Ypos = list(code_length.values())[:upper_limit] + [code_upper]
Zneg = list(code_length_omp.values())[:upper_limit] + [omp_upper]
  
X_axis = np.arange(len(X))
  
plt.title('Distribution of Number of Lines')
plt.xlabel('# lines')
plt.ylabel('# examples')

plt.bar(X_axis - 0.2, Ypos, 0.4, label = 'without omp')
plt.bar(X_axis + 0.2, Zneg, 0.4, label = 'with omp')

plt.xticks(X_axis, X) #, rotation = 100)

plt.legend()
# plt.show()
plt.savefig('dist.png')