#!/bin/bash
g++ -Xpreprocessor -march=native -fopenmp vector_add.cpp -o vector_add -std=c++17
./vector_add.exe