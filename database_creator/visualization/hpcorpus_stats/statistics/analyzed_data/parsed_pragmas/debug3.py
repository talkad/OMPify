import re
from tqdm import tqdm


def for_no_paralle(pragma):

    pattern = r'u\'(.*?)\''
    matches = re.findall(pattern, pragma)
    
    return 'for' in matches and 'parallel' not in matches

total = 0
with open('c_parsing.log', 'r') as f:
    for l in tqdm(f):
        if for_no_paralle(l):
            total += 1
print(total)


# fortran
# 9412

# cpp
# 18124

# c
# 7236
