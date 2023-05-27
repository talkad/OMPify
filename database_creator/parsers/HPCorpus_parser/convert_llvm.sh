#!/bin/bash

cd $1
ls
clang -S -Oz -emit-llvm code.c 