import os
# from tqdm import tqdm
import json

amount_repo = 0

dir_path = '/home/talkad/LIGHTBITS_SHARE/repos_omp'

print(len(os.listdir(dir_path)))

# for username in os.listdir(dir_path):
#     amount_repo += len(os.path.join(dir_path, username))

# print(amount_repo)

# num_c, num_cpp = 0, 0
# for idx, (root, dirs, files) in enumerate(os.walk('/home/talkad/Downloads/thesis/data_gathering_script/database_creator/repositories_openMP')):
#     for file in files:
#         # if file.endswith('.cpp'):
#         num_c += 1

#     # num += len(files)

# print(num_c, num_cpp)

# with open('/home/talkad/Downloads/database.json', 'r') as f:
#     data = json.load(f)

#     mem = []
#     for sample in data:
#         name = '_'.join(sample.split('_')[:2])
#         if name not in mem:
#             mem.append(name)
#     print(mem[0])
#     print(len(mem))


repos, files =0, 0
path = '/home/talkad/shared/nadavsc/c'
for js in os.listdir(path):
    with open(os.path.join(path, js), 'r') as f:
        data = json.load(f)

        mem = []
        for sample in data:
            name = '_'.join(sample.split('_')[:2])
            if name not in mem:
                mem.append(name)
        print(mem[0])
        

