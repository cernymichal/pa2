#ifndef __PROGTEST__

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#endif  // __PROGTEST__

struct CDate {
    uint16_t year;
    uint8_t month;
    uint8_t day;

    CDate(uint16_t year, uint8_t month, uint8_t day) : year(year), month(month), day(day) {
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
};

class CSupermarket {
    class Product {
        struct DateCountPair {
            CDate date;
            uint16_t count;

            bool operator<(const DateCountPair& other) const {
                return date < other.date;
            }
        };

        std::priority_queue<DateCountPair> _counts;

    public:
        std::string name;

        Product(const std::string& name) : name(name) {
        }
    };

    std::vector<Product*> _productsUnordered;
    std::map<std::string, Product*> _productsDefinite;

public:
    CSupermarket() {
    }

    ~CSupermarket() {
        for (const auto& product : _productsUnordered)
            delete product;
    }

    CSupermarket& store(const std::string& name, const CDate& date, int count) {
        auto search = _productsDefinite.find(name);
        Product* product;
        if (search == _productsDefinite.end())
            _productsDefinite[name] = new Product(name);
        else
            search->second->count++;

        return *this;
    }

    CSupermarket& sell(std::list<std::pair<std::string, int>> shoppingList) {
        // todo
        return *this;
    }

    std::list<std::pair<std::string, int>> expired(const CDate& date) const {
        // todo
    }

private:
    static bool stringCloseEnough(const std::string& a, const std::string& b) {
        if (a.length() != b.length())
            return false;

        int errors = 0;
        for (int i = 0; i < a.length() && errors < 2; i++) {
            if (a[i] != b[i])
                errors++;
        }

        return errors < 2;
    }
};
