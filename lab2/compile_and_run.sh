#!/bin/bash

input="PiCalculation.cpp"

optimization_keys=("-O0" "-O1" "-O2" "-O3" "-Os" "-Ofast" "-Og")
n="750000000"

echo "Optimization level, N value, Time taken (seconds)" > report.csv

for i in "${optimization_keys[@]}"; do
    g++ $input $i -std=c++11
    output=$(./a.out $n)

    # минимальное время, не среднее!
    echo "$output" | grep "Min time:" | awk -v opt="$i" -v N="$n" '{print opt ", " N ", " $3}' >> report.csv
done

echo "Successfully generated report.csv"

./create_table.sh
