#!/bin/bash
g++ -Xpreprocessor -march=native -fopenmp omp_matrix_multiplication.cpp -o omp_matrix_multiplication -std=c++17
./omp_matrix_multiplication.exe