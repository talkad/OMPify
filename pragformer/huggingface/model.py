from transformers import AutoModel, AutoConfig
import torch.nn as nn
from transformers import BertPreTrainedModel, AutoModel, PreTrainedModel
from model_config import PragFormerConfig



class BERT_Arch(PreTrainedModel): #(BertPreTrainedModel):
    config_class = PragFormerConfig

    def __init__(self, config):
        super().__init__(config)
        print(config.bert)
        self.bert = AutoModel.from_pretrained(config.bert['_name_or_path'])

        # dropout layer
        self.dropout = nn.Dropout(config.dropout)

        # relu activation function
        self.relu = nn.ReLU()

        # dense layer 1
        self.fc1 = nn.Linear(self.config.bert['hidden_size'], config.fc1)
        # self.fc1 = nn.Linear(768, 512)

        # dense layer 2 (Output layer)
        self.fc2 = nn.Linear(config.fc1, config.fc2)

        # softmax activation function
        self.softmax = nn.LogSoftmax(dim = config.softmax_dim)

    # define the forward pass
    def forward(self, input_ids, attention_mask):
        # pass the inputs to the model
        _, cls_hs = self.bert(input_ids, attention_mask = attention_mask, return_dict=False)

        x = self.fc1(cls_hs)

        x = self.relu(x)

        x = self.dropout(x)
        
        # output layer
        x = self.fc2(x)

        # apply softmax activation
        x = self.softmax(x)
        return x
