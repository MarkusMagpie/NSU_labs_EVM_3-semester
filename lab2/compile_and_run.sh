#!/bin/bash

input="PiCalculation.cpp"

optimization_keys=("-O0" "-O1" "-O2" "-O3" "-Os" "-Ofast" "-Og")
n=("450000000" "600000000" "700000000")

echo "Optimization level, N value, Time taken (seconds)" > report.csv

for i in "${optimization_keys[@]}"; do
    for j in "${n[@]}"; do
        g++ $input $i -std=c++11
        output=$(./a.out $j)

        # каждая итерация
        echo "$output" | grep "Run time:" | awk -v opt="$i" -v N="$j" '{print opt ", " N ", " $3}' >> report.csv

        # среднее время
        avg_time=$(echo "$output" | grep "Average time:" | awk '{print $3}')
        echo "$i, $j, $avg_time" >> report.csv
    done
done

echo "Successfully generated report.csv"

./create_table.sh
