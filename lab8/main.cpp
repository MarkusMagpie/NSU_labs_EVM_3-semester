#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <fstream>

unsigned long long MeasureAccessTime(const std::vector<int>& array, std::fstream& dev_null) {
    auto start = std::chrono::high_resolution_clock::now(); // Начало замера

    size_t sum = 0;
    size_t prev = 0;
    for (size_t i = 0; i < array.size(); i++) {
        sum += array[prev]; // Доступ к элементу по указанному индексу
        prev = array[prev];
    }

    auto end = std::chrono::high_resolution_clock::now(); // Конец замера
    std::chrono::duration<double, std::nano> elapsed_time = end - start; // Разница во времени в наносекундах

    dev_null << sum << std::endl;
    
    return static_cast<unsigned long long>(elapsed_time.count()); // Возвращаем время в наносекундах
}

std::vector<size_t> GenerateIndices(size_t size, const std::string& mode) {
    std::vector<size_t> indices(size);
    std::iota(indices.begin(), indices.end(), 0); // Заполняем индексами 0, 1, 2 и т.д.

    if (mode == "reverse") {
        std::reverse(indices.begin(), indices.end()); // Обратный порядок
    }

    return indices;
}

int main() {
    std::vector<size_t> sizes;
    for (size_t i = 1*1024; i <= 2 * 1024*1024; i += 1024) {
        sizes.push_back(i);
    }

    std::fstream dev_null("/dev/null", std::ios::out);

    std::fstream forward_res("forward_result.txt", std::ios::out);
    std::fstream reverse_res("reverse_result.txt", std::ios::out);

    std::string modes[] = {"forward", "reverse"};

    for (size_t size : sizes) {
        std::vector<int> array(size * sizeof(int)); // создаем массив нужного размера в байтах

        for (const std::string& mode : modes) {
            const int repetitions = 10; // Количество повторений для замера
            auto indices = GenerateIndices(size * sizeof(int), mode);

            size_t index = 0;
            for (size_t i = 0; i < size * sizeof(int); ++i) {
                array[index] = indices[i];
                index = indices[i];
            }

            unsigned long long min_time_ns = 1000000000;
            for (int r = 0; r < repetitions; r++) {
                min_time_ns = std::min(min_time_ns, MeasureAccessTime(array, dev_null));
            }

            double avg_time_per_element_ns = min_time_ns / size;

            // size * sizeof(int) - размер массива в байтах
            // avg_time_per_element_ns - среднее время доступа к одному элементу в наносекундах
            if (mode == "forward") {
                forward_res << size / 1024 << " " << avg_time_per_element_ns << std::endl;
            } else if (mode == "reverse") {
                reverse_res << size / 1024 << " " << avg_time_per_element_ns << std::endl;
            }
        }
    }

    return 0;
}
