#include <cassert>
#include <iostream>
#include <string>
#include <tuple>
#include <typeinfo>

#define LEVEL 5

template <typename R, typename S, typename T>
struct Triplet {
    R first;
    S second;
    T third;

    friend std::ostream& operator<<(std::ostream& out, const Triplet& triplet) {
        return out << "{ " << triplet.first << ", " << triplet.second << ", " << triplet.third << " }";
    }

    bool operator==(const Triplet<R, S, T>& other) const {
        return first == other.first && second == other.second && third == other.third;
    }

    bool operator!=(const Triplet<R, S, T>& other) const {
        return !(*this == other);
    }

    bool operator<(const Triplet<R, S, T>& other) const {
        return (first < other.first && second <= other.second && third <= other.third) || (first <= other.first && second < other.second && third <= other.third) || (first <= other.first && second <= other.second && third < other.third);
    }

    bool operator<=(const Triplet<R, S, T>& other) const {
        return *this < other || *this == other;
    }

    bool operator>(const Triplet<R, S, T>& other) const {
        return (first > other.first && second >= other.second && third >= other.third) || (first >= other.first && second > other.second && third >= other.third) || (first >= other.first && second >= other.second && third > other.third);
    }

    bool operator>=(const Triplet<R, S, T>& other) const {
        return *this > other || *this == other;
    }

    template <int I>
    auto& get() {
        if constexpr (I == 0)
            return first;
        if constexpr (I == 1)
            return second;
        if constexpr (I == 2)
            return third;
    }
};

template <typename R, typename S, typename T>
Triplet<R, S, T> make_triplet(const R& first, const S& second, const T& third) {
    return {first, second, third};
}

int main() {
    using std::operator""s;

    std::cout << "Level 1: konstruktor & přístup k prvkům" << std::endl;
    Triplet<int, double, std::string> t1{3, .2, "Hello"};
    Triplet<std::string, int, Triplet<int, int, int>> tt{"3", 15, {3, 2, 1}};

    t1.first += 2;
    assert(t1.first == 5);
    assert(t1.second == 0.2);
    assert(t1.third == "Hello");

#if LEVEL >= 2
    std::cout << "Level 2: operátor <<" << std::endl;
    std::cout << t1 << std::endl;
    std::cout << tt << std::endl;
    // Očekávaný výstup:
    // { 5, 0.2, Hello }
    // { 3, 0, { 3, 2, 1 } }
#endif

#if LEVEL >= 3
    std::cout << "Level 3: make_triplet" << std::endl;
    auto t2 = make_triplet(5, .2, "Hello"s);
    assert(typeid(t1) == typeid(t2));
    std::cout << t2 << std::endl;
#endif

#if LEVEL >= 4
    std::cout << "Level 4: porovnávání" << std::endl;
    assert(t1 == t2);
    t2.second += 0.02;
    assert(t1 < t2);
    assert(t1 != t2);
    assert(t1 <= t2);
    assert(!(t1 >= t2));
    assert(!(t1 > t2));
#endif

#if LEVEL >= 5
    std::cout << "Level 5: metoda get<>" << std::endl;
    assert(t1.get<0>() == 5);
    t1.get<1>() = 1.5;
    assert(t1.second == 1.5);
    assert(t1.get<2>() == "Hello");
#endif

    std::cout << "Level " << LEVEL << " done" << std::endl;

    return EXIT_SUCCESS;
}
