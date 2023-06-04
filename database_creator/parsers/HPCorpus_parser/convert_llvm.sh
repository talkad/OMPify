#!/bin/bash

cd $1
ls
# clang -S -Oz -emit-llvm code.c 
clang -c -emit-llvm -S -g1 -Oz -O0 code.c -std=c17 -Xclang -disable-O0-optnone -Wno-narrowing