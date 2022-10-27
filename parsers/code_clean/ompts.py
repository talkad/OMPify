import re

redundant_ompts = re.compile("<ompts:testdescription>.*<\/ompts:testdescription>|<ompts:ompversion>.*<\/ompts:ompversion>|<ompts:directive>.*<\/ompts:directive>|<ompts:dependences>.*<\/ompts:dependences>|<ompts:.*?>|<\/ompts:.*>")


def log(file_name, msg):
    with open(file_name, 'a') as f:
        f.write(f'{msg}\n')


def remove_namespace(code):
    code_buf = []

    for line in code.split('\n'):
        l = line.lower().split()

        if l[0] == 'using' and l[1] == 'namespace':
            continue

        code_buf.append(line)

    return code_buf


def remove_paren(code):
    flag = False
    num_paren = 0
    idx = 0

    for letter in code:
        if flag and num_paren == 0:
            return code[idx: ]

        if letter == '(':
            flag = True
            num_paren += 1
        elif letter == ')':
            num_paren -= 1

        idx += 1

    return ''


def remove_attribute(code):
    splitted_code = code.split('__attribute__')
    
    if len(splitted_code) == 1:
        return code

    updated_code = list(map(lambda code: remove_paren(code), splitted_code[1:]))

    return ''.join(list(splitted_code[0]) + updated_code)





with open('/home/talkad/Desktop/error_logger.txt', 'r') as err: 
    errors = err.read().split('\n\n')

for error in errors:
    match = re.search(r'Parser Error: (.*) ->\n .*:(\d+):(\d+): before:', error)

    if match is None:
        continue

    path, line, pos = m.group(1), m.group(2), m.group(3)

    with open(path, 'r') as f:
        code = f.read()


    match = re.search(r'(\w+)\W*$', code.split('\n')[line-1][:pos-1])
    if match is None:
        continue

    typedef = match.group(1)

    
    


# 

# with open('/home/talkad/Downloads/thesis/data_gathering_script/asd/1/omp_for_schedule_static_3.c', 'r') as f:
#     code = f.read()

# # code = redundant_ompts.sub("", code)
# print(code)


# *
for line, pos in re.findall(r':(\d+):(\d+): before:', 'Parser Error: /home/talkad/Downloads/thesis/data_gathering_script/parsers/../repositories_openMP/bdevans/spike/source/spike.c ->/tmp/tmppyv3kf03.c:12:18: before: *'):
    print(int(line))

    # (\w+)\W*$

