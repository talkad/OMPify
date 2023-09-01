#!/bin/bash

for i in {1..675}; do
    echo $i
    python extract_omp5.py $i
done