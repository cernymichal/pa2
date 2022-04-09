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
            int count;

            DateCountPair(const CDate& date) : date(date) {
            }

            DateCountPair(const CDate& date, int count) : date(date), count(count) {
            }

            bool operator<(const DateCountPair& other) const {
                return date > other.date;
            }
        };

        std::vector<DateCountPair> _stock;

    public:
        std::string name;

        Product(const std::string& name) : name(name) {
        }

        void add(const CDate& date, int count) {
            auto stockPos = _findStockIter(date);

            if (stockPos == _stock.end() || date != stockPos->date) {
                _stock.emplace(stockPos, date, count);
                return;
            }

            _stock[stockPos - _stock.begin()].count += count;
        }

        int take(int count) {
            while (count > 0 && _stock.size() > 0) {
                auto& back = _stock.back();

                int canTake = std::min(back.count, count);
                back.count -= canTake;
                count -= canTake;

                if (back.count == 0)
                    _stock.pop_back();
            }

            return count;
        }

        int expired(const CDate& date) const {
            int expiredCount = 0;

            for (int i = _stock.size() - 1; i >= 0; i--) {
                if (_stock[i].date >= date)
                    break;

                expiredCount += _stock[i].count;
            }

            return expiredCount;
        }

        bool hasStock() {
            return _stock.size() > 0;
        }

    private:
        std::vector<DateCountPair>::const_iterator _findStockIter(const CDate& date) const {
            if (_stock.size() == 0)
                return _stock.end();

            DateCountPair key(date);
            auto stockPos = std::lower_bound(_stock.begin(), _stock.end(), key);

            return stockPos;
        }
    };

    std::unordered_map<std::string, Product*> _products;

public:
    CSupermarket() {
    }

    ~CSupermarket() {
        for (auto const& el : _products)
            delete el.second;
    }

    CSupermarket& store(const std::string& name, const CDate& date, int count) {
        auto search = _products.find(name);
        Product* product;
        if (search == _products.end()) {
            product = new Product(name);
            _products[name] = product;
        }
        else
            product = search->second;

        product->add(date, count);

        return *this;
    }

    CSupermarket& sell(std::list<std::pair<std::string, int>>& shoppingList) {
        auto products = std::list<Product*>();

        for (const auto& el : shoppingList)
            products.push_back(_findProduct(el.first));

        auto iterList = shoppingList.begin();
        auto iterProducts = products.begin();
        for (; iterList != shoppingList.end(); iterList++, iterProducts++) {
            auto product = *iterProducts;

            if (!product)
                continue;

            iterList->second = product->take(iterList->second);

            if (!product->hasStock()) {
                _products.erase(product->name);
                delete product;
            }

            if (iterList->second == 0)
                iterList = --shoppingList.erase(iterList);
        }

        return *this;
    }

    std::list<std::pair<std::string, int>> expired(const CDate& date) const {
        auto products = std::list<std::pair<std::string, int>>();

        for (auto const& el : _products) {
            auto product = el.second;

            int expiredCount = product->expired(date);
            if (expiredCount == 0)
                continue;

            for (auto iter = products.begin();; iter++) {
                if (iter == products.end() || iter->second <= expiredCount) {
                    products.emplace(iter, product->name, expiredCount);
                    break;
                }
            }
        }

        return products;
    }

private:
    Product* _findProductDefinite(const std::string& name) const {
        auto search = _products.find(name);

        if (search == _products.end())
            return nullptr;

        return search->second;
    }

    Product* _findProductClose(const std::string& name) const {
        Product* product = nullptr;

        for (auto const& el : _products) {
            if (_stringCloseEnough(name, el.second->name)) {
                if (product)
                    return nullptr;

                product = el.second;
            }
        }

        return product;
    }

    Product* _findProduct(const std::string& name) const {
        auto product = _findProductDefinite(name);

        if (product)
            return product;

        return _findProductClose(name);
    }

    static bool _stringCloseEnough(const std::string& a, const std::string& b) {
        if (a.length() != b.length())
            return false;

        int errors = 0;
        for (size_t i = 0; i < a.length() && errors < 2; i++) {
            if (a[i] != b[i])
                errors++;
        }

        return errors < 2;
    }
};
