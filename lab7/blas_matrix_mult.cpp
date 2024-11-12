#include <iostream>
#include <vector>
#include <cmath>
#include <cblas.h>
#include <ctime>
#include <cstdlib>

class Matrix {
public:
    std::vector<float> elements;
    unsigned int size;

    // конструктор для создания матрицы заданного размера, заполненяем сразу нулями
    Matrix(unsigned int n) : size(n), elements(n * n, 0.0f) {}

    // вывод матрицы на экран
    void print() const {
        for (unsigned int i = 0; i < size; i++) {
            for (unsigned int j = 0; j < size; j++) {
                std::cout << elements[i * size + j] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
};

// Заполнение матрицы случайными числами от 0 до 1
void FillMatrix(Matrix& matrix) {
    for (float& elem : matrix.elements) {
        elem = static_cast<float>(rand()) / RAND_MAX;
    }
}

// Сcздание единичной матрицы [[1, 0, ..., 0], [0, 1, ..., 0], ..., [0, 0, ..., 1]] 
Matrix CreateIdentityMatrix(unsigned int n) {
    Matrix identity(n);
    for (unsigned int i = 0; i < n; i++) {
        identity.elements[i * n + i] = 1.0f;
    }
    return identity;
}

// кпирование одной матрицы в другую
void copyMatrix(Matrix& dest, const Matrix& src) {
    dest.elements = src.elements;
}

// нахождение макс. суммы абсолютных значений строк матрицы
float FindMaxAbsSumByRows(const Matrix& matrix) {
    float max_sum = 0;
    for (unsigned int i = 0; i < matrix.size; i++) {
        float row_sum = 0;
        for (unsigned int j = 0; j < matrix.size; j++) {
            row_sum += std::fabs(matrix.elements[i * matrix.size + j]);
            // The fabs() function calculates the absolute value of the floating-point argument x.
        }
        max_sum = std::max(max_sum, row_sum);
    }
    return max_sum;
}

// нахождение макс суммы абсолютных значений столбцов матрицы
float FindMaxAbsSumByColumns(const Matrix& matrix) {
    float max_sum = 0;
    for (unsigned int i = 0; i < matrix.size; i++) {
        float col_sum = 0;
        for (unsigned int j = 0; j < matrix.size; j++) {
            col_sum += std::fabs(matrix.elements[j * matrix.size + i]);
        }
        max_sum = std::max(max_sum, col_sum);
    }
    return max_sum;
}

// транспонирование матрицы (главная диагональ без изменения)
// пример
// [[1, 2, 3], [4, 5, 6], [7, 8, 9]] -> [[1, 4, 7], [2, 5, 8], [3, 6, 9]]
void Transpose(Matrix& matrix) {
    for (unsigned int i = 0; i < matrix.size; i++) {
        for (unsigned int j = i + 1; j < matrix.size; j++) {
            std::swap(matrix.elements[i * matrix.size + j], matrix.elements[j * matrix.size + i]);
        }
    }
}

// умножение матрицы на число
// https://developer.apple.com/documentation/accelerate/1513354-cblas_sscal
void MultiplyByNumber(Matrix& matrix, float number) {
    cblas_sscal(matrix.size * matrix.size, number, matrix.elements.data(), 1);
}

// Умножение двух матриц (level 3 BLAS function)
Matrix MultiplyMatrices(const Matrix& matrix1, const Matrix& matrix2) {
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

// нахождение обратной матрицы методом Шульца
Matrix FindInverseMatrix(const Matrix& matrix, unsigned int M) {
    // 1 транспонирование и нормализация данной матрицы
    Matrix B = matrix;
    Transpose(B);

    float A1 = FindMaxAbsSumByColumns(matrix);
    float A2 = FindMaxAbsSumByRows(matrix);

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

// Функция для измерения абсолютной разницы между двумя матрицами
float getMatricesDifference(const Matrix& matrix1, const Matrix& matrix2) {
    float difference = 0;
    for (unsigned int i = 0; i < matrix1.size * matrix1.size; i++) {
        difference += std::fabs(matrix1.elements[i] - matrix2.elements[i]);
    }
    return difference;
}

int main(int argc, char** argv) {
    unsigned int N = 2048;  // размер матрицы
    unsigned int M = 10;    // число итераций для приближения

    // Создание и заполнение случайной (0 и 1) матрицы
    Matrix matrix(N);
    FillMatrix(matrix);

    // запуск таймера поиска обратной матрицы
    struct timespec start, end;
    std::cout << "Started searching for inversed matrix..." << std::endl;

    clock_gettime(CLOCK_MONOTONIC, &start);
    Matrix inverse = FindInverseMatrix(matrix, M);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double duration = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    std::cout << "Elapsed time: " << duration << " seconds" << std::endl;

    // std::cout << "Original matrix: " << std::endl;
    // matrix.print();

    // std::cout << "Reversed matrix: " << std::endl;
    // inverse.print();

    // Проверка качества обратной матрицы, умножая её на исходную и сравнивая с единичной
    Matrix matrix_check = MultiplyMatrices(matrix, inverse);

    // std::cout << "Multiplied matrixes: " << std::endl;
    // matrix_check.print();

    Matrix identity = CreateIdentityMatrix(N);
    std::cout << "Difference from identity: " << getMatricesDifference(identity, matrix_check) << "\n";

    return 0;
}
