#include "gtest/gtest.h" 
#include "LongNum/LongNum.hpp"    
#include <limits>
#include <cmath>


TEST(LongNumTest, DefaultConstructor) {
    LongNum num;
    EXPECT_EQ(0.0_longnum, num);
}


// Тест для конструктора из long double
// TEST(LongNumTest, LongDoubleConstructor) {
//     LongNum num(123.456L);
//     EXPECT_EQ(num, );
// }

// Тест для сложения
TEST(LongNumTest, Addition) {
    LongNum a(123.L);
    LongNum b(456.L);
    LongNum result = a + b;
    EXPECT_EQ(result, 579._longnum);
}

// Тест для вычитания
TEST(LongNumTest, Subtraction) {
    LongNum a(1000.L);
    LongNum b(123.L);
    LongNum result = a - b;
    EXPECT_EQ(result, 877._longnum);
}

// Тест для умножения
TEST(LongNumTest, Multiplication) {
    LongNum a(123.L);
    LongNum b(456.L);
    LongNum result = a * b;
    EXPECT_EQ(result, 56088._longnum);
}

// Тест для деления
TEST(LongNumTest, Division) {
    LongNum a(1000.L);
    LongNum b(10.L);
    LongNum result = a / b;
    EXPECT_EQ(result, 100._longnum);
}

// Тест для оператора сравнения (меньше)
TEST(LongNumTest, LessThan) {
    LongNum a(123.L);
    LongNum b(456.L);
    EXPECT_TRUE(a < b);
}

// Тест для оператора сравнения (больше)
TEST(LongNumTest, GreaterThan) {
    LongNum a(456.L);
    LongNum b(123.L);
    EXPECT_TRUE(a > b);
}

// Тест для оператора равенства
TEST(LongNumTest, Equality) {
    LongNum a(123.L);
    LongNum b(123.L);
    EXPECT_TRUE(a == b);
}

// Тест для оператора неравенства
TEST(LongNumTest, Inequality) {
    LongNum a(123.L);
    LongNum b(456.L);
    EXPECT_TRUE(a != b);
}

// Тест для метода abs (абсолютное значение)
TEST(LongNumTest, AbsoluteValue) {
    LongNum a(-123.L);
    LongNum result = a.abs();
    EXPECT_EQ(result, 123._longnum);
}

// Тест для метода frac (дробная часть)
// TEST(LongNumTest, FractionalPart) {
//     LongNum a(123.456L);
//     LongNum result = a.frac();
//     EXPECT_EQ(result.toString(3), 0.456_longnum);
// }

// Тест для метода withPrecision
// TEST(LongNumTest, WithPrecision) {
//     LongNum a(123.456789L);
//     LongNum result = a.withPrecision(3);
//     EXPECT_EQ(result.toString(3), 123.456_longnum);
// }

// // Тест для метода toString
// TEST(LongNumTest, ToString) {
//     LongNum a(123.456L);
//     EXPECT_EQ(a.toString(2), 123.45_longnum);
// }

// Тест для литерала _longnum (long double)
// TEST(LongNumTest, LongDoubleLiteral) {
//     LongNum num = 123.456_longnum;
//     EXPECT_EQ(num.toString(3), "123.456");
// }


