#pragma once

#include <iostream>

#include "ast/ast.hpp"


class SourceTree
{
public:
    SourceTree() : final_node(nullptr) {}
    void add_node(FunctionNode *node);
    void add_node(PrototypeNode *node);
    void parse(std::istream &input);

    std::vector<ASTNode*> nodes;
    ASTNode *final_node;
};
