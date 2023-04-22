#!/bin/bash

mkdir saved_models
python run.py \
    --output_dir=saved_models \
    --config_name=microsoft/graphcodebert-base \
    --model_name_or_path=microsoft/graphcodebert-base \
    --tokenizer_name=microsoft/graphcodebert-base \
    --do_train \
    --train_data_file=/home/talkad/LIGHTBITS_SHARE/pragformer_projects/a_code/data_creator/json_c_cpp/train.jsonl \
    --eval_data_file=/home/talkad/LIGHTBITS_SHARE/pragformer_projects/a_code/data_creator/json_c_cpp/valid.jsonl \
    --test_data_file=/home/talkad/LIGHTBITS_SHARE/pragformer_projects/a_code/data_creator/json_c_cpp/test.jsonl \
    --epoch 8 \
    --code_length 512 \
    --data_flow_length 128 \
    --train_batch_size 16 \
    --eval_batch_size 32 \
    --learning_rate 2e-5 \
    --max_grad_norm 1.0 \
    --evaluate_during_training \
    --seed 123456 2>&1| tee train_c_cpp_clean_reduction.log