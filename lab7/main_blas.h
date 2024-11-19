#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <cblas.h>
#include <cstdlib>

class Matrix {
public:
    std::vector<float> elements;
    unsigned int size;

    // конструктор для создания матрицы заданного размера, заполненяем сразу нулями
    Matrix(int n) : size(n), elements(n * n, 0.0f) {}

    // вывод матрицы на экран
    void Print() const {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                std::cout << elements[i * size + j] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    // Заполнение матрицы случайными числами от 0 до 1
    void FillRandom() {
        for (float& elem : elements) {
            elem = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        }
    }

    static Matrix CreateIdentityMatrix(unsigned int n) {
        Matrix identity(n);

        for (int i = 0; i < n; i++) {
            identity.elements[i * n + i] = 1.0f;
        }

        return identity;
    }

    // кпирование одной матрицы в другую
    void CopyMatrix(Matrix& dest, const Matrix& src) {
        dest.elements = src.elements;
    }

    // нахождение макс. суммы абсолютных значений строк матрицы
    static float FindMaxAbsSumByRows(const Matrix& matrix) {
        float max_sum = 0;

        for (int i = 0; i < matrix.size; i++) {
            float row_sum = 0;
            for (int j = 0; j < matrix.size; j++) {
                row_sum += std::fabs(matrix.elements[i * matrix.size + j]);
                // The fabs() function calculates the absolute value of the floating-point argument x.
            }
            max_sum = std::max(max_sum, row_sum);
        }

        return max_sum;
    }

    // транспонирование матрицы (главная диагональ без изменения)
    // пример
    // [[1, 2, 3], [4, 5, 6], [7, 8, 9]] -> [[1, 4, 7], [2, 5, 8], [3, 6, 9]]
    static void Transpose(Matrix& matrix) {
        for (int i = 0; i < matrix.size; i++) {
            for (int j = i + 1; j < matrix.size; j++) {
                std::swap(matrix.elements[i * matrix.size + j], matrix.elements[j * matrix.size + i]);
            }
        }
    }

    // умножение матрицы на число
    // https://developer.apple.com/documentation/accelerate/1513354-cblas_sscal
    static void MultiplyByNumber(Matrix& matrix, float number) {
        cblas_sscal(matrix.size * matrix.size, number, matrix.elements.data(), 1);
    }

    // Умножение двух матриц (level 3 BLAS function)
    // https://www.intel.com/content/www/us/en/docs/onemkl/developer-reference-c/2023-0/cblas-gemm-001.html
    static Matrix MultiplyMatrices(const Matrix& matrix1, const Matrix& matrix2) {
        Matrix result(matrix1.size);
        // matrix1.size - количество строк/столбцов 1 матрицы
        // matrix2.size - количество столбцов 2 матрицы
        // 1.0f - коэффицент умножения 1 (1 * m1 * m2)
        // 0.0f - коэффицент умножения 2 (0 * m3)
        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                    matrix1.size, matrix2.size, matrix1.size,
                    1.0f, 
                    matrix1.elements.data(), matrix1.size,
                    matrix2.elements.data(), matrix2.size,
                    0.0f, result.elements.data(), result.size);

        return result;
    }

    // для измерения абсолютной разницы между двумя матрицами
    static float CalculateDifference(const Matrix& matrix1, const Matrix& matrix2) {
        float difference = 0;

        for (int i = 0; i < matrix1.size * matrix1.size; i++) {
            difference += std::fabs(matrix1.elements[i] - matrix2.elements[i]);
        }

        return difference;
    }

    // нахождение обратной матрицы 
    static Matrix FindInverseMatrix(const Matrix& matrix, int M) {
        // 1 транспонирование и нормализация данной матрицы
        Matrix B = matrix;
        Transpose(B);

        float A1 = FindMaxAbsSumByRows(matrix);
        float A2 = FindMaxAbsSumByRows(B);

        MultiplyByNumber(B, 1.0f / (A1 * A2)); // масштабируем B приближая его к обратной матрице

        // 2 построение начальной ошибки и установка начального значения результата
        // R = I - BA - матрица-ошибка
        Matrix R = CreateIdentityMatrix(matrix.size);
        Matrix result = R;

        Matrix BA = MultiplyMatrices(B, matrix); // BA - приближение умножить на аргумент
        // https://developer.apple.com/documentation/accelerate/1513188-cblas_saxpy
        cblas_saxpy(matrix.size * matrix.size, -1, BA.elements.data(), 1, R.elements.data(), 1); // R = I + (-1)BA

        // 3 итерирование метода 
        Matrix R_series = R;

        // Итерации для максимального приближения обратной матрицы
        for (unsigned int i = 1; i < M; i++) {
            Matrix tmp = MultiplyMatrices(R_series, R);
            R_series = tmp;
            cblas_saxpy(result.size * result.size, 1.0f, R_series.elements.data(), 1, result.elements.data(), 1); // result += 1 * R_series
        }

        MultiplyByNumber(result, 1.0f / (A1 * A2));
        return result;
    }
};

