#!/bin/bash
#SBATCH --job-name=pretrain_fortran        # Job name (you can choose any name)
#SBATCH --partition=hpci-h100              # Specify the partition or queue name


source activate ompify_env

cd /home/1010/talkad/OMPify/CompCoder

# python main.py \
#         --langs fortran \
#         --do-pre-train \
#         --pre-train-tasks cap,mass \
#         --n-epoch 5\
#         --batch-size 64 \
#         --fp16 \
#         --eval-batch-size 64 \
#         --no-dfg \
#         --model-name wed_pre_train_fortran_tokom_cap_mass \
#         --no-replaced
        # --no-ast \

 python main.py     \
        --langs c      \
        --do-pre-train  \
        --pre-train-tasks mass   \
        --n-epoch 1      \
        --batch-size 64   \
        --fp16      \
        --eval-batch-size 64    \
        --no-dfg     \
        --no-ast      \
        --model-name c_mass_tokom

conda deactivate




# python main.py \
#         --langs c \
#         --do-pre-train \
#         --pre-train-tasks mass \
#         --n-epoch 1\
#         --batch-size 64 \
#         --fp16 \
#         --eval-batch-size 64 \
#         --no-dfg \
#         --no-ast \
#         --model-name c_mass_tokom
