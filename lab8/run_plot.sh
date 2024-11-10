#!/bin/bash

g++ -O1 main2.cpp -o main2.out
time(./main2.out > log.txt)

gnuplot <<EOF
set terminal png
set output 'plot.png'
plot 'log.txt' with lines linecolor rgb "red" title 'Random access'
set output
EOF

echo "Plot saved as plot.png"
