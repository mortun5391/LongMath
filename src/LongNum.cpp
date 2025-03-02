
#include "LongNum.hpp"
#include <compare>
#include <bitset>
#include <cmath>
#include <stdexcept>
#include <limits>
#include <iostream>

constexpr unsigned BASE = 32;
constexpr unsigned DEFAULT_PRECISION = 64;

// ****** Constructors and destructors ******

LongNum::LongNum(long long number) : precision(0), isNegative(number < 0){
    if (number == 0) {
        return;
    }
    number = std::abs(number);
    while (number) {
        digits.push_back(number & UINT32_MAX);
        number >>= BASE;
    }
}

LongNum::LongNum(unsigned long long number) : precision(0){
    if (number == 0) {
        return;
    }
    while (number) {
        digits.push_back(number & UINT32_MAX);
        number >>= BASE;
    }
}

LongNum::LongNum(long double number) : LongNum(number, DEFAULT_PRECISION) {
}

LongNum::LongNum(long double number, uint32_t precision) : precision(precision), isNegative(number < 0){
    number = number < 0 ? -number : number;
    long double intPart;
    long double fracPart = std::modf(number, &intPart);
    initializeFraction();

    const long double epsilon = std::numeric_limits<long double>::epsilon();
    int i = 0;
    uint32_t fractionDigits = getFractionDigits();
    while (std::abs(fracPart) > epsilon && i < fractionDigits) {
        fracPart *= (1ULL << BASE);
        auto digit = static_cast<uint32_t>(std::trunc(fracPart));
        digits[fractionDigits - i - 1] = digit;
        fracPart -= digit;
        i++;
    }
    while (intPart >= 1.0) {
        digits.push_back(static_cast<uint32_t>(std::fmod(intPart, 1ULL << BASE)));
        intPart /= (1ULL << BASE);
    }
}

// Private methods


void LongNum::initializeFraction(void) {
    uint32_t fractionDigits = 0;
    while (32 * fractionDigits < precision) {
        digits.push_back(0);
        fractionDigits++;
    }
}

void LongNum::printDigits() {
    for (size_t i = 0; i < digits.size(); i++) {
        std::cout << digits[i] << " ";
    }
    std::cout << std::endl;
}
void LongNum::printBinaryDigits() {
    for (int i = digits.size() - 1; i >= 0; i--) {
        std::bitset<BASE> bits(digits[i]);  // 32 бита для uint32_t
        std::string binary_string = bits.to_string();
        std::cout << binary_string << " ";

    }
    std::cout << std::endl;
}


uint32_t LongNum::getFractionDigits() const {
    return (precision/BASE) + (precision % BASE != 0);
}

void LongNum::removeLeadingZeros() {
    while (digits.size() > getFractionDigits() && digits.back() == 0) {
        digits.pop_back();
    }
}

LongNum LongNum::abs(void) const {
	LongNum result = *this;
	result.isNegative = false;
	return result;
}

LongNum LongNum::pow(uint32_t power) const {
    LongNum res = 1, a = *this;
    while (power) {
        if (power & 1)
            res *= a;
        a *= a;
        power >>= 1;
    }
    return res;
}

LongNum LongNum::sqrt() const {
    if (*this < 0) {
        throw std::invalid_argument("Number is negative");
    }
    LongNum num1 = 0, num2 = 1;
    while (num1 != num2) {
        num1 = num2;
        num2 += *this / num2;
        num2 >>= 1;
    }
    return num2;
}

std::string LongNum::toString(uint32_t decimalPrecision) const {
    const LongNum base = 10;
    std::string res;
    LongNum intPart = (*this).withPrecision(0).abs();
    while (intPart != 0) {
        LongNum q = intPart / base;
        LongNum r = intPart - (q * base);
        if (r == 0) {
            res += '0';
        } else {
            res += std::to_string(r.digits.front());
        }
        intPart = q;
    }
    if (res.empty()) {
        res += '0';
    }
    if (isNegative) {
        res += '-';
    }
    std::ranges::reverse(res);
    if (decimalPrecision == 0) {
        return res;
    }
    LongNum fracPart = ((*this) - (*this).withPrecision(0)).abs();
    if (fracPart == 0) {
        return res;
    }
    res += '.';
    unsigned cnt = 0;
    while (fracPart != 0 && cnt++ < decimalPrecision) {
        fracPart *= base;
        LongNum r = fracPart.withPrecision(0);
        if (r == 0) {
            res += '0';
        } else {
            res += std::to_string(r.digits.front());
        }
        fracPart -= r;
    }
    return res;
}

// Precision

void LongNum::setPrecision(uint32_t newPrecision) {
    uint32_t newFractionDigits = newPrecision/32 + (newPrecision % 32 != 0);
    uint32_t fractionDigits = getFractionDigits();
    precision = newPrecision;
    if (newFractionDigits == fractionDigits) {
        return;
    }
    int diff = newFractionDigits - fractionDigits;
    
    if (diff > 0) {
        digits.insert(digits.begin(), diff,0);
    }
    else{
        digits.erase(digits.begin(), digits.begin() - diff);
    }
}


uint32_t LongNum::getPrecision() const {
    return precision;
}

LongNum LongNum::withPrecision(uint32_t precision) const{
    LongNum result(*this);
    result.setPrecision(precision);
    return result;
}

// Operators
LongNum& LongNum::operator=(const LongNum& other) {
    if (this != &other) {
        digits = other.digits;
        precision = other.precision;
        isNegative = other.isNegative;
    }
    return *this;
}

LongNum & LongNum::operator<<=(uint32_t shift) {
    *this = *this << shift;
    return *this;
}

LongNum & LongNum::operator>>=(uint32_t shift) {
    *this = *this >> shift;
    return *this;
}

LongNum & LongNum::operator+=(const LongNum &other) {
    *this = *this + other;
    return *this;
}

LongNum & LongNum::operator-=(const LongNum &other) {
    *this = *this - other;
    return *this;
}

LongNum & LongNum::operator*=(const LongNum &other) {
    *this = *this * other;
    return *this;
}

LongNum & LongNum::operator/=(const LongNum &other) {
    *this = *this / other;
    return *this;
}

LongNum operator<<(const LongNum &number, unsigned shift) {
    LongNum result(number);
    uint32_t newDigits = shift / 32;
    if (newDigits) {
        result.digits.insert(result.digits.begin(), newDigits, 0);
    }
    shift = shift % 32;
    if (shift) {
        uint32_t carry = 0;
        for (size_t i = newDigits; i < result.digits.size(); i++) {
            uint32_t newCarry = result.digits[i] >> (32 - shift);
            result.digits[i] <<= shift;
            result.digits[i] |= carry;
            carry = newCarry;
        }
        if (carry) {
            result.digits.push_back(carry);
        }
    }
    return result;

}

LongNum operator>>(const LongNum &number, unsigned shift) {
    LongNum result(number);
    uint32_t numDigits = shift / 32;
    if (numDigits >= result.digits.size()) {
        return (0.0_longnum).withPrecision(number.precision);
    }
    result.digits.erase(result.digits.begin(), result.digits.begin() + numDigits);
    shift = shift % 32;
    if (shift) {
        uint32_t carry = 0;
        for (int i = result.digits.size() - 1; i >= 0; i--) {
            uint32_t newCarry =  result.digits[i] << (32 - shift);
            result.digits[i] >>= shift;
            result.digits[i] |= carry;
            carry = newCarry;
        }
    }
    result.removeLeadingZeros();
    return result;
}

LongNum LongNum::operator+() const {
    return *this;
}

LongNum LongNum::operator-() const {
    LongNum res(*this);
    if (*this != 0) {  // -0 == +0
        res.isNegative ^= 1;
    }
    return res;
}

LongNum operator+(const LongNum& lnum,const LongNum& rnum) {
    if (lnum.isNegative != rnum.isNegative) {
        return lnum.isNegative ? rnum - (-lnum) : lnum - (-rnum);
    }
    uint32_t maxPrecision = std::max(lnum.precision, rnum.precision);
    int l_fractionDigits = lnum.getFractionDigits();
    int r_fractionDigits = rnum.getFractionDigits();
    LongNum result(0.0,maxPrecision);
    int maxFractionDigits = std::max(l_fractionDigits, r_fractionDigits);
    int maxSize = std::max(lnum.digits.size() - l_fractionDigits, rnum.digits.size() - r_fractionDigits) + maxFractionDigits;
    while (result.digits.size() < maxSize) {
        result.digits.push_back(0);
    }
    uint32_t carry = 0;
    for (int i = 0; i < maxSize; i++) {
        uint64_t sum = carry;
        int index1 = i - (maxFractionDigits - l_fractionDigits);
        if (index1 >= 0 && index1 < lnum.digits.size())
            sum += lnum.digits[index1];

        int index2 = i - (maxFractionDigits - r_fractionDigits);
        if (index2 >= 0 && index2 < rnum.digits.size())
            sum += rnum.digits[index2];
        carry = sum >> BASE;
        result.digits[i] = static_cast<uint32_t>(sum);
    }
    if (carry != 0) {
        result.digits.push_back(carry);
    }

    result.isNegative = lnum.isNegative;
    result.precision = maxPrecision;
    return result;
}

LongNum operator-(const LongNum &lnum,const LongNum &rnum) {
    if (lnum.isNegative || rnum.isNegative) {
        if (lnum.isNegative == rnum.isNegative) return (-rnum) - (-lnum);
        return (-rnum) + lnum;
    }
    uint32_t maxPrecision = std::max(lnum.precision, rnum.precision);
    LongNum result(0L,maxPrecision);
    int l_fractionDigits = lnum.getFractionDigits();
    int r_fractionDigits = rnum.getFractionDigits();
    int maxFractionDigits = std::max(l_fractionDigits, r_fractionDigits);
    int maxSize = std::max(lnum.digits.size() - l_fractionDigits, rnum.digits.size() - r_fractionDigits) + maxFractionDigits;
    while (result.digits.size() < maxSize) {
        result.digits.push_back(0);
    }
    std::strong_ordering ordering = lnum.absCompare(rnum);
    if (ordering == std::strong_ordering::equal) {
        result.removeLeadingZeros();
        return result;
    }
    bool  isBorrow = false;
    for (int i = 0; i < maxSize; i++) {
        int index1 = i - (maxFractionDigits - l_fractionDigits);
        int index2 = i - (maxFractionDigits - r_fractionDigits);
        uint64_t greaterDigit = 0;
        uint64_t lowerDigit = 0;
        if (ordering == std::strong_ordering::greater) {
            if (index1 >= 0 && index1 < lnum.digits.size()) greaterDigit += lnum.digits[index1];
            if (index2 >= 0 && index2 < rnum.digits.size()) lowerDigit += rnum.digits[index2];
        }
        else {
            if (index1 >= 0 && index1 < lnum.digits.size()) lowerDigit += lnum.digits[index1];
            if (index2 >= 0 && index2 < rnum.digits.size()) greaterDigit += rnum.digits[index2];
        }
        if (greaterDigit == 0 && isBorrow) {
            greaterDigit += (1ULL << 32) - 1;
            }
        else if (isBorrow) {
            greaterDigit--;
            isBorrow = false;
        }

        if (greaterDigit < lowerDigit) {
            isBorrow = true;
            greaterDigit += (1ULL << 32) - lowerDigit;
        }
        else {
            greaterDigit -= lowerDigit;
        }
        result.digits[i] = greaterDigit;
    }
    result.removeLeadingZeros();
    result.isNegative = ordering == std::strong_ordering::greater ? false : true;
    return result;
}

LongNum operator*(const LongNum& lnum,const LongNum& rnum) {
    if (lnum == 0 || rnum == 0) {
        LongNum result(0.0,std::max(lnum.precision, rnum.precision));
        return result;
    }

    LongNum result(0.0,static_cast<uint32_t>(lnum.precision + rnum.precision));
    result.digits.resize(lnum.digits.size() + rnum.digits.size());
    for (size_t i = 0; i < lnum.digits.size(); i++) {
        uint32_t carry = 0;
        for (size_t j = 0; j < rnum.digits.size() || carry; j++) {
            if (j < rnum.digits.size()) {
                const uint64_t cur = static_cast<uint64_t>(lnum.digits[i]) * rnum.digits[j] + result.digits[i + j] + carry;
                result.digits[i + j] = cur;
                carry = cur >> 32;
            } else {
                result.digits[i + j] += carry;
                carry = 0;
            }
        }
    }
    result.removeLeadingZeros();
    result.setPrecision(std::max(lnum.precision, rnum.precision));
    result.isNegative = lnum.isNegative ^ rnum.isNegative;
    return result;
}

LongNum operator/(const LongNum& lnum,const LongNum& rnum) {
    if (rnum == 0) {
        throw std::invalid_argument("Division by zero");
    }
    if (lnum == 0) {
        return (0_longnum).withPrecision(std::max(lnum.precision, rnum.precision));
    }
    uint32_t maxPrecision = std::max(lnum.precision, rnum.precision);
	uint32_t normPrecision = std::max(maxPrecision, 96U);

	LongNum l_copy = lnum.abs().withPrecision(normPrecision);

	LongNum r_copy = rnum.abs().withPrecision(normPrecision);

	l_copy <<= l_copy.getFractionDigits() * 32;

	LongNum result(0.0L, normPrecision);
	result.isNegative = lnum.isNegative ^ rnum.isNegative;
	LongNum rem(0.0L, normPrecision);

	
	for (int i = l_copy.digits.size() * 32 - 1; i >= 0; --i) {
		rem <<= 1;
        uint32_t digitIndex = i / 32;
	    uint32_t bitIndex = i % 32;
	    bool bit = (l_copy.digits[digitIndex] >> bitIndex) & 1;
		rem.digits[0] |= (bit ? 1 : 0);
		if (rem >= r_copy) {
			rem -= r_copy;
	        if (digitIndex >= result.digits.size()) {
		        result.digits.resize(digitIndex + 1, 0);
            }
	        result.digits[digitIndex] |= (1U << bitIndex);
	    }
		}
        result.setPrecision(maxPrecision);
	    result.removeLeadingZeros();  
	    return result;
}

bool LongNum::operator==(const LongNum &other) const{
    return (*this <=> other) == std::strong_ordering::equal;
}

bool LongNum::operator!=(const LongNum &other) const {
    return (*this <=> other) != std::strong_ordering::equal;
}

bool LongNum::operator<(const LongNum &other) const {
    return (*this <=> other) == std::strong_ordering::less;
}

bool LongNum::operator>(const LongNum &other) const {
    return (*this <=> other) == std::strong_ordering::greater;
}

std::strong_ordering LongNum::absCompare(const LongNum &other) const {
    if (digits.size() - getFractionDigits() != other.digits.size() - other.getFractionDigits()) {
        return digits.size() - getFractionDigits() < other.digits.size() - other.getFractionDigits() ?
            std::strong_ordering::less : std::strong_ordering::greater;
    }
    int maxSize = std::max(digits.size(), other.digits.size());
    for (int i = maxSize - 1; i >= 0; i--) {
        if (i < maxSize - digits.size()) {
            if (other.digits[i] != 0) return std::strong_ordering::less;
            continue;
        }
        if (i < maxSize - other.digits.size()) {
            if (digits[i] != 0) return std::strong_ordering::greater;
            continue;
        }
        uint32_t ldigit = digits[i - (maxSize - digits.size())];
        uint32_t rdigit = other.digits[i - (maxSize - other.digits.size())];
        if (ldigit != rdigit) {
            return ldigit >  rdigit ? std::strong_ordering::greater : std::strong_ordering::less;
        }
    }
    return std::strong_ordering::equal;
}


std::strong_ordering LongNum::operator<=>(const LongNum &other) const {
    if (isNegative != other.isNegative) {
        return isNegative ? std::strong_ordering::less : std::strong_ordering::greater;
    }
    return absCompare(other);
}

LongNum operator""_longnum(const long double number) {
    return LongNum(number, 64);
}

LongNum operator""_longnum(const unsigned long long number) {
    return LongNum(number);
}




