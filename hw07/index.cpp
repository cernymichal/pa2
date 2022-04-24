#ifndef __PROGTEST__

#include <algorithm>
#include <any>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#endif  // __PROGTEST__

#define TYPE(x) typename x::value_type

template <typename Container, typename Comp = std::less<TYPE(Container)>>
class CIndex {
    Container _container;
    Comp _comparator;

public:
    CIndex(const Container &container, const Comp &comparator = Comp()) : _container(container), _comparator(comparator) {
    }

    std::set<size_t> search(const Container &query) const {
        auto found = std::set<size_t>();

        auto cStartIter = _container.begin();
        size_t cIndex = 0;
        for (; cStartIter != _container.end(); cStartIter++, cIndex++) {
            auto cIter = cStartIter;
            auto qIter = query.begin();

            for (; qIter != query.end() && cIter != _container.end() && _equalValues(*cIter, *qIter); cIter++, qIter++)
                ;

            if (qIter == query.end())
                found.insert(cIndex);
        }

        return found;
    }

private:
    bool _equalValues(const TYPE(Container) & a, const TYPE(Container) & b) const {
        return !_comparator(a, b) && !_comparator(b, a);
    }
};

// KMP
template <>
std::set<size_t> CIndex<std::string, std::less<char>>::search(const std::string &query) const {
    auto found = std::set<size_t>();

    if (query.length() == 0) {
        for (size_t i = 0; i < _container.length(); i++)
            found.insert(i);
        return found;
    }

    std::vector<size_t> matchTable(query.length());
    matchTable[0] = 0;
    for (size_t i = 1, j = 0; i < query.length(); i++) {
        while (!_equalValues(query[i], query[j]) && j != 0)
            j = matchTable[j - 1];

        if (_equalValues(query[i], query[j]))
            matchTable[i] = j++ + 1;
        else
            matchTable[i] = 0;
    }

    /*
    std::cout << query << " [ ";
    for (const auto &el : matchTable)
        std::cout << el << ", ";
    std::cout << "\b\b ] in " << _container << " => ";
    */

    for (size_t i = 0, j = 0; i <= _container.length(); i++) {
        if (!_equalValues(_container[i], query[j])) {
            if (j != 0) {
                j = matchTable[j - 1];
                i--;
            }
        }
        else if (++j == query.length()) {
            found.insert(i + 1 - j);
            j = matchTable[j - 1];
        }
    }

    /*
    for (const auto &el : found)
        std::cout << el << ", ";
    std::cout << "\b\b " << std::endl;
    */

    return found;
}
