#!/bin/bash

# echo clang -S -emit-llvm -g $1 
# echo /home/talkad/llvm-cbe/build/tools/llvm-cbe/llvm-cbe $2

cd $1
clang -S -emit-llvm -g $2 && /home/talkad/llvm-cbe/build/tools/llvm-cbe/llvm-cbe $3