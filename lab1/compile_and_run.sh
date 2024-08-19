#!/bin/bash

input="SinCalculation.cpp"

optimization_levels=("-O0" "-O1" "-O2" "-O3" "-Os")
n=1080000000
x=90

echo "Optimixation level, N value, X value, Time taken (seconds)" > report.csv

for i in "${optimization_levels[@]}"; do
    g++ -o sin $input $i -std=c++11
    output=$(./sin $x $n | grep "Average time:" | awk '{print $3}')
    echo "$i, $n, $x, $output" >> report.csv
done

echo "Successfully generated report.csv"