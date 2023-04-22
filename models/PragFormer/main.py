import sys
sys.path.append("..")
# python main.py --multiple_gpu --train 2>&1 | tee train_c_cpp_clean.log
import torch
import pandas as pd
import torch.nn as nn
from transformers import AutoModel, AutoModelForSequenceClassification, AutoConfig
from transformers import AdamW
from torch.utils.data import TensorDataset, DataLoader, RandomSampler, SequentialSampler
from sklearn.utils.class_weight import compute_class_weight
import PragFormer.train as trainer
from PragFormer.predict import predict
from PragFormer.model import  BERT_Arch
import argparse
# from PreProcessing.data_creator import *
# from PragFormer.huggingface_model import PragFormerHuggingFace
from PragFormer.pragformer_config import PragFormerConfig
# from Utils.tokenizer import *
from transformers import AutoTokenizer, AutoModelForMaskedLM, AutoModel
import numpy as np
import json
import os
import pickle
import PragFormer.global_parameters as gp
from sklearn.model_selection import train_test_split
import random
from tqdm import tqdm



def deepscc_tokenizer(data, max_len=150, pt_model="/home/talkad/Desktop/pragformer_projects/pragformer/PragFormer/DeepSCC-RoBERTa"):
    # model_pretained_name = "NTUYG/DeepSCC-RoBERTa"  # 'bert-base-uncased'
    # model_pretained_name = 'bert-base-uncased'
    tokenizer = AutoTokenizer.from_pretrained(pt_model)
    if max_len == 0:
        tokenized = tokenizer.batch_encode_plus(
            data,
            # max_length = max_len,
            pad_to_max_length=True,
            truncation=True
        )
    else:
        tokenized = tokenizer.batch_encode_plus(
            data,
            max_length = max_len,
            pad_to_max_length = True,
            truncation = True
        )

    return tokenized, tokenizer.vocab_size


def load_data(label):

    data_path =  '/home/talkad/Downloads/thesis/data_gathering_script/models/dataset/json_c_cpp'
    splits = ['train', 'valid', 'test']    
    df = {}

    for split in splits:
        df[split] = []
        print(split)
        with open(os.path.join(data_path, f'{split}.jsonl')) as f:
            for line in tqdm(f):
                js = json.loads(line.strip())
                df[split].append((js['code'], js[label]))

    data = gp.Data()
    
    data.train, data.train_labels= [code for code, _ in df['train']], [label for _, label in df['train']]
    data.val, data.val_labels= [code for code, _ in df['valid']], [label for _, label in df['valid']]
    data.test, data.test_labels= [code for code, _ in df['test_spec']], [label for _, label in df['test_spec']]

    print(f'size train {len(data.train_labels)} val {len(data.val_labels)} test {len(data.test_labels)}')

    return data




if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--config_file', default=None, type=str,
                        dest='config_file', help='The file of the hyper parameters.')
    parser.add_argument('--train', default=False, action="store_true",
                        dest='train', help='Train phase.')
    parser.add_argument('--predict', default=False, action="store_true",
                        dest='predict', help='Predict phase.')
    parser.add_argument('--huggingface', default=False, action="store_true",
                        dest='hf', help='Predict phase.')
    # parser.add_argument('--save', default="", type = str,
    #                     dest='save', help='Save tokenize phase.')
    parser.add_argument('--multiple_gpu', default=False, action = "store_true",
                        dest='multiple_gpu', help='Number of gpus')
    parser.add_argument('--out', default="saved_weights.pt", type=str,
                        dest='out', help='Saved model name.')

    # ***********  Params for data.  **********
    parser.add_argument('--data_dir', type=str,
                        dest='data_dir', help='The Directory of the data.')
    parser.add_argument('--max_len', default=0, type=int,
                        dest='max_len', help='The type of read.')
    parser.add_argument('--epochs', default=8, type=int,
                        dest='epochs', help='The type of read.')
    parser.add_argument('--specific_directive', default="reduction", type=str,
                        dest='max_len', help='The type of read.')
    parser.add_argument('--reshuffle', dest='reshuffle',action = "store_true", default=False)

    args = parser.parse_args()

    # CREATE CONFIG DIC
    config = {}
    # config["data_type"] = args.data_type
    config["data_dir"] = args.data_dir
    epochs = args.epochs

    max_len = args.max_len
    print('load data')
    data = load_data('exist')

    torch.cuda.empty_cache()
    device = torch.device("cuda")

    # Define model
    model_pretained_name = "NTUYG/DeepSCC-RoBERTa" # "/home/talkad/Desktop/pragformer/PragFormer/DeepSCC-RoBERTa" #"./PragFormer"
    # pragformer_config = AutoConfig.from_pretrained(model_pretained_name)
    # model = AutoModel.from_pretrained(model_pretained_name)

    # # tokenizer1 = AutoTokenizer.from_pretrained(model_pretained_name)
    # # tokenizer1.push_to_hub("PragFormer")
    

    # model = AutoModel.from_pretrained("Pragformer/PragFormer", trust_remote_code=True)
    print('loading model')
    config = PragFormerConfig.from_pretrained('./PragFormer')
    model = BERT_Arch(config)
    # pretrained_model = BERT_Arch.from_pretrained("./PragFormer_clang")
    # model.load_state_dict(pretrained_model.state_dict())


    batch_size = 32
    # Convert lists to tensors..
    if args.train:
        print("Example of data: \n", data.train[126])

        train, _ = deepscc_tokenizer(data.train, args.max_len, model_pretained_name)
        val, _ = deepscc_tokenizer(data.val, args.max_len, model_pretained_name)
        train_seq = torch.tensor(train['input_ids'])
        train_mask = torch.tensor(train['attention_mask'])
        train_y = torch.tensor(data.train_labels)
        # wrap tensors
        train_data = TensorDataset(train_seq, train_mask, train_y)
        # sampler for sampling the data during training
        train_sampler = RandomSampler(train_data)
        # dataLoader for train set
        train_dataloader = DataLoader(train_data, sampler = train_sampler, batch_size = batch_size)

        val_seq = torch.tensor(val['input_ids'])
        val_mask = torch.tensor(val['attention_mask'])
        val_y = torch.tensor(data.val_labels)
        # wrap tensors
        val_data = TensorDataset(val_seq, val_mask, val_y)
        # sampler for sampling the data during training
        val_sampler = SequentialSampler(val_data)
        # dataLoader for validation set
        val_dataloader = DataLoader(val_data, sampler = val_sampler, batch_size = batch_size)


    if args.predict:
        print("Example of data:\n", data.test[1])

        data_test = data.test
        label_test = data.test_labels

        test, _ = deepscc_tokenizer(data_test, args.max_len, model_pretained_name)

        maxx = len(test['input_ids'])
        test_seq = torch.tensor(test['input_ids'])
        test_mask = torch.tensor(test['attention_mask'])
        test_y = torch.tensor(label_test)
        test_data = TensorDataset(test_seq, test_mask, test_y)
        test_sampler = SequentialSampler(test_seq)
        test_dataloader = DataLoader(test_data, sampler = test_sampler, batch_size = batch_size)
        test_to_show = {'label': [], 'id': [], 'input_ids': []}
        test_to_show['label'].extend(label_test)
        test_to_show['id'].extend(data.test_ids)
        test_to_show['input_ids'].extend(test['input_ids'])

    # # freeze all the parameters - I.E DO NOT UPDATE PRE-TRAINED MODEL FROM NEW TRAINING
    # for param in pt_model.parameters():
    #     param.requires_grad = False
    #     print (param.requires_grad)

#     # pass the pre-trained BERT to our define architecture
#     # PragFormerConfig.register_for_auto_class()
#     # if not args.hf:
#         # config = AutoConfig.from_pretrained("NTUYG/DeepSCC-RoBERTa")
#         # config.save_pretrained("./PragFormer")
#         #PragFormerHuggingFace.
#         # model = PragFormerHuggingFace()
#         # model.from_pretrained("./PragFormer")

#     # elsekodel = pt_model

#     # push the model to GPU
#     # if args.predict:
#     #     # model.load_state_dict(torch.load(args.out))
#     #     print("@@@@@@@@@@@@@@@ We print load_state @@@@@@@@@@@@@@@ ")
#     #     model = PragFormerHuggingFace.from_pretrained("./PragFormer")
#     #     # model = AutoModel.from_pretrained("./PragFormer", config=config)
#     #     print(model)
#         # print(model.model.module.fc2.weight)
#         # model = PragFormerHuggingFace.from_pretrained("./PragFormer", config=config)
#         # print(model)
#     # Now inside the model


#     print("Load from dict!")
#     # if args.predict:
#     #     model2 = PragFormerHuggingFace(config)
#     #     model2.load_state_dict(torch.load("./PragFormer/pytorch_model.bin"))
#     #     a= model2.fc1.weight
#     #     print(torch.equal(model2.fc1.weight, model.fc1.weight))
#     #     print(model2.fc2.weight)

#     #     model2.load_state_dict(torch.load("saved_weights.pt"))
#     #     print(torch.equal(model2.fc1.weight, model.fc1.weight))
#     #     print(model2.fc2.weight)
#     #     # print(model.fc2.weight)
#     #     print(f'aaaaaaaaaaaaaaaaaaaa {torch.equal(model2.fc1.weight, a)}')

    if args.multiple_gpu:
        model = nn.DataParallel(model)

    model = model.to(device)

    # define the optimizer
    optimizer = AdamW(model.parameters(), lr = 1e-5) # train all parameters
    # optimizer = AdamW([
    #                     {'params': model.module.fc1.parameters()},
    #                     {'params': model.module.fc2.parameters()}
    #                     ], lr = 1e-5)

    # There is a class imbalance in our dataset. The majority of the observations are not spam.
    # So, we will first compute class weights for the labels in the train set
    # and then pass these weights to the loss function so that it takes care of the class imbalance.
    if args.train:
        class_weights = compute_class_weight('balanced', np.unique(data.train_labels), data.train_labels)
        # print("Class Weights:", class_weights)
        # converting list of class weights to a tensor
        weights = torch.tensor(class_weights, dtype=torch.float)
        # push to GPU
        weights = weights.to(device)
        # define the loss function
        cross_entropy = nn.NLLLoss(weight=weights)

    if args.train:
        trainer.train(model, epochs, train_dataloader, device, cross_entropy, optimizer, val_dataloader, args.out)

    # for each epoch
    if args.predict:
        # tokenizer1.push_to_hub(repo_url="https://huggingface.co/reemha/PragFormer")
        # tokenizer1.push_to_hub("PragFormer")
        # tokenizer1.save_pretrained('/home/reemh/CLPP/CLPP/Pretrain/', repo_name='PragFormer')
        predict(model, device, test_dataloader, test_y, args.out, test_to_show)

# load weights of best model
    # path = 'saved_weights.pt'
    # model.load_state_dict(torch.load(path))
    #
    # # get predictions for test data
    # with torch.no_grad():
    #   preds = model(test_seq.to(device), test_mask.to(device))
    #   preds = preds.detach().cpu().numpy()
    #
    # preds = np.argmax(preds, axis = 1)
    # print(classification_report(test_y, preds))
    # function to train the model

