#!/bin/bash

input_file="/home/1010/talkad/Downloads/Fortran_HPCorpus/total_uniq.jsonl"
output_file_1="/home/1010/talkad/Downloads/HPCorpus_final/pre_train/fortran/dataset_fortran.jsonl"
output_file_2="/home/1010/talkad/Downloads/HPCorpus_final/fine_tune/fortran/dataset_fortran.jsonl"

# Get the total number of lines in the input file
total_lines=$(wc -l < "$input_file")


echo "$total_lines"

# Calculate the number of lines for 80% and 20%
lines_80_percent=$((total_lines * 80 / 100))
lines_20_percent=$((total_lines - lines_80_percent))

# Split the input file into two output files based on line count
head -n "$lines_80_percent" "$input_file" > "$output_file_1"
tail -n "$lines_20_percent" "$input_file" > "$output_file_2"
