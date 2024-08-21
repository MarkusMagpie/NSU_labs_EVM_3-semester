#!/bin/bash

input="ExponentCalculation.cpp"

optimization_keys=("-O0" "-O1" "-O2" "-O3" "-Os" "-Ofast" "-Og")
n=("4000000000" "4500000000" "5000000000")
x=90

echo "Optimixation level, N value, Time taken (seconds)" > report.csv

for i in "${optimization_keys[@]}"; do
    for j in "${n[@]}"; do
        g++ $input $i -std=c++11
        output=$(./a.out $x $j | grep "Average time:" | awk '{print $3}')
        echo "$i, $j, $output" >> report.csv
    done
done

echo "Successfully generated report.csv"