#pragma once

#include <vector>
#include <chrono> // std::chrono
#include <cstring> // std::memcpy
#include <immintrin.h> // SSE instructions

// функции шаблонные для возможности проверки с разными типами данных

// стандартное чтение
template <typename T>
void TestRead(T* array, size_t size) {
    volatile T sum = 0; // volatile блочит оптимизацию

    for (volatile size_t i = 0; i < size; ++i) {
        sum += array[i];
    }
}

// 2 subtask
void ReadMemory(float* src, float*, size_t size) {
    volatile float sum = 0;
    for (volatile size_t i = 0; i < size; ++i) {
        sum += src[i];
    }
}

// unrollled чтение
template <typename T>
void UnrolledTestRead(T* array, size_t size) {
    volatile T sum = 0;
    size_t i = 0;

    for (; i + 4 <= size; i += 4) {
        sum += array[i] + array[i+1] + array[i+2] + array[i+3];
    }

    for (; i < size; ++i) {
        sum += array[i];
    }
}

// non non-temporal чтение - НЕ ПО УСЛОВИЮ 
// тут нюанс ибо __mm_store_ps записывает данные из SIMD регистра сначала в кэш, а уже потом в кэш

// void Non_Temporal_Load(const float* src, float* dest, size_t size) {
//     for (size_t i = 0; i < size; i += 8) {
//         // Загрузка 4 элементов (256 бит) из src
//         __m128 data = _mm_load_ps(&src[i]); // загружаем 4 float элемента из src
//         _mm_store_ps(&dest[i], data);       // сохраняем 4 float элемента в dest по выровненному адресу 
//     }
// }

//--------------------------------------------------------------------------------------------------------

// тест записи в массив array
template <typename T>
void TestWrite(T* array, size_t size) {
    for (volatile size_t i = 0; i < size; ++i) {
        array[i] = static_cast<T>(i);
    }
}

// 2 subtask
void WriteMemory(float*, float* dest, size_t size) {
    for (volatile size_t i = 0; i < size; ++i) {
        dest[i] = 0;
    }
}

// unrolled запись
template <typename T>
void UnrolledTestWrite(T* array, size_t size) {
    size_t i = 0;

    for (; i + 4 <= size; i += 4) {
        array[i] = static_cast<T>(i);
        array[i+1] = static_cast<T>(i+1);
        array[i+2] = static_cast<T>(i+2);
        array[i+3] = static_cast<T>(i+3);
    }

    for (; i < size; ++i) {
        array[i] = static_cast<T>(i);
    }
}

//--------------------------------------------------------------------------------------------------------

// копирование в массив dest
template <typename T>
void TestCopy(T* src, T* dest, size_t size) {
    for (volatile size_t i = 0; i < size; ++i) {
        dest[i] = src[i];
    }
}

// 2 subtask
void CopyMemory(float* src, float* dest, size_t size) {
    for (volatile size_t i = 0; i < size; ++i) {
        dest[i] = src[i];
    }
}

// unrolled копирование 
template <typename T>
void UnrolledTestCopy(T* src, T* dest, size_t size) {
    size_t i = 0;

    for (; i + 4 < size; i += 4) {
        dest[i] = src[i];
        dest[i+1] = src[i+1];
        dest[i+2] = src[i+2];
        dest[i+3] = src[i+3];
    }

    for (; i < size; ++i) {
        dest[i] = src[i];
    }
}

void Test_Memcpy(void* src, void* dest, size_t size) {
    // https://en.cppreference.com/w/cpp/string/byte/memcpy
    // Copies size bytes from the object pointed to by src to the object pointed to by dest. Both objects are reinterpreted as arrays of unsigned char.
    std::memcpy(dest, src, size);
}

// 1 and 2 subtask
void Non_Temporal_Copy(float* src, float* dest, size_t size) {
    for (size_t i = 0; i < size; i += 4) {
        __m128 data = _mm_load_ps(&src[i]); // загружаем 4 float элемента из src
        _mm_stream_ps(&dest[i], data);      // сохраняем 4 float элемента в dest (без записи в кэш => non-temporal instruction)
    }
}
