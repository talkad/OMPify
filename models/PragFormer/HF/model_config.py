from transformers import PretrainedConfig


class PragFormerConfig(PretrainedConfig):
    model_type = "pragformer_reduction"

    def __init__(self, bert=None, dropout=0.2, fc1=512, fc2=2, softmax_dim=1, **kwargs):
        self.bert = bert
        self.dropout = dropout
        self.fc1 = fc1
        self.fc2 = fc2
        self.softmax_dim = softmax_dim
        super().__init__(**kwargs)
