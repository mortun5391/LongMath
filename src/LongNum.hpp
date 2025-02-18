#ifndef LONGNUM_H
#define LONGNUM_H
#include <compare>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdint>


class LongNum {
private:
    std::vector<uint32_t> digits;
    bool isNegative = false;
    int precision;


    void initializeFraction(void);
    inline uint32_t getFractionDigits(void) const;
    inline void removeLeadingZeros(void);

public:
    LongNum(uint32_t precision = 64);
    LongNum(unsigned long long x);
    LongNum(long long x);
    LongNum(long double number, uint32_t precision = 64);
    LongNum(const LongNum& other);
    ~LongNum();

    
    LongNum& operator=(const LongNum& other);

    LongNum& operator<<=(uint32_t shift);
    friend LongNum operator<<(const LongNum& number, unsigned shift);
 
    LongNum& operator>>=(uint32_t shift);
    friend LongNum operator>>(const LongNum& number, unsigned shift);
 
    LongNum& operator+=(const LongNum& other);
    LongNum& operator-=(const LongNum& other);
    LongNum& operator*=(const LongNum& other);
    LongNum& operator/=(const LongNum& other);
 
    friend LongNum operator+(const LongNum& lnum,const LongNum& rnum);
    friend LongNum operator-(const LongNum& lnum,const LongNum& rnum);
    friend LongNum operator*(const LongNum& lnum,const LongNum& rnum);
    friend LongNum operator/(const LongNum& lnum,const LongNum& rnum) ;
 
 
    std::strong_ordering operator<=>(const LongNum &other) const;
    std::strong_ordering absCompare(const LongNum &other) const;
 
    bool operator==(const LongNum& other) const;
    bool operator!=(const LongNum& other) const;
    bool operator<(const LongNum& other) const;
    bool operator>(const LongNum& other) const;
 
 
     // Методы для работы с точностью
    void setPrecision(uint32_t precision);
    uint32_t getPrecision() const;
    LongNum withPrecision(uint32_t precision);
 
 
    void printDigits(void);
    void printBinaryDigits(void);
 
 };
 LongNum operator""_longnum(long double number);
 LongNum operator""_longnum(unsigned long long number);
#endif
