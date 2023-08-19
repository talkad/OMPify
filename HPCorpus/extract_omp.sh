#!/bin/bash

for i in {0..5}; do
    echo $i
    python extract_omp.py $i
done