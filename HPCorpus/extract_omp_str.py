import os
import sys
import json



# data_dir = '/home/1010/talkad/Downloads/HPCorpus_final/fine_tune/c/batches'
# json_file = f'batch_{sys.argv[1]}.jsonl'
# counter = 0

# with open('res.txt', 'a') as res, open(os.path.join(data_dir, json_file), 'r') as f:
#     for l in f:
#         js = json.loads(l.strip())
#         code = js['code']

#         for line in code.split('\n'):
#             line_clean = line.strip().lower()

#             if line_clean.startswith('#pragma') and ' omp' in line_clean and ' for' in line_clean:
#                 counter+=1

#     res.write(f'{counter}\n')



counter = 0

with open('res.txt', 'r') as res:
    for l in res:
        counter += int(l)

print(counter)