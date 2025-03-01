#include "gtest/gtest.h"
#include "src/LongNum.hpp"
#include <limits>
#include <cmath>


TEST(LongNumTest, DefaultConstructor) {
    LongNum num(5.5L);
    EXPECT_EQ(num.toString(1), "5.5");
}



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
    EXPECT_EQ(result.toString(0), "877");
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


TEST(LongNumTest, WithPrecision) {
    LongNum a(123.456789L);
    LongNum result = a.withPrecision(3);
    EXPECT_EQ(result.toString(3), "123.456");
}

// Тест для метода toString
TEST(LongNumTest, ToString) {
    LongNum a(123.456L);
    EXPECT_EQ(a.toString(2), "123.45");
}

TEST(LongNumTest, LongDoubleLiteral) {
    LongNum num = 123.456_longnum;
    EXPECT_EQ(num.toString(3), "123.456");
}

TEST(LongNumTest, MinMaxValues) {
    LongNum min_val(std::numeric_limits<long double>::min());
    LongNum max_val(std::numeric_limits<long double>::max());

    EXPECT_GT(max_val, min_val);
    EXPECT_LT(min_val, max_val);
}

TEST(LongNumTest, Zero) {
    LongNum zero(0.0L);
    EXPECT_EQ(zero.toString(), "0");
    EXPECT_EQ(zero + zero, zero);
    EXPECT_EQ(zero * zero, zero);
    EXPECT_EQ(zero / LongNum(1.0L), zero);
}

TEST(LongNumTest, AdditionWithZero) {
    LongNum a(123.456L);
    LongNum zero(0.0L);
    EXPECT_EQ(a + zero, a);
    EXPECT_EQ(zero + a, a);
}

TEST(LongNumTest, SubtractionWithZero) {
    LongNum a(123.456L);
    LongNum zero(0.0L);
    EXPECT_EQ(zero - a, -a);  // Проверяем, что zero - a == -a
    EXPECT_EQ(a - zero, a); 
}

TEST(LongNumTest, MultiplicationWithZero) {
    LongNum a(123.456L);
    LongNum zero(0.0L);
    EXPECT_EQ(a * zero, zero);
    EXPECT_EQ(zero * a, zero);
}

TEST(LongNumTest, DivisionByZero) {
    LongNum a(123.456L);
    LongNum zero(0.0L);
    EXPECT_THROW(a / zero, std::invalid_argument);  // Ожидаем исключение
}

TEST(LongNumTest, AdditionWithNegative) {
    LongNum a(123.456L);
    LongNum b(-456.789L);
    EXPECT_EQ((a + b).toString(3), "-333.333");
}

TEST(LongNumTest, SubtractionWithNegative) {
    LongNum a(123.456L);
    LongNum b(-456.789L);
    EXPECT_EQ((a - b).toString(3), "580.245");
}

TEST(LongNumTest, MultiplicationWithNegative) {
    LongNum a(123.456L);
    LongNum b(-456.789L);
    EXPECT_EQ((a * b).toString(3), "-56393.342");
}

TEST(LongNumTest, DivisionWithNegative) {
    LongNum a(123.456L);
    LongNum b(-456.789L);
    EXPECT_EQ(a / b, LongNum(123.456L / -456.789L));
}

TEST(LongNumTest, WithPrecisionZero) {
    LongNum a(123.456789L);
    LongNum result = a.withPrecision(0);
    EXPECT_EQ(result.toString(), "123");
}

TEST(LongNumTest, ToStringZeroPrecision) {
    LongNum a(123.456L);
    EXPECT_EQ(a.toString(0), "123");
}

TEST(LongNumTest, AdditionLargeNumbers) {
    LongNum a(1e20L);
    LongNum b(2e20L);
    EXPECT_EQ(a + b, LongNum(3e20L));
}

TEST(LongNumTest, MultiplicationLargeNumbers) {
    LongNum a(1e20L);
    LongNum b(2e20L);
    EXPECT_EQ(a * b, LongNum(2e40L));
}

TEST(LongNumTest, AdditionSmallNumbers) {
    LongNum a(1e-20L);
    LongNum b(2e-20L);
    EXPECT_EQ(a + b, LongNum(3e-20L));
}

TEST(LongNumTest, MultiplicationSmallNumbers) {
    LongNum a(1e-20L);
    LongNum b(2e-20L);
    EXPECT_EQ(a * b, LongNum(2e-40L));
}

TEST(LongNumTest, ComparisonWithNegative) {
    LongNum a(-123.456L);
    LongNum b(0.0L);
    EXPECT_LT(a, b);
    EXPECT_GT(b, a);
}

TEST(LongNumTest, ComparisonCloseNumbers) {
    LongNum a(123.456L);
    LongNum b(123.4560001L);
    EXPECT_LT(a, b);
    EXPECT_GT(b, a);
}


