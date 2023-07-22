#!/bin/bash
#PBS -N train_model
#PBS -l nodes=1:ppn=1

source activate ompify_env2

cd /homes/talkad/OMPify/CompCoder


python main.py \
        --do-pre-train \
        --pre-train-tasks mass \
        --batch-size 64 \
        --eval-batch-size 64 \
        --fp16 \
        --model-name pre_train_fortran_mass
        # --cuda-visible-devices 0,1,2,3 \

conda deactivate