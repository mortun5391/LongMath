
#include "LongNum.hpp"
#include <bitset>
#include <cmath>
#include <stdexcept>
#include <limits>
#include <iostream>

constexpr unsigned BASE = 32;
constexpr unsigned DEFAULT_PRECISION = 64;

// ****** Constructors and destructors ******

LongNum::LongNum(uint32_t precision) :  precision(precision) {
    initializeFraction();
    digits.push_back(0);
}

LongNum::LongNum(unsigned long long number) {
    if (number == 0) {
        return;
    }
    while (number) {
        digits.push_back(static_cast<uint32_t>(number % (1ULL << BASE)));
        number >>= BASE;
    }
}

LongNum::LongNum(long long number) {
    if (number == 0) {
        return;
    }
    isNegative = number < 0;
    number = std::abs(number);
    while (number) {
        digits.push_back(static_cast<uint32_t>(number % (1ULL << BASE)));
        number >>= BASE;
    }
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
    if (digits.size() == getFractionDigits()) {
        digits.push_back(0);
    }
}
LongNum::LongNum(const LongNum& other) :
    digits(other.digits), precision(other.precision), isNegative(other.isNegative) {}

LongNum::~LongNum() {}

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

LongNum LongNum::withPrecision(uint32_t precision) {
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

LongNum operator+(const LongNum& lnum,const LongNum& rnum) {
    if (lnum.isNegative != rnum.isNegative) {
        LongNum temp = rnum;
        temp.isNegative = !temp.isNegative;
        return lnum - temp;
    }
    uint32_t maxPrecision = std::max(lnum.precision, rnum.precision);
    int l_fractionDigits = lnum.getFractionDigits();
    int r_fractionDigits = rnum.getFractionDigits();
    LongNum result(maxPrecision);
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
    uint32_t maxPrecision = std::max(lnum.precision, rnum.precision);
    LongNum result(maxPrecision);
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
    return result;
}

LongNum operator*(const LongNum& lnum,const LongNum& rnum) {
    if (lnum == 0.0_longnum || rnum == 0.0_longnum) {
        LongNum result(0,std::max(lnum.precision, rnum.precision));
        return result;
    }

    LongNum result(lnum.precision + rnum.precision);
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
    if (rnum == 0_longnum) {
        throw std::invalid_argument("Division by zero");
    }
    if (lnum == 0_longnum) {
        return (0_longnum).withPrecision(std::max(lnum.precision, rnum.precision));
    }
    LongNum l_copy(lnum);
    LongNum r_copy(rnum);

    if (lnum.precision < rnum.precision) {
        l_copy <<= 2 *rnum.precision - lnum.precision;
    } else {
        l_copy <<= rnum.precision;
    }
    if (l_copy.digits.size() < r_copy.digits.size() ||
        (l_copy.digits.size() == r_copy.digits.size() && l_copy.digits.back() < r_copy.digits.back())) {
        return (0_longnum).withPrecision(std::max(l_copy.precision, r_copy.precision));
        }
    LongNum res(std::max(lnum.precision, rnum.precision));
    if (r_copy.digits.size() == 1) {
        res = l_copy;
        uint32_t carry = 0;
        for (int i = res.digits.size() - 1; i >= 0; i--) {
            const uint64_t cur = res.digits[i] + (static_cast<uint64_t>(carry) << BASE);
            res.digits[i] = cur / rnum.digits.front();
            carry = cur - res.digits[i] * rnum.digits.front();
        }
    } else {

        // https://skanthak.hier-im-netz.de/division.html

        const size_t m = l_copy.digits.size(), n = r_copy.digits.size(); 
        const unsigned s = std::countl_zero(r_copy.digits.back());

        l_copy <<= s;
        r_copy <<= s;
        std::vector<uint32_t>& u = l_copy.digits;
        if (u.size() == m) {
            u.push_back(0);
        }
        const std::vector<uint32_t>& v = r_copy.digits;
        res.digits.resize(m - n + 1);
        std::vector<uint32_t>& q = res.digits;
        for (int j = m - n; j >= 0; j--) {
            uint64_t qhat = ((static_cast<uint64_t>(u[j + n]) << BASE) | u[j + n - 1]) / v[n - 1];
            uint64_t rhat = ((static_cast<uint64_t>(u[j + n]) << BASE) | u[j + n - 1]) % v[n - 1];
            while (rhat < (1ULL << BASE)) {
                if (qhat >= (1ULL << BASE) ||  static_cast<uint32_t>(qhat)*static_cast<uint64_t>(v[n - 2])  > ((rhat << BASE) | u[j + n - 2])) {
                    qhat--;
                    rhat += v[n - 1];
                    if (rhat >= (1ULL << BASE)) {
                        break;
                    }
                } else {
                    break;
                }
            }
            int64_t t;
            uint64_t carry = 0;
            for (size_t i = 0; i < n; i++) {
                const uint64_t prod =static_cast<uint32_t>(qhat)*static_cast<uint64_t>(v[i]);
                t = static_cast<int64_t>(u[i + j] - carry - (prod & UINT32_MAX));
                u[i + j] = t;
                carry = (prod >> BASE) - (t >> BASE);
            }
            t = static_cast<int64_t>(u[j + n] - carry);
            u[j + n] = t;

            q[j] = qhat;
            if (t < 0) {  
                q[j]--;
                carry = 0;
                for (size_t i = 0; i < n; i++) {
                    t = static_cast<int64_t>(u[i + j] + v[i] + carry);
                    u[i + j] = t;
                    carry = t >> BASE;
                }
                u[j + n] += carry;
            }
        }
    }
    res.setPrecision(std::max(lnum.precision, rnum.precision));
    res.isNegative = lnum.isNegative ^ rnum.isNegative;
    res.removeLeadingZeros();
    if (res.digits.size() == res.getFractionDigits()) {
        res.digits.push_back(0);
    }
    return res;
}


bool LongNum::operator==(const LongNum &other) const {
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
            if (other.digits[i] != 0) return isNegative ?
                std::strong_ordering::greater : std::strong_ordering::less;
        }
        if (i < maxSize - other.digits.size()) {
            if (digits[i] != 0) return isNegative ?
                std::strong_ordering::less : std::strong_ordering::greater;
        }
        if (digits[i] != other.digits[i]) {
            if (isNegative) return digits[i] > other.digits[i] ? std::strong_ordering::less : std::strong_ordering::greater;
            return digits[i] > other.digits[i] ? std::strong_ordering::greater : std::strong_ordering::less;
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
LongNum operator""_longnum(unsigned long long  number) {
    return LongNum(number);
}




