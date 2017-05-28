#pragma once

#include <iostream>


class ASTNode {
public:
    virtual ~ASTNode() {};
    virtual void write(std::ostream&);
};


std::ostream& operator<<(std::ostream& out, ASTNode& node);


class NumberNode: public ASTNode {
    double value;

public:
    NumberNode(double value);
    void write(std::ostream&);
};


class SymbolNode: public ASTNode {
    std::string name;

public:
    SymbolNode(const std::string &name);
    void write(std::ostream&);
};


class StringNode: public ASTNode {
    std::string value;
    char delimiter;

public:
    StringNode(const std::string &value, char delimiter);
    void write(std::ostream&);
};
