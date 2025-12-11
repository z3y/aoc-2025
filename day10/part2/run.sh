#!/bin/bash

mkdir -p build
g++ main.cpp -I z3/include -L z3/bin -lz3 -std=c++20 -o build/main

LD_LIBRARY_PATH=z3/bin ./build/main
