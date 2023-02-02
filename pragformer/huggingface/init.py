from transformers import AutoModel, AutoConfig
from transformers import BertPreTrainedModel, AutoModel, PretrainedConfig
from model import BERT_Arch
from model_config import PragFormerConfig

if __name__ == "__main__":



    # PragFormerConfig.register_for_auto_class()
    # BERT_Arch.register_for_auto_class("AutoModel")
    # config = PragFormerConfig.from_pretrained('../PragFormer2')
    # model = BERT_Arch(config)
    # model.push_to_hub("PragFormer")

    # config = AutoConfig.from_pretrained('NTUYG/DeepSCC-RoBERTa')
    # config = PragFormerConfig(vars(config))
    # config.save_pretrained("../PragFormer2")

    config = PragFormerConfig.from_pretrained('../PragFormer')
    model = BERT_Arch(config)
    pretrained_model = BERT_Arch.from_pretrained("../PragFormer")
    model.load_state_dict(pretrained_model.state_dict())
    model.push_to_hub("PragFormer")


    # AutoConfig.register("pragformer", PragFormerConfig)
    # AutoModel.register(PragFormerConfig, BERT_Arch)
    # config = PragFormerConfig.from_pretrained('../PragFormer2')
    # model = BERT_Arch(config)
    # # pretrained_model = BERT_Arch.from_pretrained("../PragFormer")
    # # model.load_state_dict(pretrained_model.state_dict())
    # model.push_to_hub("PragFormer")