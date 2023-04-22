from transformers import AutoModel

model = AutoModel.from_pretrained("Pragformer/PragFormer_private", trust_remote_code=True)


