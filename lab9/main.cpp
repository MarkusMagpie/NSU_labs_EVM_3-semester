#include <iostream>
#include <fstream>
#include <climits>
#include <x86intrin.h>
#include <cstdlib>

#define L1 49152     // 48 КБ
#define L2 1310720   // 1.25 МБ = 1280 КБ
#define L3 25165824  // 24 МБ

#define RUN_TIMES 4  // количество прогонов для замера

int N = 200000000;  // размер массива

// Функция инициализации массива с фрагментированным отступом
void InitArray(unsigned int *array, unsigned int fragment_count, size_t offset, size_t size){
    int i, j;

    for(i = 0; i < size; i++) {
        // std::cout << "i = " << i << std::endl;
        for(j = 1; j < fragment_count; j++) {
            array[i + (j - 1) * offset] = i + j * offset;
            // std::cout << "array[" << i + (j - 1) * offset << "] = " << i + j * offset << std::endl;
        }
        array[i + (j - 1) * offset] = i + 1;
        // std::cout << "array[" << i + (j - 1) * offset << "] = " << i + 1 << std::endl;
    }

    array[i - 1 + (j - 1) * offset] = 0;
}

// Функция для замера времени обхода массива
unsigned long long RunArray(unsigned int *array) {
    unsigned long long start, end;
    unsigned long long min_time = ULLONG_MAX;

    for(size_t j = 0; j < RUN_TIMES; j++) {
        start = __rdtsc();  // начало замера времени

        // Обход массива
        for(volatile size_t k = 0, i = 0; i < N; i++) {
            k = array[k];
        }

        end = __rdtsc();  // окончание замера времени
        min_time = std::min(min_time, end - start);
    }

    return min_time;
}

// подсчет времени для различных фрагментов
void CountTime(unsigned int *array, unsigned int fragment_count, int offset, int size, std::ofstream& file) {
    InitArray(array, fragment_count, offset, size);
    unsigned long long time = RunArray(array) / N;

    file << fragment_count << " " << time << std::endl;
}

int main() {
    // память для массива
    auto *array = new unsigned int[N];

    if (array == nullptr) {
        std::cerr << "Ошибка: не удалось выделить память" << std::endl;
        delete[] array;
        return 1;
    }

    std::ofstream l1File("l1.txt");
    // std::ofstream l2File("l2.txt");
    // std::ofstream l3File("l3.txt");

    if (!l1File.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл" << std::endl;
        delete[] array;
        return 1;
    }

    unsigned int offsets[] = {L1 * 2, L2 * 2}; // шаги(смещения) между фрагментами
    unsigned int block_size[] = {L1 / sizeof(int), L2 / sizeof(int)};

    std::ofstream* files[] = {&l1File}; // массив указателей на файлы с результатами

    // перебираем фрагменты для разных кешей
    // for (int i = 0; i < 3; ++i) {
    for (int fragment_count = 1; fragment_count <= 32; ++fragment_count) {
        CountTime(array, fragment_count, offsets[0], block_size[0], *files[0]);
    }

    std::cout << "Завершил перебор фрагментов" << std::endl;

    l1File.close();
    // l2File.close();

    delete[] array;
    return 0;
}
