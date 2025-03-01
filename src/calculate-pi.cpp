#include "LongNum.hpp"
#include <iostream>
#include <stdexcept>

LongNum calculate_pi(const uint32_t precision) {
    // https://www.craig-wood.com/nick/articles/pi-chudnovsky/


    LongNum k = 1._longnum;
    LongNum a_k = (1._longnum).withPrecision(precision);
    LongNum a_sum = (1.0_longnum).withPrecision(precision);
    LongNum b_sum = 0._longnum;
    const LongNum C = 640320._longnum;
    const LongNum C3_OVER_24 = C.pow(3) / 24._longnum;
    int  iteration = 0;
    while (a_k != 0._longnum) {
        a_k *= -(6._longnum * k - 5._longnum) * (2._longnum * k - 1._longnum) * (6._longnum* k - 1._longnum);
        a_k /= k.pow(3) * C3_OVER_24;
        a_sum += a_k;
        b_sum += k * a_k;
        k += 1._longnum;
    }
    LongNum total = a_sum * 13591409._longnum + b_sum * 545140134._longnum;
    LongNum pi = (426880._longnum * (10005._longnum).withPrecision(precision).sqrt()) / total;
    return pi;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Specify calculation precision by providing a command line argument\n";
        return 1;
    }
    const std::string arg = argv[1];
    try {
        const int precision = std::stoi(arg);
        if (precision <= 0) {
            std::cerr << "Precision must be a positive\n";
            return 1;
        }
        const LongNum pi = calculate_pi(std::max(380u, (unsigned)precision * 69 / 20 + 1));
        std::cout << pi.toString(precision) << '\n';
        return 0;
    } catch (const std::invalid_argument& ex) {
        std::cerr << "Wrong precision: " << arg << '\n';
    } catch (const std::out_of_range& ex) {
        std::cerr << "Precision out of range: " << arg << '\n';
    }
}