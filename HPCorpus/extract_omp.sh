#!/bin/bash

for i in {1..514}; do
    echo $i
    python extract_omp_str.py $i
done