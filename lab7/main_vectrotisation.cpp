#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <xmmintrin.h> // для __m128 операций 
// x86 SSE instructions can be accessed from C/C++ via the header <xmmintrin.h>
#include <algorithm> // std::swap

// структура для хранения матрицы
struct Matrix {
    std::vector<float> elements;
};

unsigned int N = 2048; // размерность матрицы
unsigned int M = 10;   // количество итераций

// функция нахождения максимальной суммы по строкам матрицы
// просто прохожу по строкам, считаю суммы элементов строки и ищу такую максималььную
float findMaxAbsSumByRows(const Matrix& matrix) {
    float result = 0.0f;

    for (unsigned int i = 0; i < N; i++) {
        float current_sum = 0.0f;
        for (unsigned int j = 0; j < N; j++) {
            current_sum += std::fabs(matrix.elements[i * N + j]);
        }
        result = std::max(result, current_sum);
    }

    return result;
}

// макс сумма по столбцам матрицы
float findMaxAbsSumByColumns(const Matrix& matrix) {
    float result = 0.0f;

    for (unsigned int i = 0; i < N; i++) {
        float current_sum = 0.0f;
        for (unsigned int j = 0; j < N; j++) {
            current_sum += std::fabs(matrix.elements[j * N + i]);
        }
        result = std::max(result, current_sum);
    }

    return result;
}

// создание матрицы и заполнение нулями
Matrix createMatrix() {
    return Matrix{std::vector<float>(N * N, 0.0f)};
}

// Заполнение матрицы случайными значениями
void fillMatrix(Matrix& matrix) {
    for (auto& element : matrix.elements) {
        element = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }
}

// скад=лярное произведение двух векторов a и b с использованием SIMD операций
float dot(const float* a, const float* b) {
    const __m128* x = reinterpret_cast<const __m128*>(a);
    const __m128* y = reinterpret_cast<const __m128*>(b);
    __m128 result_vector = _mm_setzero_ps();

    for (unsigned int i = 0; i < N / 4; i++) {
        result_vector = _mm_add_ps(result_vector, _mm_mul_ps(x[i], y[i]));
    }

    float result[4];
    _mm_storeu_ps(result, result_vector);
    return result[0] + result[1] + result[2] + result[3];
}

// создание заполненной случайными числами от 0 до 1 матрицы 
Matrix createFilledMatrix() {
    Matrix matrix = createMatrix();
    fillMatrix(matrix);
    return matrix;
}

// Создание единичной матрицы
Matrix createIdentityMatrix() {
    Matrix matrix = createMatrix();
    for (unsigned int i = 0; i < N; i++) {
        matrix.elements[i * N + i] = 1.0f;
    }
    return matrix;
}

// Транспонирование матрицы
void transpose(Matrix& matrix) {
    for (unsigned int i = 0; i < N; i++) {
        for (unsigned int j = i + 1; j < N; j++) {
            std::swap(matrix.elements[i * N + j], matrix.elements[j * N + i]);
        }
    }
}

// Умножение матрицы на число
void multiplyByNumber(Matrix& matrix, float number) {
    for (auto& element : matrix.elements) {
        element *= number;
    }
}

// умножение матриц с использованием векторизации
Matrix multiplyMatrices(const Matrix& A, const Matrix& B) {
    Matrix result = createMatrix();

    for (unsigned int i = 0; i < N; i++) {
        for (unsigned int j = 0; j < N; j++) {
            result.elements[i * N + j] = dot(&A.elements[i * N], &B.elements[j * N]);
        }
    }

    return result;
}

// Функция для вычисления обратной матрицы (без изменений, как и в предыдущем варианте)
Matrix findInverseMatrix(const Matrix& matrix) {
    // транспонированируем
    Matrix B = matrix;
    transpose(B);

    // нахождение коэффициентов для масштабирования B
    float A1 = findMaxAbsSumByColumns(matrix);
    float A2 = findMaxAbsSumByRows(matrix);

    multiplyByNumber(B, 1.0f / A1);
    multiplyByNumber(B, 1.0f / A2);

    // инициализация матрицы ошибки R, изначально она единичная
    Matrix R = createIdentityMatrix();
    Matrix RSeries = createIdentityMatrix();
    
    Matrix matrixTransposed = matrix; // транспонированная исходная матрица.
    transpose(matrixTransposed);

    // вычисление BA
    Matrix BA = multiplyMatrices(B, matrixTransposed);

    // вычисление R = E - BA
    for (unsigned int i = 0; i < N * N; i++) {
        R.elements[i] -= BA.elements[i];
    }
    transpose(R);

    Matrix result = createIdentityMatrix();

    // Вычисление R^k и добавление к result.
    for (unsigned int i = 1; i < M; i++) {
        Matrix tmp = multiplyMatrices(RSeries, R);
        
        RSeries = tmp;
        // result += R^k
        for (unsigned int j = 0; j < N * N; j++) {
            result.elements[j] += RSeries.elements[j];
        }
    }

    // result *= B (было изначально транспонированно, поэтому восстанавливаем)
    transpose(B);
    for (unsigned int i = 0; i < N * N; i++) {
        result.elements[i] *= B.elements[i];
    }

    return result;
}

static float calculateDifference(const Matrix& m1, const Matrix& m2) {
        float diff = 0.0f;

        for (unsigned int i = 0; i < N * N; ++i) {
            diff += std::fabs(m1.elements[i] - m2.elements[i]);
        }

        return diff;
    }

int main(int argc, char** argv) {
    if (argc == 3) {
        N = std::stoi(argv[1]);
        M = std::stoi(argv[2]);
    }

    srand(static_cast<unsigned>(time(nullptr)));

    Matrix matrix = createFilledMatrix(); // заполнение случайными числами от 0 до 1

    struct timespec start, end;
    std::cout << "Started searching for inversed matrix..." << std::endl;

    clock_gettime(CLOCK_MONOTONIC, &start); // замер времени начала вычисления обратной матрицы
    Matrix inverse = findInverseMatrix(matrix);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double duration = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    std::cout << "Time of calculation inversed matrix: " << duration << " seconds" << std::endl;

    Matrix identity = multiplyMatrices(inverse, matrix); // Проверка результата (обратная * исходная) = единичная (предполагаемая)
    Matrix expectedIdentity = createIdentityMatrix(); // жидаемая единичная матрица.

    std::cout << "Difference: " << calculateDifference(identity, expectedIdentity) << std::endl;
    
    return 0;
}
