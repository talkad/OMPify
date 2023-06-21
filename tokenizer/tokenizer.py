import re
import code_tokenize as ctok
import sys
import json
from tqdm import tqdm
import os
from transformers import PreTrainedTokenizerFast, AutoTokenizer
from transformers import GPT2Tokenizer


# sys.path.extend(['.','/home/talkad/Downloads/thesis/data_gathering_script/database_creator/parsers/HPCorpus_parser'])
sys.path.extend(['.','/home/talkad/OpenMPdb/database_creator/parsers/HPCorpus_parser'])

import parse_tools, preprocess
from convert_representation import code2xsbt, code2dfg


tokenizer_gpt = GPT2Tokenizer.from_pretrained("gpt2")
tokenizer_deepscc = AutoTokenizer.from_pretrained('NTUYG/DeepSCC-RoBERTa')


# class CodeTokenizer(PreTrainedTokenizerFast):
#     def __init__(self, *args, **kwargs):
#         super().__init__(*args, **kwargs)
        
#     def _tokenize(self, code):
#         code = generate_replaced(code)
#         tokens = ctok.tokenize(code, lang = "c", syntax_error = "ignore")

#         return tokens

def log(file_name, msg):
    with open(file_name, 'w') as f:
        f.write(f'{msg}\n')


def tokenize_code(code, replaced=False):
    code = preprocess.remove_comments(code)

    if len(code.strip()) == 0:
        return []

    if replaced:
        code = parse_tools.generate_replaced(code, num_generator=parse_tools.generate_random_numbers)
        
    tokens = ctok.tokenize(code, lang = "c", syntax_error = "ignore")

    updated_tokens = []
    for token in tokens:
        try:
            str_token = token.text.strip()
        except:
            str_token = 'TOKEN'

        if replaced and any([str_token.startswith(prefix) for prefix in parse_tools.replaced_prefixes.values()]):
            updated_tokens += list(str_token.split('_'))
        else:
            updated_tokens.append(str_token)
                                
    return updated_tokens


def tokenize_ast(code, replaced=False):
    ast = code2xsbt(code)
    return ast.split()


def tokenize_dfg(code, replaced=False):
    dfg = code2dfg(code)
    updated_dfg = []

    if replaced:
        for cnt in dfg:
            sample = (tuple(cnt[0].split('_')), cnt[1], cnt[2], [tuple(var.split('_')) for var in cnt[3]], cnt[4])
            updated_dfg.append(sample)
    else:
        updated_dfg = dfg

    return updated_dfg


def tokenize_deepSCC(code):
    return tokenizer_deepscc.tokenize(code)


def tokenize_gpt(code):
    return tokenizer_gpt.tokenize(code)



# with open("ours_vocab.json", "r") as f:
#     vocab = json.load(f)

#     vals = vocab.values()
#     print(f'OOVs : {sum(list(vals)[1100:])/sum(vals)}')
# exit()


# iterate over sub-HPCorpus
# json_dir = '/home/talkad/Downloads/thesis/data_gathering_script/tokenizer/HPCorpus'
# json_dir = '/home/talkad/OpenMPdb/tokenizer/HPCorpus'

# occurrences = {}
# total_tokens, amount_samples = 0, 0

# for json_file in os.listdir(json_dir):
#     with open(os.path.join(json_dir, json_file), 'r') as f:
#         for idx, line in tqdm(enumerate(f)):
#             if idx > 450:
#                 break

#             js = json.loads(line.strip())

#             if 'content' not in js:
#                 continue

#             for _, func in preprocess.extract_funcs(js['content']):
#                 # tokens = tokenize_gpt(func)
#                 tokens = tokenize(func, replaced=True)

#                 total_tokens += len(tokens)
#                 amount_samples += 1

#                 # print(func)
#                 # print(total_tokens)
#                 # break

#                 for token in tokens:
#                     occurrences[token] = 1 if token not in occurrences else occurrences[token]+1

#             # break

# sorted_data = sorted(occurrences.items(), key=lambda x: x[1], reverse=True)
# sorted_dict = {k: v for k, v in sorted_data}

# print(f'AVG tokens per sample: {total_tokens/amount_samples}')

# with open("ours_vocab.json", "w") as outfile:
#     json.dump(sorted_dict, outfile, indent=4)


# 450 samples
# gpt2 -     AVG tokens per sample: 14050.552106430156
# deepSCC -  AVG tokens per sample: 14050.545454545454
# ctok -     AVG tokens per sample: 4609.314855875831
# ours -     AVG tokens per sample: 5406.760532150776
# no split - AVG tokens per sample: 4239.538802660754

# results per function
# gpt2 -    AVG tokens per sample: 382.46796409846263
# ctok -    AVG tokens per sample: 146.37741046831957
# ours -    AVG tokens per sample: 198.7129654314405


code = '''
#include <stdio.h>

int func_879() {
    int arr_285[num_405 + num_293];
    int var_723, var_651;
    int var_914, var_640;
    int var_841 = num_167;

    for (var_723 = num_167; var_723 < num_405; var_723++) {
        arr_285[var_723] = num_76;
    }

    for (var_651 = num_405; var_651 > num_167; var_651 -= num_613) {
        var_640 = num_167;

        var_723 = var_651;
        for (;;) {
            var_640 += arr_285[var_723] * num_274;
            var_914 = num_194 * var_723 - num_293;

            arr_285[var_723] = var_640 % var_914;
            var_640 /= var_914;
            var_723--;
            if (var_723 == num_167) break;
            var_640 *= var_723;
        }
        func_819(str_523, var_841 + var_640 / num_274);
        var_841 = var_640 % num_274;
    }

    return num_167;
}

'''

print(tokenize_ast(code, replaced=True))