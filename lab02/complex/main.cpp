#include <iostream>
#include "complex.cpp"

int main()
{
    Complex a(1, 1);
    std::cout << "a: " << a << std::endl;

    Complex b(1, 0);
    std::cout << "b: " << b << std::endl;

    std::cout << "a + b = " << a + b << std::endl;
    std::cout << "a == b = " << (a == b) << std::endl;
    std::cout << "a == a = " << (a == a) << std::endl;
    std::cout << "Complex::comparison_count = " << Complex::get_comparison_count() << std::endl;

    return 0;
}