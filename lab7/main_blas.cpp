#include "main_blas.h"

int main(int argc, char** argv) {
    int N = 2048;  // размер матрицы
    int M = 10;    // число итераций для приближения обратной матрицы к реальной обратной матрице

    if (argc == 3) {
        N = atoi(argv[1]);
        M = atoi(argv[2]);
    }

    // Создание и заполнение случайной (0 и 1) матрицы
    Matrix matrix(N);
    matrix.FillRandom();

    // запуск таймера поиска обратной матрицы
    struct timespec start, end;
    std::cout << "Started searching for inversed matrix..." << std::endl;

    clock_gettime(CLOCK_MONOTONIC, &start);
    // FindInverseMatrix нужно ключевое слово static для того чтобы метод вызывался без объекта
    //иначе была бы запись типа: inverse.FindInverseMatrix(matrix, M)
    // https://ravesli.com/urok-125-staticheskie-metody-klassa/
    Matrix inverse = Matrix::FindInverseMatrix(matrix, M);
    clock_gettime(CLOCK_MONOTONIC, &end);

    double duration = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    std::cout << "Elapsed time: " << duration << " seconds" << std::endl;

    // std::cout << "Original matrix: " << std::endl;
    // matrix.print();

    // std::cout << "Reversed matrix: " << std::endl;
    // inverse.print();

    // Проверка качества обратной матрицы, умножая её на исходную и сравнивая с единичной
    Matrix check = Matrix::MultiplyMatrices(inverse, matrix);
    Matrix identity = Matrix::CreateIdentityMatrix(N);

    std::cout << "Difference from identity: " << Matrix::CalculateDifference(identity, check) << "\n" << std::endl;

    return 0;
}
