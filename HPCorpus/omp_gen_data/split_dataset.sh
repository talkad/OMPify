#!/bin/bash

input_file="/home/1010/talkad/Downloads/HPCorpus_final/fine_tune/cpp/dataset_cpp.jsonl"
output_prefix="batch"
batch_size=10000
line_count=0
batch_number=1

while IFS= read -r line; do
    echo "$line" >> "/home/1010/talkad/Downloads/HPCorpus_final/fine_tune/cpp/batches/${output_prefix}_${batch_number}.jsonl"
    ((line_count++))

    if [ "$line_count" -eq "$batch_size" ]; then
        ((batch_number++))
        line_count=0
    fi
done < "$input_file"