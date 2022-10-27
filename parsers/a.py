import re

path = '/home/talkad/Downloads/thesis/data_gathering_script/parsers/../repositories_openMP/ryanfb/teem-parallel/src/unrrdu/dice.c'
with open(path, 'r') as f:
    code = f.read()

    line_num = 34
    line_pos = 50

    lines = code.split('\n')
    print(path)
    print(f'line {line_num}: {lines[line_num - 1]}')
    print(f'{lines[line_num - 1][:line_pos]}\n ============\n')