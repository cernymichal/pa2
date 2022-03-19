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
    int year;
    int month;
    int day;

public:
    CDate(int year, int month, int day) : year(year), month(month), day(day) {
        if (!valid())
            throw InvalidDateException();
    }

    CDate operator+(int days) const {
        auto date = *this;

        date.day += days;
        date.normalize();

        return date;
    }

    CDate operator-(int days) const {
        return *this + -days;
    }

    int operator-(const CDate& other) const {
        auto date = *this;

        int i = 0;
        int d = date < other ? 1 : -1;
        for (; date != other; date = date + d, i++)
            ;

        return i;
    }

    CDate operator++() {
        day++;
        normalize();
        return *this;
    }

    CDate operator++(int) {
        auto temp = *this;
        ++*this;
        return temp;
    }

    CDate operator--() {
        day--;
        normalize();
        return *this;
    }

    CDate operator--(int) {
        auto temp = *this;
        --*this;
        return temp;
    }

    bool operator==(const CDate& other) const {
        return year == other.year && month == other.month && day == other.day;
    }

    bool operator!=(const CDate& other) const {
        return !(*this == other);
    }

    bool operator>(const CDate& other) const {
        return year > other.year || (year == other.year && month > other.month) || (year == other.year && month == other.month && day > other.day);
    }

    bool operator>=(const CDate& other) const {
        return *this > other || *this == other;
    }

    bool operator<(const CDate& other) const {
        return year < other.year || (year == other.year && month < other.month) || (year == other.year && month == other.month && day < other.day);
    }

    bool operator<=(const CDate& other) const {
        return *this < other || *this == other;
    }

    friend std::ostream& operator<<(std::ostream& stream, const CDate& date);
    friend std::istream& operator>>(std::istream& stream, CDate& date);

private:
    static bool isLeapYear(int year) {
        if (year % 4 == 0) {
            if (year % 100 == 0) {
                if (year % 400 == 0) {
                    return true;
                }
                return false;
            }
            return true;
        }
        return false;
    }

    bool isLeapYear() {
        return isLeapYear(year);
    }

    static int daysInMonth(int month, int year) {
        if (month == 2) {
            if (isLeapYear(year))
                return 29;

            return 28;
        }

        if (month > 7)
            month++;

        if (month % 2 == 0)
            return 30;

        return 31;
    }

    int daysInMonth() const {
        return daysInMonth(month, year);
    }

    bool valid() const {
        return month >= 1 && month <= 12 && day >= 1 && day <= daysInMonth(month, year);
    }

    void normalize() {
        while (!valid()) {
            auto monthDays = daysInMonth();
            if (day > monthDays) {
                day -= monthDays;
                month++;
                normalizeMonths();
            }
            else if (day < 1) {
                month--;
                normalizeMonths();
                day += daysInMonth();
            }
        }
    }

    void normalizeMonths() {
        if (month > 12) {
            year += month / 12;
            month %= 12;
        }
        else if (month < 0) {
            year += month / 12 - 1;
            month %= 12;
        }

        if (month == 0) {
            year--;
            month = 12;
        }
    }
};

std::ostream& operator<<(std::ostream& stream, const CDate& date) {
    stream << date.year;
    stream << '-' << std::setw(2) << std::setfill('0') << date.month;
    stream << '-' << std::setw(2) << std::setfill('0') << date.day;
    return stream;
}

std::istream& operator>>(std::istream& stream, CDate& date) {
    auto temp = date;

    if (!(stream >> temp.year) || stream.get() != '-' || !(stream >> temp.month) || stream.get() != '-' || !(stream >> temp.day) || !temp.valid())
        stream.setstate(std::ios::failbit);
    else
        date = temp;

    return stream;
}