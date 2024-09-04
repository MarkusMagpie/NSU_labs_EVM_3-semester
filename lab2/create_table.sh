#!/bin/bash

if [ ! -f report.csv ]; then
    echo "Файл report.csv не найден!"
    exit 1
fi

# заголовок таблицы
echo -e "Оптимизация\t N\t\t Время выполнения (сек)"

prev=""

# -F',': запятая в качестве разделителя столбцов
# NR>1: пропускаем первую строку
awk -F',' 'NR>1 { 
    if (prev != $1 && NR > 2) {
        print ""; # перевод на новую строку
    }

    printf "%s\t\t%s\t%s\n", $1, $2, $3;
    prev = $1;
}' report.csv
