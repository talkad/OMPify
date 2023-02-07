#!/bin/bash

# echo clang -S -emit-llvm -g $1 
# echo /home/talkad/llvm-cbe/build/tools/llvm-cbe/llvm-cbe $2

cd $1
echo "clang -S -emit-llvm $2  -g $3 && /home/talkad/llvm-cbe/build/tools/llvm-cbe/llvm-cbe $4"
clang -S -emit-llvm $2  -g $3 && /home/talkad/llvm-cbe/build/tools/llvm-cbe/llvm-cbe $4