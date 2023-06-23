#!/bin/bash

clang++ -c -emit-llvm -S -g1 -Oz code.cpp -o code.ll -std=c++17 -Xclang -disable-O0-optnone -Wno-narrowing