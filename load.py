from parsers.cParser import CLoopParser

parser = CLoopParser('asd', 'c_loops2')

data = parser.load('/home/talkad/LIGHTBITS_SHARE/c_loops/1/example/0')
# print(f'pragma: {data.omp_pragma}')
print('code:\n')
print(data.inner_functions)