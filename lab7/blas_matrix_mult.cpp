#include <iostream>
#include <cblas.h>
#include <vector>

// просто печатает матрицу
void PrintMatrix(const std::vector<float>& matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << matrix[i * cols + j] << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    int m = 2; // к-во строк (rows) матрицы A
    int n = 3; // к-во столбцов (columns) матрицы A и количество строк матрицы B
    int k = 2; // к-во столбцов матрицы B

    // инициализация матриц A и B
    std::vector<float> A = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
    std::vector<float> B = {7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f};
    std::vector<float> C (m * k); // hезультирующая матрица (пока задали только размер)

    // параметры функции cblas_sgemm
    float alpha = 1.0f; // коэффициент для умножения элементов матрицы A
    float beta = 0.0f;  // коэффициент для умножения элементов матрицы C

    // Выполнение умножения матриц: C = alpha * A * B + beta * C
    // A - (m x n), B - (n x k), и C - (m x k)
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                m, k, n, alpha,
                A.data(), n, 
                B.data(), k,
                beta, C.data(), k);

    // Вывод результата
    std::cout << "Matrix A:" << std::endl;
    PrintMatrix(A, m, n);

    std::cout << "Matrix B:" << std::endl;
    PrintMatrix(B, n, k);

    std::cout << "Result Matrix C:" << std::endl;
    PrintMatrix(C, m, k);

    return 0;
}
