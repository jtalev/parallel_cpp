#!/bin/bash

./sequential_matrix_multiplication
./stdthread_matrix_multiplication
./omp_matrix_multiplication

sed -ie '/cores/s/2/4/g' config.txt

./sequential_matrix_multiplication
./stdthread_matrix_multiplication
./omp_matrix_multiplication

sed -ie '/cores/s/4/6/g' config.txt

./sequential_matrix_multiplication
./stdthread_matrix_multiplication
./omp_matrix_multiplication

sed -ie '/cores/s/6/8/g' config.txt

./sequential_matrix_multiplication
./stdthread_matrix_multiplication
./omp_matrix_multiplication

sed -ie '/cores/s/8/10/g' config.txt

./sequential_matrix_multiplication
./stdthread_matrix_multiplication
./omp_matrix_multiplication

sed -ie '/cores/s/10/12/g' config.txt

./sequential_matrix_multiplication
./stdthread_matrix_multiplication
./omp_matrix_multiplication

sed -ie '/cores/s/12/44/g' config.txt

./sequential_matrix_multiplication
./stdthread_matrix_multiplication
./omp_matrix_multiplication

sed -ie '/cores/s/14/16/g' config.txt