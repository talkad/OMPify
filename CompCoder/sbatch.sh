#!/bin/bash
#SBATCH --job-name=pretrain_fortran        # Job name (you can choose any name)
#SBATCH --partition=hpci-h100              # Specify the partition or queue name
#SBATCH --nodes=1                          # Number of nodes to request


source activate ompify_env

cd /home/1010/talkad/OMPify/CompCoder

python main.py \
        --langs fortran \
        --do-pre-train \
        --pre-train-tasks mass \
        --n-epoch 5\
        --batch-size 32 \
        --eval-batch-size 64 \
        --no-ast \
        --no-dfg \
        --model-name pre_train_fortran_tokom_mass \
        --fp16
        
        # --no-replaced \

conda deactivate


