#include "main_no_vectorisation.h"
#include "gtest/gtest.h"

#include <random> // std::mt19937
#include <new>

#include <cmath>
#include <ctime>
#include <cstdlib>  // rand; srand

#include <Eigen/Dense>

// создание и заполнение матрицы нулями
TEST(MatrixTests, ConstructorTest) {
    Matrix m(3);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_EQ(m.at(i, j), 0.0f);
        }
    }
}

TEST(MatrixTests, MakeIdentityTest) {
    Matrix m(3);
    m.makeIdentity();

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (i == j) {
                EXPECT_EQ(m.at(i, j), 1.0f);
            } else {
                EXPECT_EQ(m.at(i, j), 0.0f);
            }
        }
    }
}

TEST(MatrixTests, TransposeTest) {
    Matrix m(3);
    m.at(0, 1) = 1.0f;
    m.at(1, 2) = 2.0f;
    m.at(2, 0) = 3.0f;

    std::cout << "Матрица:" << std::endl;
    m.print();

    m.transpose();

    std::cout << "Транспонированная матрица:" << std::endl;
    m.print();

    EXPECT_EQ(m.at(1, 0), 1.0f);
    EXPECT_EQ(m.at(2, 1), 2.0f);
    EXPECT_EQ(m.at(0, 2), 3.0f);
}

TEST(MatrixTests, MultiplyByNumberTest) {
    Matrix m(2);
    m.at(0, 0) = 1.0f;
    m.at(1, 1) = 2.0f;

    std::cout << "Матрица:" << std::endl;
    m.print();

    m.multiplyByNumber(2.0f);

    std::cout << "Результат умножения матрицы на 2:" << std::endl;
    m.print();

    EXPECT_EQ(m.at(0, 0), 2.0f);
    EXPECT_EQ(m.at(1, 1), 4.0f);
}

TEST(MatrixTests, MultiplyMatricesTest) {
    Matrix m1(2), m2(2);
    m1.at(0, 0) = 1.0f; 
    m1.at(0, 1) = 2.0f;
    m1.at(1, 0) = 3.0f; 
    m1.at(1, 1) = 4.0f;

    m2.at(0, 0) = 2.0f; 
    m2.at(0, 1) = 0.0f;
    m2.at(1, 0) = 1.0f; 
    m2.at(1, 1) = 2.0f;

    std::cout << "Первая матрица:" << std::endl;
    m1.print();
    std::cout << "Вторая матрица:" << std::endl;
    m2.print(); 

    Matrix result = m1.multiply(m2);

    std::cout << "Результат умножения матриц:" << std::endl;
    result.print();

    EXPECT_EQ(result.at(0, 0), 4.0f);
    EXPECT_EQ(result.at(0, 1), 4.0f);
    EXPECT_EQ(result.at(1, 0), 10.0f);
    EXPECT_EQ(result.at(1, 1), 8.0f);
}

TEST(MatrixTests, FindInverseIdentityTest) {
    Matrix identity(3);
    identity.makeIdentity();
    // до нахождения обратной матрицы
    // 1 0 0
    // 0 1 0
    // 0 0 1
    // после она должна сохраниться

    std::cout << "Единичная матрица до нахождения обратной матрицы:" << std::endl;
    identity.print();

    Matrix inverse = findInverseMatrix(identity, 10); 

    std::cout << "Единичная матрица после нахождения обратной матрицы:" << std::endl;
    inverse.print();

    // результат должен остаться единичной матрицей
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (i == j) {
                EXPECT_NEAR(inverse.at(i, j), 1.0f, 1e-5); // ожидаем 1, точностью 1e-5
            } else {
                EXPECT_NEAR(inverse.at(i, j), 0.0f, 1e-5);
            }
        }
    }
}

// Проверяет расчет максимальной суммы модулей элементов по строкам
TEST(MatrixTests, FindMaxAbsSumByRowsTest) {
    Matrix m(2);
    m.at(0, 0) = -1.0f; 
    m.at(0, 1) = -2.0f;
    m.at(1, 0) = 3.0f; 
    m.at(1, 1) = 4.0f;

    std::cout << "Матрица:" << std::endl;
    m.print();

    float maxSum = Matrix::findMaxAbsSumByRows(m);

    std::cout << "Максимальная сумма модулей элементов по строкам: " << maxSum << std::endl;

    EXPECT_EQ(maxSum, 7.0f); // 3.0 + 4.0
}

// разность матриц
TEST(MatrixTests, CalculateDifferenceTest) {
    Matrix m1(2), m2(2);
    m1.at(0, 0) = 1.0f; 
    m1.at(0, 1) = 2.0f;
    m1.at(1, 0) = 3.0f; 
    m1.at(1, 1) = 4.0f;

    m2.at(0, 0) = 2.0f; 
    m2.at(0, 1) = 1.0f;
    m2.at(1, 0) = 4.0f; 
    m2.at(1, 1) = 3.0f;

    std::cout << "Матрица 1:" << std::endl;
    m1.print();
    std::cout << "Матрица 2:" << std::endl;
    m2.print();

    float diff = Matrix::calculateDifference(m1, m2);

    std::cout << "Разность матриц: " << diff << std::endl;

    EXPECT_EQ(diff, 4.0f); // Сумма модулей разностей всех элементов
}

void create_random_diagonally_dominant_matrix(Matrix& matrix) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.1f, 10.0f);

    for (int i = 0; i < matrix.size; ++i) {
        for (int j = 0; j < matrix.size; ++j) {
            matrix.at(i, j) = dis(gen);
        }
    }

    // добавляем диагональное доминирование
    for (int i = 0; i < matrix.size; ++i) {
        float row_sum = 0.0f;
        for (int j = 0; j < matrix.size; ++j) {
            row_sum += matrix.at(i, j);
        }
        matrix.at(i, i) += row_sum;
    }
}

// параметризованный тест
class MatrixInversionParametrizedTest : public ::testing::TestWithParam<std::size_t> {};

TEST_P(MatrixInversionParametrizedTest, test1) {
    std::size_t N = GetParam();
    Matrix matrix(N);
    std::cout << "N = " << N << std::endl;

    create_random_diagonally_dominant_matrix(matrix);

    Matrix inverse_matrix = findInverseMatrix(matrix, 100);

    // преобразуем мою изначальную матрицу и её обратную в формат Eigen для проверки
    Eigen::MatrixXf eigen_matrix = Eigen::Map<Eigen::MatrixXf>(matrix.elements.data(), N, N);
    Eigen::MatrixXf eigen_inverse = Eigen::Map<Eigen::MatrixXf>(inverse_matrix.elements.data(), N, N);

    // проверяем, что произведение matrix * inverse_matrix близко к единичной
    Eigen::MatrixXf identity_check = eigen_matrix * eigen_inverse;
    EXPECT_TRUE(identity_check.isIdentity(1));
}

INSTANTIATE_TEST_SUITE_P(
    MatrixInversionTests,
    MatrixInversionParametrizedTest,
    ::testing::Values(4, 16, 32, 64, 128, 256, 512)
);

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}