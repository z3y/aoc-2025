#!/bin/bash

mkdir -p build
g++ -std=c++20 -O2 main.cpp -o build/main
./build/main