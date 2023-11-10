#!/bin/bash

# Set the directory path
directory="/tier2/bgu/temp/cpp/batches"

# Get a list of files in the directory and sort them
files=($(ls "$directory" | sort))

# Specify the range of files you want, e.g., from file 3 to 6
start_index=3000
end_index=3499
# in c there are 2600 batches
# in cpp there are 3400 batches
echo "$start_index" - "$end_index"

# Slice the array to get the range of files
selected_files=("${files[@]:$start_index:$((end_index - start_index + 1))}")

for file in "${selected_files[@]}"; do
    echo $file
    python extract_omp3.py $file
done



