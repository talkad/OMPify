import pygments 
from pygments.lexers import get_lexer_by_name
import numpy as np
import json
import data.asts.convert_representation as cr
from  data.asts import parse_tools
from data.asts.lexicalization import lexicalize
from tqdm import tqdm

def get_lex_count(texts,lang,show_lex=True):
	lexer = get_lexer_by_name(lang)
	if show_lex:
		print(f'the count is made with: {lexer}')
	pygments_len=sum(len(list(pygments.lex(t, lexer))) for t in texts)
	lex_vocab = sum(len(v) for v in lexer.tokens.values())
	return pygments_len*lex_vocab


def compute_ppl(loss, batch_amount, lex_count):
    return np.exp(loss * batch_amount / lex_count)


def count_replace_tokens():
	total = 0

	with open('/home/1010/talkad/Downloads/HPCorpus_final/fine_tune/fortran/dataset_fortran.jsonl' ,'r') as f:
		for line in tqdm(f):
			js = json.loads(line)

			ast = parse_tools.parse(js['code'].strip(), 'fortran')
			replaced_code = cr.generate_replaced(ast)
			lex = lexicalize(replaced_code, lang='fortran')
			total += len(lex.split())

	return total

# print(count_replace_tokens())


27838486
1249955746

print(compute_ppl(13812.332727938026, 7003, 278384860))