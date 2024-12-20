#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <emmintrin.h>
// x86 SSE2 instructions can be accessed from C/C++ via the header <xmmintrin.h>
#include <algorithm> // std::swap

class Matrix {
public:
    std::vector<float> elements;
    int size;

    Matrix(int n) : size(n), elements(n * n, 0.0f) {}

    void MakeIdentity() {
        for (int i = 0; i < size; i++) {
            elements[i * size + i] = 1.0f;
        }
    }

    void FillRandom() {
        for (auto& element : elements) {
            element = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        }
    }

    void Transpose(Matrix& matrix) {
        for (int i = 0; i < size; i++) {
            for (int j = i + 1; j < size; j++) {
                std::swap(matrix.elements[i * size + j], matrix.elements[j * size + i]);
            }
        }
    }

    void MultiplyByNumber(Matrix& matrix, float number) {
        for (auto& element : matrix.elements) {
            element *= number;
        }
    }

    float FindMaxAbsSumByRows(Matrix& matrix) {
        float result = 0.0f;

        for (int i = 0; i < size; i++) {
            float current_sum = 0.0f;
            for (int j = 0; j < size; j++) {
                current_sum += std::fabs(matrix.elements[i * size + j]);
            }
            result = std::max(result, current_sum);
        }

        return result;
    }

    float dot(float* a, float* b) {
        __m128* x = reinterpret_cast<__m128*>(a);
        __m128* y = reinterpret_cast<__m128*>(b);
        __m128 result_vector = _mm_setzero_ps();

        for (int i = 0; i < size / 4; i++) {
            result_vector = _mm_add_ps(result_vector, _mm_mul_ps(x[i], y[i]));
        }

        float result[4];
        _mm_storeu_ps(result, result_vector);
        return result[0] + result[1] + result[2] + result[3];
    }

    Matrix MultiplyMatrices(Matrix& other) {
        Matrix result(size);
        std::vector<float> column_buffer(size); // Буфер для хранения текущего столбца

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                // столбец j из матрицы other в column_buffer
                for (int k = 0; k < size; k++) {
                    column_buffer[k] = other.elements[k * size + j];
                }

                // скалярное произведение строки i из A и столбца j из B
                result.elements[i * size + j] = dot(&elements[i * size], column_buffer.data());
            }
        }

        return result;
    }

    Matrix MultiplyMatrices_v1(Matrix& other) {
        Matrix result(size);

        for (int i = 0; i < size; ++i) {
            float *c = result.elements.data() + i * size; // указатель на начало строки в результирующей матрице
            for (int k = 0; k < size; ++k) {
                float *b = other.elements.data() + k * size; // указатель на начало строки во второй матрице (other)
                float a = elements[i * size + k]; // элемент из текущей матрицы
                
                for (int j = 0; j < size; j += 4) {
                    __m128 b_vec = _mm_loadu_ps(b + j);         // загружаем 4 элемента из b
                    __m128 a_vec = _mm_set1_ps(a);              // загружаем a в каждую позицию вектора
                    __m128 c_vec = _mm_loadu_ps(c + j);         // загружаем текущие значения результата
                    c_vec = _mm_add_ps(c_vec, _mm_mul_ps(a_vec, b_vec));    // c[j:j+4] += a * b[j:j+4]
                    _mm_storeu_ps(c + j, c_vec); // сохраняем обратно в c
                }
            }
        }

        return result;
    }

    float CalculateDifference(Matrix& other) {
        float diff = 0.0f;

        for (int i = 0; i < elements.size(); ++i) {
            diff += std::fabs(elements[i] - other.elements[i]);
        }

        return diff;
    }

    Matrix FindInverseMatrix(Matrix& matrix, int iterations) {
        // 1
        Matrix B = matrix;
        Transpose(B);

        float A1 = FindMaxAbsSumByRows(matrix);
        float A2 = FindMaxAbsSumByRows(B);

        MultiplyByNumber(B, 1.0f / (A1 * A2));

        // 2
        // инициализация матрицы ошибки R, изначально она единичная
        Matrix R(size);
        R.MakeIdentity();

        // BA = (A^{T}/(A1 * A2)) * A
        Matrix BA = B.MultiplyMatrices_v1(matrix);

        // R = E - BA
        for (int i = 0; i < matrix.size * matrix.size; i++) {
            R.elements[i] -= BA.elements[i];
        }

        Matrix result(size);
        result.MakeIdentity();

        // Вычисление R^k и добавление к result
        Matrix RSeries = R;
        for (int i = 1; i < iterations; i++) {
            RSeries = RSeries.MultiplyMatrices_v1(R);
            // result += R^k (R^k = Rseries)
            for (int j = 0; j < size * size; j++) {
                result.elements[j] += RSeries.elements[j];
            }
        }

        result = result.MultiplyMatrices_v1(B);
        return result;
    }
};