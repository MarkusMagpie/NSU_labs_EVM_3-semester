#include "main_no_vectorisation.h"

int main(int argc, char** argv) {
    int N = 2048; // размер матрицы по умолчанию
    int M = 10;   // количество итераций по умолчанию

    if (argc == 3) {
        N = std::stoi(argv[1]);
        M = std::stoi(argv[2]);
    }

    srand(static_cast<unsigned int>(time(nullptr))); // инициализация генератора случайных чисел по текущему времени

    Matrix matrix(N);
    matrix.fillRandom();

    std::cout << "Original matrix: " << std::endl;
    matrix.print();

    struct timespec start, end;
    std::cout << "Started searching for inversed matrix..." << std::endl;

    clock_gettime(CLOCK_MONOTONIC, &start); // замер времени начала вычисления обратной матрицы
    Matrix inverse = findInverseMatrix(matrix, M); 
    clock_gettime(CLOCK_MONOTONIC, &end); // замер времени окончания вычисления
    double duration = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    std::cout << "Inverse matrix: " << std::endl;
    inverse.print();

    // inverse.transpose(); // транспонирование обратной матрицы inverse
    Matrix identity = inverse.multiply_v1(matrix); // Проверка результата (обратная * исходная) = единичная (предполагаемая)

    Matrix expectedIdentity(N);
    expectedIdentity.makeIdentity(); // жидаемая единичная матрица

    std::cout << "Identity matrix: " << std::endl;
    identity.print();

    std::cout << "Expected identity matrix: " << std::endl;
    expectedIdentity.print();

    std::cout << "Elapsed time: " << duration << " seconds" << std::endl;
    std::cout << "Difference: " << Matrix::calculateDifference(identity, expectedIdentity) << "\n" << std::endl;

    return 0;
}
