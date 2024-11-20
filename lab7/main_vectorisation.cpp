#include "main_vectorisation.h"

int main(int argc, char** argv) {
    int N = 2048, M = 10;

    if (argc == 3) {
        N = std::stoi(argv[1]);
        M = std::stoi(argv[2]);
    }

    srand(static_cast<unsigned>(time(nullptr)));

    Matrix matrix(N);
    matrix.FillRandom();

    struct timespec start, end;
    std::cout << "Started searching for inversed matrix..." << std::endl;

    clock_gettime(CLOCK_MONOTONIC, &start); // замер времени начала вычисления обратной матрицы
    Matrix inverse = matrix.FindInverseMatrix(matrix, M);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double duration = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    std::cout << "Elapsed time: " << duration << " seconds" << std::endl;

    Matrix identity = matrix.MultiplyMatrices(inverse); // Проверка результата (обратная * исходная) = единичная (предполагаемая)
    
    Matrix expected(N);
    expected.MakeIdentity();

    std::cout << "Difference: " << expected.CalculateDifference(identity) << "\n" << std::endl;
    
    return 0;
}
