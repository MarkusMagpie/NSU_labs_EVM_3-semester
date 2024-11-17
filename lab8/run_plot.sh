#!/bin/bash

g++ -O1 main2.cpp -o main2.out
time(./main2.out > random_l2_2.txt)

gnuplot <<EOF
set terminal png
set output 'random_l2_2.png'

set arrow from 48, graph 0 to 48, graph 1 nohead lw 2 lc rgb "blue"
set arrow from 1280, graph 0 to 1280, graph 1 nohead lw 2 lc rgb "blue" 

plot 'random_l2_2.txt' with lines linecolor rgb "red" title 'Random access'
EOF

echo "Plot saved as random_l2_2.png"