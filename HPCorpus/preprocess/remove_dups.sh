#!/bin/bash

# Input CSV file path
input_file="/home/1010/talkad/Downloads/OMP_Dataset/c/source/hash/c/total.csv"

# Output file path (file without hash duplications)
output_file="total_uniq_c.csv"

# Check if the input file exists
if [ ! -f "$input_file" ]; then
  echo "Input file not found: $input_file"
  exit 1
fi

# Use sort and uniq to remove duplicates based on the hash field
sort -t',' -k3,3 -u "$input_file" > "$output_file"

echo "Duplicate hash lines removed. Output written to: $output_file"