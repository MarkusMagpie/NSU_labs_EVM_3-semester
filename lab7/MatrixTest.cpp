#include "main_no_vectrotisation.h"
#include "gtest/gtest.h"
#include <cmath>
#include <ctime>
#include <cstdlib>  // rand; srand

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

// TEST(MatrixTests, FindInverseIdentityTest2) {
//     Matrix m(3);
//     m.at(0, 0) = 1.0f, m.at(0, 1) = 2.0f, m.at(0, 2) = 3.0f;
//     m.at(1, 0) = 4.0f, m.at(1, 1) = 5.0f, m.at(1, 2) = 6.0f;
//     m.at(2, 0) = 7.0f, m.at(2, 1) = 8.0f, m.at(2, 2) = 9.0f;

//     Matrix inverse = findInverseMatrix(m, 10);

//     Matrix identity = m.multiply(inverse);

//     Matrix expectedIdentity(3);
//     expectedIdentity.makeIdentity();


//     for (int i = 0; i < 3; ++i) {
//         for (int j = 0; j < 3; ++j) {
//             ASSERT_NEAR(identity.at(i, j), expectedIdentity.at(i, j), 1);
//         }
//     }   
// }

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

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}