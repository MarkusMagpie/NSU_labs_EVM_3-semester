#include <iostream>
#include <thread>
#include <sched.h> // cpu_set_t
#include <mutex> // std::lock_guard

#include "subtask1.h"

size_t ARRAY_SIZE = 1024 * 1024 * 1024;
size_t NUM_THREADS = 20;

std::mutex output_mutex;

void BindThreadToCore(size_t thread_id) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset); // очистка cpuset
    if (thread_id < 12) {
        // привязываем потоки к power-ядрам (2 потока на ядро, от 0 до 11)
        CPU_SET(thread_id / 2, &cpuset); 
    } else {
        // Привязываем потоки к efficient-ядрам (по одному потоку на ядро, от 12 до 19)
        CPU_SET(12 + (thread_id - 12), &cpuset);
    }
    sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);
}

// теста памяти
void MemoryTask(size_t thread_id, void (*operation)(float*, float*, size_t), size_t size) {
    // привязка потока к ядру
    BindThreadToCore(thread_id);

    float* src = new float[size];
    float* dest = new float[size];

    if (!src || !dest) {
        std::cout << "Ошибка выделения памяти" << std::endl;
        delete[] src;
        delete[] dest;
        return;
    }

    std::fill(src, src + size, 1.0f);
    std::fill(dest, dest + size, 0.0f);

    // барьер
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // тест производительности
    operation(src, dest, size);
    auto start = std::chrono::high_resolution_clock::now();
    operation(src, dest, size);
    auto end = std::chrono::high_resolution_clock::now();
    operation(src, dest, size);

    std::chrono::duration<double, std::nano> elapsed = end - start;
    int duration = elapsed.count();
    double bandwidth = (size * sizeof(float)) / (duration / 1.0e9) / (1 << 30); // GB/s

    // для корректного вывода в несколько потоков используем mutex
    std::lock_guard<std::mutex> lock(output_mutex);
    std::string core_type = (thread_id < 12) ? "Power core" : "Efficient core";
    std::cout << "Поток " << thread_id << " (" << core_type << "): " << duration << " наносекунд, " << bandwidth << " GB/s" << std::endl;

    delete[] src;
    delete[] dest;
}

int main() {
    std::cout << "Тест многопоточной работы с памятью и учитывая используемую вычислительную систему" << std::endl;

    std::vector<std::thread> threads;

    std::cout << "\nТест чтения памяти" << std::endl;
    for (size_t i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(MemoryTask, i, ReadMemory, ARRAY_SIZE / NUM_THREADS);
    }
    for (auto& t : threads) {
        t.join();
    }
    threads.clear();

    std::cout << "\nТест записи в память" << std::endl;
    for (size_t i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(MemoryTask, i, WriteMemory, ARRAY_SIZE / NUM_THREADS);
    }
    for (auto& t : threads) {
        t.join();
    }
    threads.clear();

    std::cout << "\nТест копирования памяти" << std::endl;
    for (size_t i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(MemoryTask, i, CopyMemory, ARRAY_SIZE / NUM_THREADS);
    }
    for (auto& t : threads) {
        t.join();
    }
    threads.clear();

    std::cout << "\nТест non-temporal копирования памяти" << std::endl;
    for (size_t i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(MemoryTask, i, Non_Temporal_Copy, ARRAY_SIZE / NUM_THREADS);
    }
    for (auto& t : threads) {
        t.join();
    }
    threads.clear();

    return 0;
}
