#include <iostream>
#include <fstream>
#include <vector>
#include <x86intrin.h> // для __rdtsc - замеров времени в тактах

#define L1 49152     // 48 КБ
#define L2 1310720   // 1.25 МБ
#define L3 25165824   // 24 МБ

#define TEST_REPEATS 4      // повторения теста для минимального времени
#define ITERATIONS 4          // Коэффициент повторного доступа для увеличения нагрузки

#define CACHES_COUNT 3
#define SIZE_OF_INT 4


std::string PATHS[] = {"l1.txt", "l2.txt", "l3.txt"};
int OFFSETS[] = {L1 / SIZE_OF_INT, L2 / SIZE_OF_INT, L3 / SIZE_OF_INT};

// инициализация массива для фрагментированного доступа
// offset - шаг между фрагментами

// ЛОГИКА:
// заполяняем массив последовательностью индексов, чтобы процессор использовал заданные смещения
// последний элемент замыкает массив в кольцо (циклический обход)
void InitArray(unsigned int* array, int fragments_count, int array_size, int offset) {
    for (int i = 0; i < array_size; i++) {
        for (int j = 0; j < fragments_count; j++) {
            array[i + j * offset] = i + (j + 1) * offset;
        }
        
        array[i + (fragments_count - 1) * offset] = i + 1;
    }
    
    array[(array_size - 1) + (fragments_count - 1) * offset] = 0;
}

// тестирование времени обхода массива
unsigned long long TestArray(unsigned int* array, int array_size) {
    int k = 0;

    auto start_time = __rdtsc();

    // обход массива с множественными переходами
    for (int i = 0; i < array_size * ITERATIONS; ++i) {
        k = array[k];
    }

    auto end_time = __rdtsc();
    auto duration = end_time - start_time; // время обхода массива в тактах
    
    // среднее количество тактов на один доступ
    return duration / array_size / ITERATIONS;
}

int main() {
    for (int i = 0; i < CACHES_COUNT; i++) {
        std::ofstream file(PATHS[i]);

        int offset = OFFSETS[i];
        int array_size = 32 * offset;

        // выделение памяти для массива
        unsigned int* array = new unsigned int[array_size];

        if (!array) {
            std::cerr << "Ошибка выделения памяти!\n";
            return 1;
        }

        for (int fragments_count = 1; fragments_count <= 32; ++fragments_count) {
            InitArray(array, fragments_count, array_size, offset);

            unsigned long long min_time = 10000000;

            // поиск минимального времени доступа
            for (int j = 0; j < TEST_REPEATS; j++) {
                unsigned long long current_time = TestArray(array, array_size);
                min_time = std::min(min_time, current_time);
            }

            // запись результатов в файл
            file << fragments_count << " " << min_time << "\n";
        }

        delete[] array;
        file.close();
    }

    return 0;
}
