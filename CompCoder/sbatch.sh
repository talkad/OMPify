#!/bin/bash
#SBATCH --job-name=pretrain_fortran        # Job name (you can choose any name)
#SBATCH --partition=hpci-h100              # Specify the partition or queue name


source activate ompify_env

cd /home/1010/talkad/OMPify/CompCoder

python main.py \
        --langs fortran \
        --do-pre-train \
        --pre-train-tasks cap,mass \
        --n-epoch 5\
        --batch-size 64 \
        --fp16 \
        --eval-batch-size 64 \
        --no-dfg \
        --model-name wed_pre_train_fortran_tokom_cap_mass \
        --no-replaced

        # --no-ast \

conda deactivate


