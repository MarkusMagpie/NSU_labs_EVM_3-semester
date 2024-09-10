#!/bin/bash

input="PiCalculation.cpp"

optimization_levels=("-O0" "-O1" "-O2" "-O3" "-Os")
n=650000000

echo "Optimixation level, N value, Time taken (seconds)" > report.csv

for i in "${optimization_levels[@]}"; do
    g++ -o pi $input $i -std=c++11
    # output=$(./pi $n | grep "Average time:" | awk '{print $3}')
    output=$(./pi $n)

    # время выполнения каждой итерации
    # https://www.opennet.ru/man.shtml?topic=awk&category=1&russian=2
    echo "$output" | grep "Run time:" | awk -v opt="$i" -v N="$n" '{print opt ", " N ", " $3}' >> report.csv

    # среднее время
    avg_time=$(echo "$output" | grep "Average time:" | awk '{print $3}')
    echo "$i, $n, $avg_time" >> report.csv
done

echo "Successfully generated report.csv"

./create_table.sh