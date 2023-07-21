from convert_representation import generate_replaced, code2xsbt, code2dfg, code2ast
import sys

sys.path.extend(['.','/home/talkad/OpenMPdb/tokenizer'])
from tokenizer import Tokompiler, TokenizerBPE, ASTokenizer, DFGTokenizer


code = '''
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
'''
lang = 'c'

tokompiler = Tokompiler()
bpe = TokenizerBPE()
astTokenizer = ASTokenizer()
dfgTokenizer = DFGTokenizer()


# code = generate_replaced(code)
# ast = code2ast(code)
# dfg = code2dfg(code)
# with open('usage_output.txt', 'w') as f:
#     f.write('code\n')
#     f.write(code)
#     f.write('ast\n')
#     f.write(ast)
#     f.write('dfg\n')
#     print(dfg)


# print(bpe.tokenize(code, lang=lang))
# print(astTokenizer.tokenize(code, lang=lang))
# print(dfgTokenizer.tokenize(code, lang=lang))

# replaced
code = generate_replaced(code, lang=lang)
print(tokompiler.tokenize(code, replaced=True, lang=lang))
print(astTokenizer.tokenize(code, replaced=True, lang=lang))
print(dfgTokenizer.tokenize(code, replaced=True, lang=lang))


