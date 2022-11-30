import re

redundant_comments = re.compile('\/\*.*\*\/', re.MULTILINE|re.DOTALL)


def is_for_pragma(line, lang='c'):
    '''
    Return true if the given line is for-pragma
    '''
    sub_line = line.lstrip() # remove redundant white spaces

    if lang == 'c':
        return sub_line.startswith('#pragma ') and ' omp ' in line and ' for' in line

    return sub_line.startswith('!$omp ') and ' do' in line and ' end' not in line


def count_for(file_path):
    '''
    Returns the amout of for-loops and pragmas exist in a given file
    '''
    loop_amount, pragma_amount = 0, 0

    with open(file_path, 'r') as f:
        try:
            code = f.read()
        except UnicodeDecodeError:
            return 0

        code = redundant_comments.sub("\n", code)

        for line in code.split('\n'):
            l = line.lower()
			
            # if is_for(l):
            #     loop_amount += 1

            if is_for_pragma(l):
                pragma_amount += 1

    return loop_amount, pragma_amount


with open('/home/talkad/Downloads/thesis/data_gathering_script/fail_pragma.txt', 'r') as f:
    logs = f.read()

errs = logs.split('===================')

# with open('fail_pragma2.txt', 'w') as f:
total, fail, missing = 0,0,0
missed_pragma, unparsed_pragmas = 0, 0


for err in errs:
    msg = err.split('\n')

    if len(msg) < 3:
        break

    i = 0 if len(msg[0]) > 0 else 1
    file_path = msg[i]

    stat = msg [i +1] 

    match = re.search(r'found (\d+) \| there are (\d+)$', err)

    _, pragma_amount = count_for(file_path)

    if stat == 'failed to parse':
        fail += 1
        unparsed_pragmas += int(match.group(2))
    elif stat == 'missing pragmas':
        missing += 1
        missed_pragma += (pragma_amount - int(match.group(1)))

    if pragma_amount > int(match.group(1)):
        # f.write(f'{file_path}\n{msg[2]}\nfound {match.group(1)} | there are {pragma_amount}\n===================\n')
        total += 1

print(total)
print(f'files failed {fail}: pragmas {unparsed_pragmas} ')
print(f'missed pragmas {missed_pragma}')

# 1033
# files failed 946: pragmas 2688 
# missed pragmas 254
