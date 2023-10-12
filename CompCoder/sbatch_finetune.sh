#!/bin/bash
#SBATCH --job-name=pretrain_fortran        # Job name (you can choose any name)
#SBATCH --partition=hpci-h100              # Specify the partition or queue name


source activate ompify_env

cd /home/1010/talkad/OMPify/CompCoder

 python main.py     \
        --langs fortran      \
        --do-fine-tune  \
        --task pragma_generation \
        --pragma_dataset_path /home/1010/talkad/Downloads/OMP_Dataset/fortran/total_uniq.jsonl \
        --trained-model /home/1010/talkad/OMPify/outputs/wed_pre_train_fortran_mass_20230802_235207 \
        --trained-vocab /home/1010/talkad/Downloads/HPCorpus_final/vocab_saved  \
        --n-epoch 5      \
        --batch-size 64   \
        --fp16      \
        --eval-batch-size 64    \
        --no-dfg     \
        --no-ast      \
        --model-name tokom_pragma

conda deactivate


python main.py         --langs fortran   --do-fine-tune      --task pragma_generation        --n-epoch 10        --batch-size 8         --fp16         --eval-batch-size 4         --no-dfg  --no-ast  --model-name fortran_tokom_mass

