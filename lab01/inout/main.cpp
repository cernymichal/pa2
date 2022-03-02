#include <string>
#include <iostream>
#include <iomanip>

int main()
{
    int x;
    std::string c;

    std::cout << "Zadej cislo a slovo:" << std::endl;

    if (std::cin >> x >> std::setw(12) >> c)
        std::cout << "Zadal jsi: " << x << " " << c << std::endl;
    else
        std::cout << ":(" << std::endl;

    return 0;
}