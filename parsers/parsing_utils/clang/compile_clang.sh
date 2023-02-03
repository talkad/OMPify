#!/bin/bash

clang -S -emit-llvm -g $1 && ../../build/tools/llvm-cbe/llvm-cbe $2