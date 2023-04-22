import sys
sys.path.append("..")


import pickle

with open('/home/talkad/Desktop/pragformer/PragFormer/Classifier/omp_cls.pkl', 'rb') as f:
    print(f)
    data = pickle.load(f)
    print(data)
