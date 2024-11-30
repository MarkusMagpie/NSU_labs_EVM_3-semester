#!/bin/bash

g++ -O1 main.cpp
time(./a.out)

gnuplot <<EOF
set terminal pngcairo size 800,600

set output "plot.png"
set title "Время чтения данных в зависимости от количества фрагментов"
set xlabel "Число фрагментов"
set ylabel "Время чтения элементов (такты)"
plot "l1.txt" using 1:2 with linespoints title "Время чтения"
EOF

echo "График построен!"