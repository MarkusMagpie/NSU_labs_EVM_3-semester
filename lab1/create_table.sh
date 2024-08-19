#!/bin/bash

if [ ! -f report.csv ]; then
    echo "Файл report.csv не найден!"
    exit 1
fi

# Вывод заголовка таблицы
echo -e "Оптимизация\t N\t\t X\t Время выполнения (сек)"

# -F',': запятая в качестве разделителя столбцов
# NR>1: пропускаем первую строку
awk -F',' 'NR>1 { printf "%s\t\t%s\t%s\t%s\n", $1, $2, $3, $4 }' report.csv
