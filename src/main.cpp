#include <iostream>
#include <sstream>
#include <string>

#include "parser/tree.hpp"


int main(int argc, char **argv)
{
    std::string input;
    std::cerr << "ready> ";
    while (std::getline(std::cin, input, '\n')) {
        std::istringstream iss(input);

        SourceTree tree;
        tree.parse(iss);
        std::cerr << "ready> ";
    }

    return 0;
}
