import code_tokenize as ctok
import sys
from transformers import PreTrainedTokenizerFast


sys.path.extend(['.','/home/talkad/OpenMPdb/database_creator/parsers/HPCorpus_parser'])
import parse_tools


# class CodeTokenizer(PreTrainedTokenizerFast):
#     def __init__(self, *args, **kwargs):
#         super().__init__(*args, **kwargs)
        
#     def _tokenize(self, code):
#         code = generate_replaced(code)
#         tokens = ctok.tokenize(code, lang = "c", syntax_error = "ignore")

#         return tokens


def tokenize(code):
        code = parse_tools.generate_replaced(code)
        tokens = ctok.tokenize(code, lang = "c", syntax_error = "ignore")

        updated_tokens = []
        for token in tokens:
            str_token = token.text

            if any([str_token.startswith(prefix) for prefix in parse_tools.replaced_prefixes.values()]):
                updated_tokens += list(str_token.split('_'))
            else:
                 updated_tokens.append(str_token)
                                  
        return updated_tokens


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

print(tokenize(code))