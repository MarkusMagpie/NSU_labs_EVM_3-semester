#include <iostream>
#include <fstream>
#include <climits>
#include <x86intrin.h>
#include <cstdlib>

#define L1 49152     // 48 КБ
#define L2 1310720   // 1.25 МБ = 1280 КБ
#define L3 25165824  // 24 МБ

#define RUN_TIMES 4  // количество прогонов для замера

// Функция инициализации массива с фрагментированным отступом
int* InitArray(int fragment_count, size_t offset, size_t size){
    int array_size = offset * fragment_count;
    int* array = new int[array_size];

    for(int i = 0; i < size / fragment_count; ++i) {
        for(int j = 0; j < fragment_count - 1; ++j) {
            array[i + j * offset] = i + (j + 1) * offset;
        }

        array[i + (fragment_count - 1) * offset] = i + 1;
    }

    // array[size - 1] = 0;
    array[size / fragment_count - 1 + (fragment_count - 1) * offset] = 0;

    return array;
}

// Функция для замера времени обхода массива
unsigned long long RunArray(int *array, int size) {
    unsigned long long start, end;
    unsigned long long min_time = ULLONG_MAX;

    for(size_t j = 0; j < RUN_TIMES; j++) {
        start = __rdtsc();  // начало замера времени

        // Обход массива
        for(volatile size_t k = 0, i = 0; i < size; i++) {
            k = array[k];
        }

        end = __rdtsc();  // окончание замера времени
        min_time = std::min(min_time, end - start);
    }

    return min_time / size;
}

// подсчет времени для различных фрагментов
void CountTime(int fragment_count, int offset, int size, std::ofstream& file) {
    int* array = InitArray(fragment_count, offset, size);
    unsigned long long time = RunArray(array, size);

    file << fragment_count << " " << time << std::endl;
}

int main() {
    int offset = 16 * 1024 * 1024 / sizeof(int); // шаг(смещение) между фрагментами - 16МБ по условию задачи
    int array_size = L1 / sizeof(int); // размер массива для L1d кэша

    std::ofstream l1_file("l1.txt");

    for (int fragment_count = 1; fragment_count <= 32; ++fragment_count) {
        CountTime(fragment_count, offset, array_size, l1_file);
    }

    std::cout << "Завершил перебор фрагментов" << std::endl;

    l1_file.close();

    return 0;
}
