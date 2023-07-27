#!/bin/bash

# Input CSV file path
input_file="/tier2/bgu/hash/cpp/total.csv"

# Output file path (file without hash duplications)
output_file="total_uniq_cpp.csv"

# Check if the input file exists
if [ ! -f "$input_file" ]; then
  echo "Input file not found: $input_file"
  exit 1
fi

# Use sort and uniq to remove duplicates based on the hash field
sort -t',' -k3,3 -u "$input_file" > "$output_file"

echo "Duplicate hash lines removed. Output written to: $output_file"