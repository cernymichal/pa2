#ifndef __PROGTEST__

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#endif /* __PROGTEST__ */

// a dummy exception class, keep this implementation
class InvalidDateException : public std::invalid_argument {
public:
    InvalidDateException()
        : std::invalid_argument("invalid date or format") {
    }
};

// date_format manipulator - a dummy implementation. Keep this code unless you implement your
// own working manipulator.
std::ios_base& (*date_format(const char* fmt))(std::ios_base& x) {
    return [](std::ios_base& ios) -> std::ios_base& { return ios; };
}

class CDate {
    // todo
};
