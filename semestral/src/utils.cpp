template <typename T>
int sgn(T x) {
    if (x > T(0))
        return 1;

    if (x < T(0))
        return -1;

    return 0;
}
