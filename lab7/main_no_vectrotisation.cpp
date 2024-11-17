#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm> // std::swap

// Matrix - квадратная матрица с функциональностью для различных операций
class Matrix {
public:
    std::vector<float> elements;    // храним элементы матрицы в массиве.
    unsigned int size;              // размер матрицы (количество rows or columns)

    // конструктор - создает матрицу размера n x n и заполняет нулями.
    Matrix(unsigned int n) : size(n), elements(n * n, 0) {}

    // возвращает ссылку на элемент матрицы (строка row, столбец col).
    float& at(unsigned int row, unsigned int col) {
        return elements[row * size + col];
    }

    // Константная версия метода at для доступа к элементам.
    const float& at(unsigned int row, unsigned int col) const {
        return elements[row * size + col];
    }

    // Заполнение матрицы СЛУЧАЙНО числами в диапазоне [0, 1].
    void fillRandom() {
        for (auto& element : elements) {
            element = static_cast<float>(rand()) / RAND_MAX;
        }
    }

    // преобразование матрицы в единичную (главная диагональ заполняется единицами, другие - нулями).
    void makeIdentity() {
        std::fill(elements.begin(), elements.end(), 0);
        for (unsigned int i = 0; i < size; ++i) {
            at(i, i) = 1.0f; // 1.0f - это константа типа float
        }
    }

    // транспонирование матрицы (замена строк на столбцы).
    void transpose() {
        for (unsigned int i = 0; i < size; ++i) {
            for (unsigned int j = i + 1; j < size; ++j) {
                std::swap(at(i, j), at(j, i));
            }
        }
    }

    // вычитание другой матрицы из текущей матрицы класса
    void subtract(const Matrix& other) {
        for (unsigned int i = 0; i < size * size; ++i) {
            elements[i] -= other.elements[i];
        }
    }

    // сложение другой матрицы с текущей
    void add(const Matrix& other) {
        for (unsigned int i = 0; i < size * size; ++i) {
            elements[i] += other.elements[i];
        }
    }

    // умножение всех элементов матрицы на заданную конств=анту
    void multiplyByNumber(float number) {
        for (auto& element : elements) {
            element *= number;
        }
    }

    // СПОРНО   
    // умножение текущей матрицы на другую 
    Matrix multiply(const Matrix& other) const {
        Matrix result(size);
        
        for (unsigned int i = 0; i < size; ++i) {
            for (unsigned int j = 0; j < size; ++j) {
                float dotProduct = 0.0f;
                for (unsigned int k = 0; k < size; ++k) {
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

        for (unsigned int i = 0; i < matrix.size; ++i) {
            float rowSum = 0.0f;
            for (unsigned int j = 0; j < matrix.size; ++j) {
                rowSum += std::fabs(matrix.at(i, j));
            }
            maxSum = std::max(maxSum, rowSum);
        }

        return maxSum;
    }

    // по столбцам - аналогично
    static float findMaxAbsSumByColumns(const Matrix& matrix) {
        float maxSum = 0.0f;

        for (unsigned int i = 0; i < matrix.size; ++i) {
            float colSum = 0.0f;
            for (unsigned int j = 0; j < matrix.size; ++j) {
                colSum += std::fabs(matrix.at(j, i));
            }
            maxSum = std::max(maxSum, colSum);
        }

        return maxSum;
    }

    // расчет разности между двумя матрицами (поэлементная сумма разностей элементов матриц).
    static float calculateDifference(const Matrix& m1, const Matrix& m2) {
        float diff = 0.0f;

        for (unsigned int i = 0; i < m1.size * m1.size; ++i) {
            diff += std::fabs(m1.elements[i] - m2.elements[i]);
        }

        return diff;
    }

    // печать матрицы на экран
    void print() const {
        for (unsigned int i = 0; i < size; ++i) {
            for (unsigned int j = 0; j < size; ++j) {
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
    float A1 = Matrix::findMaxAbsSumByColumns(matrix);
    float A2 = Matrix::findMaxAbsSumByRows(matrix);
    B.multiplyByNumber(1 / (A1 * A2));

    // Инициализация матрицы ошибки R, изначально она единичная
    Matrix R(n);
    R.makeIdentity();

    // Вычисление BA и R = E - BA
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

int main(int argc, char** argv) {
    int N = 2048; // размер матрицы по умолчанию
    int M = 10;   // количество итераций по умолчанию

    // чтение параметров из командной строки (если есть что читать)
    if (argc == 3) {
        N = std::stoi(argv[1]);
        M = std::stoi(argv[2]);
    }

    srand(static_cast<unsigned int>(time(nullptr))); // инициализация генератора случайных чисел по текущему времени

    Matrix matrix(N);
    matrix.fillRandom(); // заполнение матрицы случайными дробными значениями от 0 до 1

    struct timespec start, end;
    std::cout << "Started searching for inversed matrix..." << std::endl;

    clock_gettime(CLOCK_MONOTONIC, &start); // замер времени начала вычисления обратной матрицы
    Matrix inverse = findInverseMatrix(matrix, M); 
    clock_gettime(CLOCK_MONOTONIC, &end); // замер времени окончания вычисления
    double duration = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    inverse.transpose(); // транспонирование обратной матрицы inverse
    Matrix identity = inverse.multiply(matrix); // Проверка результата (обратная * исходная) = единичная (предполагаемая)

    Matrix expectedIdentity(N);
    expectedIdentity.makeIdentity(); // жидаемая единичная матрица.

    std::cout << "Time of calculation inversed matrix: " << duration << " seconds" << std::endl;
    std::cout << "Difference: " << Matrix::calculateDifference(identity, expectedIdentity) << std::endl;

    return 0;
}
