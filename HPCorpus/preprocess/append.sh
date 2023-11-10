#!/bin/bash

source_file="/tier2/bgu/HPCorpus_final/pre_train/cpp/dataset_cpp.jsonl"
destination_file="/tier2/bgu/temp/cpp/dataset_cpp.jsonl"

total_lines=$(wc -l < "$source_file")

lines_to_copy=$((total_lines / 5)) 

tail -n "$lines_to_copy" "$source_file" >> "$destination_file"