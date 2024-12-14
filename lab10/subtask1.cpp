#include <iostream>
#include <vector>
#include <chrono> // std::chrono
#include <cstring> // std::memcpy
#include <immintrin.h> // SSE instructions

# include "subtask1.h"

// так как мой кэш не инклюзивный, то нужен массив больше чем L1d + L2 + L3 (power cores проврека по htop) = 48KB + 1.25 * 1024KB + 24 * 1024KB = 48+1280+24576 = 25904KB > 25MB
size_t ARRAY_SIZE = 64 * 1024 * 1024; // Размер массива (512 МБ)

template <typename Func>
void benchmark(Func func, const char* description) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << description << ": " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
}

int main() {
    // Создаем большие массивы
    // ТУТ МОЖНО КАК РАЗ МЕНЯТЬ ТИП ДАННЫХ В МАССИВЕ 
    long size = ARRAY_SIZE / sizeof(int);
    std::vector<int> src(size, 1);
    std::vector<int> dest(size); // забит нулями

    std::cout << "Тесты чтения массива src:\n";
    benchmark([&]() { TestRead(src.data(), size); }, "Скорочть сиандартного чтения");
    benchmark([&]() { UnrolledTestRead(src.data(), size); }, "Скорость чтения с раскруткой цикла");

    std::cout << "\nТесты записи в массив dest:\n";
    benchmark([&]() { TestWrite(dest.data(), size); }, "Скорость стандартной записи");
    benchmark([&]() { UnrolledTestWrite(dest.data(), size); }, "Скорость записи с раскруткой цикла");

    std::cout << "\nТесты копирования массива src в dest:\n";
    benchmark([&]() { TestCopy(src.data(), dest.data(), size); }, "Скорость стандартного копирования");
    benchmark([&]() { UnrolledTestCopy(src.data(), dest.data(), size); }, "Скорость копирования с раскруткой цикла");

    benchmark([&]() { Test_Memcpy(src.data(), dest.data(), size); }, "Скорость копирования с функцией std::memcpy");

    // Non-temporal копирование (для чисел с плавающей точкой)
    long float_size = ARRAY_SIZE / sizeof(float);
    std::vector<float> src_float(float_size, 1.0f);
    std::vector<float> dest_float(float_size, 0.0f);
    // Non_Temporal_Copy(src_float.data(), dest_float.data(), src_float.size());
    benchmark([&]() { Non_Temporal_Copy(src_float.data(), dest_float.data(), float_size); }, "Скорость non-temporal копирования");

    return 0;
}
