#include <string>
#include <fstream>
#include <iostream>

int main()
{
    int num = 0;
    std::string file_name;

    std::cout << "Zadej nazev souboru a cislo:" << std::endl;
    if (!(std::cin >> file_name >> num))
        return -1;

    std::ofstream file(file_name);
    file << num << std::endl;

    return 0;
}