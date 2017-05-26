#include <iostream>
#include <sstream>
#include <string>

#include "parser/lexer.hpp"
#include "parser/gen_parser.hpp"


int main(int argc, char **argv) {
    std::string input;
    std::cerr << "ready> ";
    while (std::getline(std::cin, input, ';')) {
        std::istringstream iss(input);

        Lexer lexer(&iss);
        bison::Parser parser(lexer);
        parser.parse();
        // lexer.yylex();
        std::cerr << "ready> ";
    }

    return 0;
}
