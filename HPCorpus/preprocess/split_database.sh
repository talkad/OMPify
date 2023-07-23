#!/bin/bash

input_file="/tier2/bgu/HPCorpus_preprocess/cpp/total_uniq.jsonl"
output_file_1="/tier2/bgu/HPCorpus_final/pre_train/cpp/dataset_cpp.jsonl"
output_file_2="/tier2/bgu/HPCorpus_final/fine_tune/cpp/dataset_cpp.jsonl"

# Get the total number of lines in the input file
total_lines=$(wc -l < "$input_file")

echo "$total_lines"

# Calculate the number of lines for 80% and 20%
lines_80_percent=$((total_lines * 80 / 100))
lines_20_percent=$((total_lines - lines_80_percent))

# Split the input file into two output files based on line count
head -n "$lines_80_percent" "$input_file" > "$output_file_1"
tail -n "$lines_20_percent" "$input_file" > "$output_file_2"
