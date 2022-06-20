#pragma once

#include <math.h>

template <typename T>
struct Vector2 {
    T x, y;

    Vector2() {
    }

    Vector2(const T& x, const T& y) : x(x), y(y) {
    }

    template <typename T1>
    operator Vector2<T1>() const {
        return {static_cast<T1>(x), static_cast<T1>(y)};
    }

    bool operator==(const Vector2<T>& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vector2<T>& other) const {
        return !(*this == other);
    }

    Vector2<T> operator+(const Vector2<T>& other) const {
        return {x + other.x, y + other.y};
    }

    Vector2<T>& operator+=(const Vector2<T>& other) {
        *this = *this + other;

        return *this;
    }

    Vector2<int> operator-(const Vector2<T>& other) const {
        return {x - other.x, y - other.y};
    }

    Vector2<T>& operator-=(const Vector2<T>& other) {
        *this = *this - other;

        return *this;
    }

    Vector2<T> operator*(double a) const {
        return {x * a, y * a};
    }

    Vector2<T>& operator*=(double a) {
        *this = *this * a;

        return *this;
    }

    Vector2<double> operator/(double a) const {
        return {x / a, y / a};
    }

    Vector2<T>& operator/=(double a) {
        *this = *this / a;

        return *this;
    }

    double length() const {
        return sqrt(x * x + y * y);
    }
};

/**
 * @brief signum of x
 *
 * @param[in] x
 */
template <typename T>
int sgn(T x) {
    if (x > T(0))
        return 1;

    if (x < T(0))
        return -1;

    return 0;
}

/**
 * @brief euklidian distance between points
 *
 * @param[in] a
 * @param[in] b
 */
template <typename T>
float distance(const Vector2<T>& a, const Vector2<T>& b) {
    return (b - a).length();
}
