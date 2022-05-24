#include <math.h>

template <typename T>
int sgn(T x) {
    if (x > T(0))
        return 1;

    if (x < T(0))
        return -1;

    return 0;
}

template <typename T>
float distance(T x1, T y1, T x2, T y2) {
    auto dx = x2 - x1;
    auto dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}
