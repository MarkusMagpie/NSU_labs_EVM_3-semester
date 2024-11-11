#!/bin/bash

g++ -O1 main.cpp -o main.out
time(./main.out > log.txt)

gnuplot <<EOF
set terminal png
set output 'forward_plot.png'
plot 'forward_result.txt' with lines linecolor rgb "blue" title 'Forward access'
set output 'reverse_plot.png'
plot 'reverse_result.txt' with lines linecolor rgb "blue" title 'Reverse access'
EOF

echo "Forward plot saved as forward_plot.png"
echo "Reverse plot saved as reverse_plot.png"
