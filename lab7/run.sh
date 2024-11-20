#!/bin/bash

echo "Running NO VECTORISATION version"
g++ -o no_vector main_no_vectorisation.cpp || exit 1
for N in 256 512 1024; do
    ./no_vector $N 10
done
echo "--------------------------------------"

echo "Running VECTORISATION version"
g++ -o vector main_vectorisation.cpp || exit 1
# clang -O3 --std=gnu++23 -Rpass=loop-vectorize -Rpass-missed=loop-vectorize -Rpass-analysis=loop-vectorize -o vector main_vectorisation.cpp -msse2
for N in 256 512 1024 2048; do
    ./vector $N 10
done
echo "--------------------------------------"

echo "Running BLAS version"
g++ -o blas main_blas.cpp -lblas || exit 1
for N in 256 512 1024 2048; do
    ./blas $N 10
done
