#include "tree.hpp"
#include "lexer.hpp"
#include "gen_parser.hpp"

#include <iostream>


void SourceTree::set_root(ASTNode *node) {
    std::cerr << (*node) << std::endl;
}


void SourceTree::parse(std::istream &input) {
    Lexer lexer(&input);
    bison::Parser parser(lexer, *this);
    parser.parse();
}
