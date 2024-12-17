#include <iostream>
#include <pthread.h>
// #define _GNU_SOURCE
#include <sched.h> // cpu_set_t
#include <mutex> // std::lock_guard

#include <unistd.h> // getpid

#include "subtask1.h"

size_t ARRAY_SIZE = 1024 * 1024 * 1024;
size_t NUM_THREADS = 14;

std::mutex output_mutex;

struct ThreadArgs {
    size_t thread_id;
    void (*operation)(float*, float*, size_t);
    size_t size;
};

void BindThreadToCore(size_t thread_id) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset); // очистка cpuset
    if (thread_id < 6) {
        // привязываем потоки к power-ядрам (2 потока на ядро, от 1 до 12)
        CPU_SET(thread_id * 2, &cpuset);
        CPU_SET(thread_id * 2 + 1, &cpuset); 
    } else {
        // привязываем потоки к efficient-ядрам (по одному потоку на ядро, от 13 до 20)
        CPU_SET(12 + (thread_id - 6), &cpuset);
    }
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset); // установили привязку

    std::lock_guard<std::mutex> lock(output_mutex);
    // int number = 0;
    // for (size_t i = 0; i < sysconf(_SC_NPROCESSORS_ONLN); ++i) {
    //     if (CPU_ISSET_S(i, sizeof(cpu_set_t), &cpuset)) {
    //         number = i;
    //         break;
    //     }
    // }

    std::cout << "Привязка потока " << thread_id << " к ядру " << std::endl;
    std::cout << "Всего привязок потоков к ядру: " << CPU_COUNT(&cpuset) << std::endl;
    
    // sched_getaffinity(0, sizeof(cpu_set_t), &cpuset); // получили

    // // std::lock_guard<std::mutex> lock(output_mutex);
    // // std::cout << "Количество доступных потоков: " << sysconf(_SC_NPROCESSORS_ONLN) << std::endl;
    // for (size_t i = 0; i < sysconf(_SC_NPROCESSORS_ONLN); ++i) {
    //     std::cout << CPU_ISSET_S(i, sizeof(cpu_set_t), &cpuset) << " ";
    // }
    // std::cout << std::endl;
}

// теста памяти
void* MemoryTask(void *args) {
    ThreadArgs* thread_args = static_cast<ThreadArgs*>(args);
    size_t thread_id = thread_args->thread_id;
    void (*operation)(float*, float*, size_t) = thread_args->operation;
    size_t size = thread_args->size;
    
    // привязка потока к ядру
    BindThreadToCore(thread_id);

    float* src = new float[size];
    float* dest = new float[size];

    std::fill(src, src + size, 1.0f);
    std::fill(dest, dest + size, 0.0f);

    // барьер
    usleep(100 * 1000); // 100 миллисекунд

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
    std::string core_type = (thread_id < 6) ? "Power core" : "Efficient core";
    std::cout << "Поток " << thread_id << " (" << core_type << "): " << duration << " наносекунд, " << bandwidth << " GB/s" << std::endl;

    delete[] src;
    delete[] dest;

    return nullptr;
}

int main() {
    std::cout << "Тест многопоточной работы с памятью и учитывая используемую вычислительную систему" << std::endl;

    pthread_t threads[NUM_THREADS];
    ThreadArgs thread_args[NUM_THREADS];

    std::cout << "\nТест чтения памяти" << std::endl;
    for (size_t i = 0; i < NUM_THREADS; ++i) {
        thread_args[i] = {i, ReadMemory, ARRAY_SIZE / NUM_THREADS};
        // threads.emplace_back(MemoryTask, i, ReadMemory, ARRAY_SIZE / NUM_THREADS);
        pthread_create(&threads[i], nullptr, MemoryTask, &thread_args[i]);
    }
    for (size_t i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], nullptr);
    }

    std::cout << "\nТест записи в память" << std::endl;
    for (size_t i = 0; i < NUM_THREADS; ++i) {
        thread_args[i] = {i, WriteMemory, ARRAY_SIZE / NUM_THREADS};
        pthread_create(&threads[i], nullptr, MemoryTask, &thread_args[i]);
    }
    for (size_t i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], nullptr);
    }

    std::cout << "\nТест копирования памяти" << std::endl;
    for (size_t i = 0; i < NUM_THREADS; ++i) {
        thread_args[i] = {i, CopyMemory, ARRAY_SIZE / NUM_THREADS};
        pthread_create(&threads[i], nullptr, MemoryTask, &thread_args[i]);
    }
    for (size_t i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], nullptr);
    }

    std::cout << "\nТест non-temporal копирования памяти" << std::endl;
    for (size_t i = 0; i < NUM_THREADS; ++i) {
        thread_args[i] = {i, Non_Temporal_Copy, ARRAY_SIZE / NUM_THREADS};
        pthread_create(&threads[i], nullptr, MemoryTask, &thread_args[i]);
    }
    for (size_t i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], nullptr);
    }

    return 0;
}
