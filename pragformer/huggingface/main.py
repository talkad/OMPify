from transformers import AutoModel

model = AutoModel.from_pretrained("Pragformer/PragFormer", pt=True, trust_remote_code=True)

