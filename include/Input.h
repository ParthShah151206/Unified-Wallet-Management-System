#ifndef UWMS_INPUT_H
#define UWMS_INPUT_H
#include <iostream>
#include <limits>
#include <cmath>

inline bool readInt(const char* prompt, int& value) {
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) return true;
        if (std::cin.eof()) return false;
        std::cout << "Invalid input. Enter a whole number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}
inline bool readPositiveAmount(const char* prompt, double& value, bool allowZero = false) {
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && std::isfinite(value) && (allowZero ? value >= 0.0 : value > 0.0)) return true;
        if (std::cin.eof()) return false;
        std::cout << "Invalid amount. Enter a finite " << (allowZero ? "non-negative" : "positive") << " number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}
inline void discardLine() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
#endif
