#include "tree.hpp"
#include "lexer.hpp"
#include "gen_parser.hpp"

#include <iostream>


void SourceTree::add_node(FunctionNode *node)
{
    if (final_node)
        nodes.push_back(final_node);
    final_node = node;
    std::cerr << (*node) << std::endl;
}


void SourceTree::add_node(PrototypeNode *node)
{
    if (final_node)
        nodes.push_back(final_node);
    final_node = node;
    std::cerr << (*node) << std::endl;
}


void SourceTree::parse(std::istream &input)
{
    Lexer lexer(&input);
    bison::Parser parser(lexer, *this);
    parser.parse();
}
