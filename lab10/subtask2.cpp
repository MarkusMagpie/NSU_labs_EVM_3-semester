#include <iostream>
#include <thread>
#include <sched.h> // cpu_set_t
#include <mutex> // std::lock_guard

#include "subtask1.h"

// size_t CACHE_LINE_SIZE = 64;
size_t ARRAY_SIZE = 64 * 1024 * 1024;
size_t NUM_THREADS = 4; // количество потоков

// https://www.geeksforgeeks.org/std-mutex-in-cpp/
std::mutex output_mutex;

void MemoryTask(size_t thread_id, void (*operation)(float*, float*, size_t), size_t size) {
    // привязка потока к ядру
    // https://man7.org/linux/man-pages/man3/cpu_or.3.html
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset); // удалям информацию о любых ранее установленных в set ядрах
    CPU_SET(thread_id, &cpuset); // добавление ядра с индексом thread_id в set
    // https://man7.org/linux/man-pages/man2/sched_setaffinity.2.html
    sched_setaffinity(0, sizeof(cpu_set_t), &cpuset); // 0 - привязка к текущему (нулевому) потоку

    // после вышенаписанного программа будет выполняться только на текущем ядре указанном по индексу потока thread_id (от 0 дo 4 - power cores (до 12 - потоки шести power cores))

    // выделение памяти для каждого потока
    // float* src = static_cast<float*>(aligned_alloc(CACHE_LINE_SIZE, size * sizeof(float))); // выделение size*sizeof(float) байт памяти, которая будет выравнена по CACHE_LINE_SIZE
    float *src = new float[size];
    float *dest = new float[size];

    if (!src || !dest) {
        std::cout << "Ошибка выделения памяти" << std::endl;

        delete[] src;
        delete[] dest;
        return;
    }

    std::fill(src, src + size, 1.0f);   // заполнение src массива числами 1
    std::fill(dest, dest + size, 0.0f);

    
    // барьер 
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // пауза 100 мс чтобы все потоки были в одинаковом состоянии перед началом теста

    // Тест производительности
    operation(src, dest, size); // разогрев? кэша...? (УТОЧНИТЬ)
    auto start = std::chrono::high_resolution_clock::now();
    operation(src, dest, size);
    auto end = std::chrono::high_resolution_clock::now();
    operation(src, dest, size); // Охлаждение

    std::chrono::duration<double, std::nano> elapsed = end - start;
    int duration = elapsed.count();
    double bandwidth = (size * sizeof(float)) / (duration / 1.0e9) / (1 << 30); // GB/s (2^30 байт = 1 GB)

    // только один поток может взаимодействовать с std::cout
    output_mutex.lock();
    std::cout << "Поток " << thread_id << ": " << duration << " наносекунд, " << bandwidth << " GB/s" << std::endl;
    output_mutex.unlock();

    delete[] src;
    delete[] dest;
}

int main() {
    std::cout << "Тест многопоточной работы с памятью" << std::endl;

    std::vector<std::thread> threads;

    std::cout << "\nТест чтения памяти" << std::endl;
    for (size_t i = 0; i < NUM_THREADS; ++i) {
        // 2 варика написания потоков
        threads.emplace_back(MemoryTask, i, ReadMemory, ARRAY_SIZE / NUM_THREADS); 
        // threads.push_back(std::thread(MemoryTask, i, ReadMemory, ARRAY_SIZE / NUM_THREADS));
    }
    for (auto& t : threads) { // проход по всем потокам (поток main приостанавливает выполнение пока не завершатся все 4 потока вектора threads)
        t.join();
    }
    threads.clear(); // очистка вектора

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
