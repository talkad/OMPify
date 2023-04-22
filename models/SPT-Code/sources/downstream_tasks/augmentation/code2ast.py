from data.asts.ast_parser import parse_ast, generate_statement_xsbt
import enums
import os
import json
import pickle
import random
from tqdm import tqdm

def code2xsbt(source, lang):
    node = parse_ast(source, lang)
    xsbt_str = generate_statement_xsbt(node, lang)

    return xsbt_str


