#!/bin/bash
#PBS -N train_model
#PBS -l nodes=1:ppn=1

source activate ompify_env2

cd /homes/talkad/OMPify/CompCoder


python ../../OMPify/CompCoder/main.py \
        --langs fortran \
        --do-pre-train \
        --pre-train-tasks mass \
        --n-epoch 5 \
        --batch-size 32 \
        --eval-batch-size 64 \
        --no-replaced \
        --no-ast \
        --no-dfg \
        --model-name pre_train_fortran_mass \
        --fp16
        # --cuda-visible-devices 0,1,2,3 \

conda deactivate