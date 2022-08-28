import os
import pycparser
from pycparser.c_ast import For
import pickle
from functools import reduce


cpp_args = ['-nostdinc',  '-E', r'-I/home/talkad/Downloads/thesis/data_gathering_script/teem-parallel', r'-I/home/talkad/Downloads/thesis/data_gathering_script/teem-parallel/a']

ast = pycparser.parse_file("/home/talkad/Downloads/thesis/data_gathering_script/temp/dice.c", use_cpp=True, cpp_path='mpicc', cpp_args = cpp_args)