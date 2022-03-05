#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> lines;

    std::string line;
    while (std::getline(std::cin, line))
        if (line != "") lines.push_back(line);

    std::sort(lines.begin(), lines.end());

    for (const auto& line : lines)
        std::cout << line << std::endl;

    return 0;
}