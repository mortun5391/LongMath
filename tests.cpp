#include "gtest/gtest.h"
#include "src/LongNum.hpp"
#include <limits>
#include <cmath>

// Тесты для конструкторов
TEST(LongNumConstructorTest, DefaultConstructor) {
    LongNum num(0.0L);
    EXPECT_EQ(num.toString(), "0");
}

TEST(LongNumConstructorTest, LongDoubleConstructor) {
    LongNum num(3.141592653589793238L);
    EXPECT_EQ(num.toString(18), "3.141592653589793238");
}

TEST(LongNumConstructorTest, LongLongConstructor) {
    LongNum num(1234567890123456789LL);
    EXPECT_EQ(num.toString(), "1234567890123456789");
}

TEST(LongNumConstructorTest, UnsignedLongLongConstructor) {
    LongNum num(12345678901234567890ULL);
    EXPECT_EQ(num.toString(), "12345678901234567890");
}

// Тесты для операторов присваивания
TEST(LongNumAssignmentTest, AssignmentOperator) {
    LongNum num1(123.456L);
    LongNum num2 = num1;
    EXPECT_EQ(num2.toString(3), "123.456");
}

// Тесты для операторов сдвига
TEST(LongNumShiftTest, LeftShift) {
    LongNum num(1.0L);
    num <<= 3;
    EXPECT_EQ(num.toString(), "8");
}

TEST(LongNumShiftTest, RightShift) {
    LongNum num(8.0L);
    num >>= 3;
    EXPECT_EQ(num.toString(), "1");
}

// Тесты для унарных операторов
TEST(LongNumUnaryTest, UnaryPlus) {
    LongNum num(-123.456L);
    LongNum result = +num;
    EXPECT_EQ(result.toString(3), "-123.456");
}

TEST(LongNumUnaryTest, UnaryMinus) {
    LongNum num(123.456L);
    LongNum result = -num;
    EXPECT_EQ(result.toString(3), "-123.456");
}

// Тесты для арифметических операций
TEST(LongNumArithmeticTest, Addition) {
    LongNum num1(123.456L);
    LongNum num2(654.321L);
    LongNum result = num1 + num2;
    EXPECT_EQ(result.toString(3), "777.777");
}

TEST(LongNumArithmeticTest, Subtraction) {
    LongNum num1(654.321L);
    LongNum num2(123.456L);
    LongNum result = num1 - num2;
    EXPECT_EQ(result.toString(3), "530.865");
}

TEST(LongNumArithmeticTest, Multiplication) {
    LongNum num1(123.456L);
    LongNum num2(2.0L);
    LongNum result = num1 * num2;
    EXPECT_EQ(result.toString(3), "246.912");
}

TEST(LongNumArithmeticTest, Division) {
    LongNum num1(123.456L);
    LongNum num2(2.0L);
    LongNum result = num1 / num2;
    EXPECT_EQ(result.toString(3), "61.728");
}

// Тесты для операторов сравнения
TEST(LongNumComparisonTest, Equal) {
    LongNum num1(123.456L);
    LongNum num2(123.456L);
    EXPECT_TRUE(num1 == num2);
}

TEST(LongNumComparisonTest, NotEqual) {
    LongNum num1(123.456L);
    LongNum num2(654.321L);
    EXPECT_TRUE(num1 != num2);
}

TEST(LongNumComparisonTest, LessThan) {
    LongNum num1(123.456L);
    LongNum num2(654.321L);
    EXPECT_TRUE(num1 < num2);
}

TEST(LongNumComparisonTest, GreaterThan) {
    LongNum num1(654.321L);
    LongNum num2(123.456L);
    EXPECT_TRUE(num1 > num2);
}

// Тесты для работы с точностью
TEST(LongNumPrecisionTest, SetPrecision) {
    LongNum num(123.456L);
    num.setPrecision(64);
    EXPECT_EQ(num.getPrecision(), 64);
}

TEST(LongNumPrecisionTest, WithPrecision) {
    LongNum num(123.456L);
    LongNum result = num.withPrecision(32);
    EXPECT_EQ(result.getPrecision(), 32);
}

// Тесты для метода toString
TEST(LongNumToStringTest, ToString) {
    LongNum num(123.456L);
    EXPECT_EQ(num.toString(3), "123.456");
}

// Тесты для метода abs
TEST(LongNumAbsTest, Abs) {
    LongNum num(-123.456L);
    LongNum result = num.abs();
    EXPECT_EQ(result.toString(3), "123.456");
}

// Тесты для метода pow
TEST(LongNumPowTest, Pow) {
    LongNum num(2.0L);
    LongNum result = num.pow(10);
    EXPECT_EQ(result.toString(), "1024");
}

// Тесты для метода sqrt
TEST(LongNumSqrtTest, Sqrt) {
    LongNum num(16.0L);
    LongNum result = num.sqrt();
    EXPECT_EQ(result.toString(), "4");
}

// Тесты для литералов
TEST(LongNumLiteralTest, LongDoubleLiteral) {
    LongNum num = 123.456_longnum;
    EXPECT_EQ(num.toString(3), "123.456");
}

TEST(LongNumLiteralTest, UnsignedLongLongLiteral) {
    LongNum num = 1234567890_longnum;
    EXPECT_EQ(num.toString(), "1234567890");
}