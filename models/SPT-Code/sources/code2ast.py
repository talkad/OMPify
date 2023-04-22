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


def save(split_name, save_path, data):
    with open(os.path.join(save_path, split_name), 'wb') as f:
        pickle.dump(data, f)


splits = ['train', 'valid', 'test']


for split in splits:
    dataset =[]

    with open(f'/home/talkad/Desktop/pragformer_projects/a_code/data_creator/pickles_c_cpp/{split}.pkl', 'rb') as f:
        data = pickle.load(f)

    for code, _,label,_,_,_ in tqdm(data):
        xsbt =  code2xsbt(code, enums.LANG_C)
        
        dataset.append((code, xsbt, label))

    save(f'{split}.pkl', '.', dataset)


# total_samples = len(dataset)
# random.shuffle(dataset)

# save('train.pkl', '.', dataset[:int(total_samples*0.8)])
# save('valid.pkl', '.', dataset[int(total_samples*0.8):int(total_samples*0.9)])
# save('test.pkl', '.', dataset[int(total_samples*0.9):])


# print(dataset)




# lang = enums.LANG_CPP
# source = '''

# for(int i = 0; i < M; i++) {
#     SomeTYPE a;

#     if (a.isfunc()){
#         sum += i;
# }
# '''


# print(code2xsbt(source, lang))