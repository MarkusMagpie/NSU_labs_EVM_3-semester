#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm> // std::swap

class Matrix {
public:
    std::vector<float> elements;    // храним элементы матрицы в массиве.
    unsigned int size;              // размер матрицы (количество rows or columns)

    // конструктор - создает матрицу размера n x n и заполняет нулями
    Matrix(unsigned int n) : size(n), elements(n * n, 0) {}

    // возвращает ссылку на элемент матрицы (строка row, столбец col)
    float& at(unsigned int row, unsigned int col) {
        return elements[row * size + col];
    }

    const float& at(unsigned int row, unsigned int col) const {
        return elements[row * size + col];
    }

    // Заполнение матрицы СЛУЧАЙНО числами в диапазоне [0, 1]
    void fillRandom() {
        for (auto& element : elements) {
            element = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        }
    }

    // преобразование матрицы в единичную (главная диагональ - единицы, другие - нулями)
    void makeIdentity() {
        std::fill(elements.begin(), elements.end(), 0);
        for (int i = 0; i < size; ++i) {
            at(i, i) = 1.0f;
        }
    }

    // транспонирование матрицы (замена строк на столбцы)
    void transpose() {
        for (int i = 0; i < size; ++i) {
            for (int j = i + 1; j < size; ++j) {
                std::swap(at(i, j), at(j, i));
            }
        }
    }

    // вычитание другой матрицы из текущей
    void subtract(const Matrix& other) {
        for (int i = 0; i < size * size; ++i) {
            elements[i] -= other.elements[i];
        }
    }

    // сложение другой матрицы с текущей 
    void add(const Matrix& other) {
        for (int i = 0; i < size * size; ++i) {
            elements[i] += other.elements[i];
        }
    }

    // float determinant() {
    //     if (size == 1) {
    //         return at(0, 0);
    //     }
    //     if (size == 2) {
    //         return at(0, 0) * at(1, 1) - at(0, 1) * at(1, 0);
    //     }

    //     // Разложение по первой строке
    //     float det = 0.0f;
    //     for (int col = 0; col < size; ++col) {
    //         det += at(0, col) * cofactor(0, col);
    //     }
    //     return det;
    // }

    // умножение всех элементов матрицы на заданную константу
    void multiplyByNumber(float number) {
        for (auto& element : elements) {
            element *= number;
        }
    }

    // СПОРНО
    // умножение текущей матрицы на другую матрицу
    Matrix multiply(const Matrix& other) const {
        Matrix result(size);

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                float dotProduct = 0.0f;
                for (int k = 0; k < size; ++k) {
                    dotProduct += at(i, k) * other.at(k, j);
                }

                result.at(i, j) = dotProduct;
            }
        }

        return result;    

    }

    // нахождение максимальной суммы модулей элементов по строкам
    // просто прохожу по строкам, считаю суммы элементов строки и ищу такую максималььную
    static float findMaxAbsSumByRows(const Matrix& matrix) {
        float maxSum = 0.0f;

        for (int i = 0; i < matrix.size; ++i) {
            float rowSum = 0.0f;
            for (int j = 0; j < matrix.size; ++j) {
                rowSum += std::fabs(matrix.at(i, j));
            }
            maxSum = std::max(maxSum, rowSum);
        }

        return maxSum;
    }

    // расчет разности между двумя матрицами (поэлементная сумма разностей элементов матриц)
    static float calculateDifference(const Matrix& m1, const Matrix& m2) {
        float diff = 0.0f;

        for (int i = 0; i < m1.size * m1.size; ++i) {
            diff += std::fabs(m1.elements[i] - m2.elements[i]);
        }

        return diff;
    }

    void print() const {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                std::cout << at(i, j) << " ";
            }
            std::cout << "\n";
        }

        std::cout << "\n";
    }
};


// ВЫЧЕИСЛЕНИЕ ОБРАТНОЙ МАТРИЦЫ
Matrix findInverseMatrix(const Matrix& matrix, unsigned int iterations) {
    unsigned int n = matrix.size;

    // транспонированируем 
    Matrix B = matrix;
    B.transpose();

    // нахождение коэффициентов для масштабирования B
    // после масштабирования значения матрицы будут находиться в пределах, близких к 1, что
    //     минимизирует ошибки, возникающие при итерационном процессе
    float A1 = Matrix::findMaxAbsSumByRows(matrix);
    float A2 = Matrix::findMaxAbsSumByRows(B); // так как B транспонирована, то A2 = сумма по столбцам
    // float A2 = Matrix::findMaxAbsSumByRows(matrix);
    B.multiplyByNumber(1 / (A1 * A2));

    // инициализация матрицы ошибки R, изначально она единичная
    Matrix R(n);
    R.makeIdentity();

    // вычисление BA и R = E - BA
    Matrix BA = B.multiply(matrix);
    R.subtract(BA);

    // итоговая матрица result (пока единичная)
    Matrix result(n);
    result.makeIdentity();

    // Вычисление R^k и добавление к result.
    Matrix RSeries = R;
    for (unsigned int i = 1; i < iterations; ++i) {
        RSeries = RSeries.multiply(R);
        result.add(RSeries);
    }

    // Умножение на транспонированную B.
    B.transpose();
    return result.multiply(B);

    // R = E - BA
    // A^{-1} = B * (E + R + R^2 + ... + R^{k})
}

int main(int argc, char** argv);