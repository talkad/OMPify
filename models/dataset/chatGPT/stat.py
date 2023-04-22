import os
from tqdm import tqdm
import shutil

folders = []
total, pragmas, gpt_pragma = 0, 0, 0
acc = 0

flag = False
TP, TN, FP, FN =0,0,0,0

# for folder in tqdm(os.listdir('./database')):
#     total += 1

#     if os.path.isfile(os.path.join('./database', folder, 'pragma.c')):
#         pragmas += 1
#         flag = True

#     with open(os.path.join('./database', folder, 'chatGPT.c'), 'r') as f:
#         code = f.read()
#         if '#pragma omp' in code:
#             gpt_pragma += 1

#             if flag:
#                 acc += 1
#         elif '#pragma omp' not in code and not flag:
#             acc +=1
#         # else:
#         #     print(os.path.join('./database', folder, 'chatGPT.c'))

#     flag = False


# print(total, pragmas, gpt_pragma)
# print('accuracy:', acc/total)


# 2506 1034 2356
# accuracy: 0.3770949720670391


db_path = '/home/talkad/LIGHTBITS_SHARE/chatGPT_DB/DB/database'
for folder in tqdm(os.listdir(db_path)):
    label, pred = False, False

    if os.path.isfile(os.path.join(db_path, folder, 'pragma.c')):
        label = True

    with open(os.path.join(db_path, folder, 'chatGPT.c'), 'r') as f:
        code = f.read()
        if '#pragma omp' in code:
            pred = True

    if label and pred:
        TP += 1
    elif label:
        FN += 1
    elif not label and not pred:
        TN += 1
    else:
        FP += 1

print([[TN, FP], [FN, TP]])
# [[61, 1411], [89, 945]]