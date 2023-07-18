import os

org_js = set(os.listdir('/tier2/bgu/bigQuery_repos/cpp'))
new_js = set([file[:-1] for file in os.listdir('/tier2/bgu/HPCorpus_preprocess/cpp')])

print(len(org_js), len(new_js))
print(org_js - new_js)



# with open('file.txt', 'w') as f:
#     f.write('aaaaaa')

# with open('file.txt', 'w') as f:
#     f.write('bb')