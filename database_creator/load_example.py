from parsers.cParser import CLoopParser
from pycparser import c_generator

code_generator = c_generator.CGenerator()

parser = CLoopParser('a', 'b')
print(code_generator.visit(parser.load('/home/talkad/LIGHTBITS_SHARE/c_loops_detailed/4phycs/openmp-ita-2018-19/inviare/lastprivate_omp2_si/0').for_node))