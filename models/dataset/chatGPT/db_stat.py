import os
from tqdm import tqdm

dir_path = '/home/talkad/LIGHTBITS_SHARE/chatGPT_DB/DB/database'
counter = 0

total = {0:0, 1:0}

for root, dirs, files in tqdm(os.walk(dir_path)):
    
    if not dirs:
        if os.path.exists(os.path.join(root, 'pragma.c')):
            total[1] += 1
            # print(root)
            with open(os.path.join(root, 'code.c'), 'r') as f:
                code = f.read()
                if " omp" in code:
                    print(os.path.join(root, 'code.c'))
                    counter += 1
        else:
            total[0] += 1


print(total)
print(counter)
