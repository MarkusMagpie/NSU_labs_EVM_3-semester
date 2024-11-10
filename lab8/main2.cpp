#include <fstream>
#include <ctime>
#include <cmath> // INFINITY
#include <algorithm> // std::swap
#include <chrono>
#include <iostream>
#include <numeric> // std::iota

#define STEP (1 * 1024)             // шаг увеличения размера массива
#define MAX_SIZE (2000 * 1024)  // максимальный размер массива

// перемешивание элементов массива СЛУЧАЙНО
void Shuffle(size_t* array, size_t n) {
    if (n < 2) { return; } // массив слишком мал для перемешивания
    for (size_t i = 0; i < n - 1; i++) {
        size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
        // swap(&array[i], &array[j]);
        std::swap(array[i], array[j]);
    }
}

// Плиск значения в массиве
size_t Find(size_t* array, size_t n, size_t value) {
    for (size_t i = 0; i < n; i++) {
        if (array[i] == value) {
            return i;
        }
    }

    return -1;
}

// С атрибутом noinline компилятор не будет встраивать эту функцию в место вызова, 
//     а будет всегда выполнять фактический вызов функции. 
// Для отладки
__attribute__((noinline)) uint64_t ff(size_t data[], size_t size) {
    uint64_t sum = 0;
    size_t prev = 0;
    // основной цикл, многократно проходящий по данным и суммирующий индексы
    for (size_t i = 0; i < size; i++) {
        size_t current = data[prev];
        sum += current;
        prev = current;
    }
    return sum;
}

// функция для тестирования времени доступа к данным
long f(size_t size) {
    // ыделение памяти для индексов и заполнение их значениями от 0 до size-1
    size_t* indicies = new size_t[size];
    std::iota(indicies, indicies + size, 0);
    Shuffle(indicies, size); // перемешка индексов

    // Находим индекс, равный 0, и перемещаем его в конец массива
    size_t zero = Find(indicies, size, 0);
    if (zero == -1) {
        printf("Couldn't find zero\n");
        free(indicies);
        exit(EXIT_FAILURE);
    }
    std::swap(indicies[zero], indicies[size - 1]);

    // массив данных, связывая элементы с индикаторами
    size_t* data = new size_t[size];
    size_t index = 0;
    for (size_t i = 0; i < size; i++) {
        data[index] = indicies[i];
        index = indicies[i];
    }
    delete[] indicies;

    const auto start = std::chrono::high_resolution_clock::now();
    uint64_t sum = ff(data, size);
    const auto end = std::chrono::high_resolution_clock::now();
    
    delete[] data;

    std::fstream dev_null("/dev/null", std::ios::out);
    dev_null << sum << std::endl;

    const std::chrono::duration<double, std::nano> elapsed_time = end - start;

    // время выполнения в наносекундах
    return elapsed_time.count();
}

int main() {
    for (size_t i = STEP; i < MAX_SIZE; i += STEP) {
        double kib = i / 1024.0; // Перевод размера в Kib (был в байтах)

        double min_time = INFINITY;
        double f_time = f(i); // Время выполнения функции f
        double curr_time = f_time / i;
        min_time = std::min(min_time, curr_time);
        // Вывод размера массива в KiB и минимального времени
        // printf("%f\t%f\n", kib, min_time);
        std::cout << kib << "\t" << min_time << std::endl;
    }

    return EXIT_SUCCESS;
}
