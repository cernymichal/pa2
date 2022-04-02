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

class CDate {
    // todo

public:
    CDate(int year, int month, int day) {
        // todo
    }
};

class CSupermarket {
    // todo

public:
    CSupermarket() {
        // todo
    }

    ~CSupermarket() {
        // todo
    }

    CSupermarket& store(const std::string& name, const CDate& date, int count) {
        // todo
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
};
