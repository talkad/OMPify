import code_tokenize as ctok
import sys
import json
from tqdm import tqdm
import os
from transformers import PreTrainedTokenizerFast, AutoTokenizer


sys.path.extend(['.','/home/talkad/OpenMPdb/database_creator/parsers/HPCorpus_parser'])
import parse_tools, preprocess


# class CodeTokenizer(PreTrainedTokenizerFast):
#     def __init__(self, *args, **kwargs):
#         super().__init__(*args, **kwargs)
        
#     def _tokenize(self, code):
#         code = generate_replaced(code)
#         tokens = ctok.tokenize(code, lang = "c", syntax_error = "ignore")

#         return tokens

def log(file_name, msg):
    with open(file_name, 'a') as f:
        f.write(f'{msg}\n')

def tokenize(code, replaced=False):
    code = preprocess.remove_comments(code)

    if len(code.strip()) == 0:
        return []
    # log('original.c', code)
    if replaced:
        code = parse_tools.generate_replaced(code)
    # log('replaced.c', code)
        
    tokens = ctok.tokenize(code, lang = "c", syntax_error = "ignore")

    updated_tokens = []
    for token in tokens:
        try:
            str_token = token.text
        except:
            str_token = 'TOKEN'

        if replaced and any([str_token.startswith(prefix) for prefix in parse_tools.replaced_prefixes.values()]):
            updated_tokens += list(str_token.split('_'))
        else:
                updated_tokens.append(str_token)

                                
    return updated_tokens


def tokenize_deepSCC(code):
    tokenizer = AutoTokenizer.from_pretrained('NTUYG/DeepSCC-RoBERTa')
    return tokenizer.tokenize(code)


code = """
#include <stdio.h>


int main() {
    int r[2800 + 1];
    int i, k;
    int b, d;
    int c = 0;


    for (i = 0; i < 2800; i++) {
        r[i] = 2000;
    }


    for (k = 2800; k > 0; k -= 14) {
        d = 0;


        i = k;
        for (;;) {
            d += r[i] * 10000;
            b = 2 * i - 1;


            r[i] = d % b;
            d /= b;
            i--;
            if (i == 0) break;
            d *= i;
        }
        printf("%.4d", c + d / 10000);
        c = d % 10000;
    }


    return 0;
}

"""

# tokenizer = CodeTokenizer()
# tokens = tokenizer.tokenize(code)
# print(tokens)

# print(tokenize(code))



# iterate over sub-HPCorpus
json_dir = '/home/talkad/OpenMPdb/tokenizer/HPCorpus'
occurrences = {}
total_tokens, amount_samples = 0, 0

for json_file in os.listdir(json_dir):
    with open(os.path.join(json_dir, json_file), 'r') as f:
        for idx, line in tqdm(enumerate(f)):
            if idx > 450:
                break

            js = json.loads(line.strip())

            if 'content' not in js:
                continue

            tokens = tokenize_deepSCC(js['content'])
            # tokens = tokenize(js['content'], replaced=False)

            total_tokens += len(tokens)
            amount_samples += 1

            for token in tokens:
                occurrences[token] = 1 if token not in occurrences else occurrences[token]+1

            # break

sorted_data = sorted(occurrences.items(), key=lambda x: x[1], reverse=True)
sorted_dict = {k: v for k, v in sorted_data}

print(f'AVG tokens per sample: {total_tokens/amount_samples}')

with open("deepscc_vocab.json", "w") as outfile:
    json.dump(sorted_dict, outfile, indent=4)


# 450 samples
# deepSCC - AVG tokens per sample: 14050.545454545454
# ctok -    AVG tokens per sample: 4609.314855875831
# ours -    AVG tokens per sample: 1949.7117516629712