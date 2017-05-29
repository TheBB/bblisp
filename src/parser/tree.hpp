#pragma once

#include <iostream>

#include "ast/ast.hpp"


class SourceTree
{
public:
    SourceTree() { };
    void set_root(ASTNode *node);
    void parse(std::istream &input);
};
