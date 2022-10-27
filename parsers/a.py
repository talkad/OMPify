import re

# path = '/home/talkad/Downloads/thesis/data_gathering_script/parsers/../repositories_openMP/ryanfb/teem-parallel/src/unrrdu/dice.c'
# with open(path, 'r') as f:
#     code = f.read()

#     line_num = 34
#     line_pos = 50

#     lines = code.split('\n')
#     print(path)
#     print(f'line {line_num}: {lines[line_num - 1]}')
#     print(f'{lines[line_num - 1][:line_pos]}\n ============\n')



redundant_ompts = re.compile("<ompts:testdescription>.*<\/ompts:testdescription>|<ompts:ompversion>.*<\/ompts:ompversion>|<ompts:directive>.*<\/ompts:directive>|<ompts:dependences>.*<\/ompts:dependences>|<ompts:.*?>|<\/ompts:.*>")



def log(file_name, msg):
    with open(file_name, 'a') as f:
        f.write(f'{msg}\n')


def remove_namespace(code):
    code_buf = []

    for line in code.split('\n'):
        l = line.lower().split()

        if len(l) > 2 and l[0] == 'using' and l[1] == 'namespace':
            continue

        code_buf.append(line)

    return '\n'.join(code_buf)


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


def remove_ompt(code):
    return redundant_ompts.sub("", code)


def update_code_pipline(code):
    code = remove_namespace(code)
    code = remove_attribute(code)
    code = remove_ompt(code)

    return code


code = """

int __attribute__((noinline)) test (long j1, long k1, long j2, long k2)
{
  test1 (j1, k1, j2, k2);
  test2 (j1, k1, j2, k2);
  test3 (j1, k1, j2, k2);
  test4 (j1, k1, j2, k2);
  test5 (j1, k1, j2, k2);
  test6 (j1, k1, j2, k2);
  test7 (j1, k1, j2, k2);
  test8 (j1, k1, j2, k2);
  test9 (j1, k1, j2, k2);
  test10 (j1, k1, j2, k2);
  test11 (j1, k1, j2, k2);
  test12 (j1, k1, j2, k2);
  test13 (j1, k1, j2, k2);
  test14 (j1, k1, j2, k2);
  test15 (j1, k1, j2, k2);
  test16 (j1, k1, j2, k2);
  return cnt;
}
"""

print(update_code_pipline(code))