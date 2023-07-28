#!/bin/bash
#SBATCH --job-name=pretrain_fortran        # Job name (you can choose any name)
#SBATCH --partition=hpci-h100              # Specify the partition or queue name
#SBATCH --nodes=1                          # Number of nodes to request


source activate ompify_env2

cd /homes/talkad/OMPify/CompCoder

python main.py \
        --langs fortran \
        --do-pre-train \
        --pre-train-tasks mass \
        --batch-size 64 \
        --eval-batch-size 64 \
        --no-replaced \
        --no-ast \
        --no-dfg \
        --model-name pre_train_fortran_mass \
        --fp16
        
        # --fp16 \
        # --cuda-visible-devices 0,1,2,3 \

conda deactivate

