#include <iostream>

#include "ast.hpp"


std::ostream& operator<<(std::ostream& out, ASTNode& node) {
    node.write(out);
    return out;
}


void ASTNode::write(std::ostream& out) {
    out << "<node>";
}


NumberNode::NumberNode(double value) : value(value) {}


void NumberNode::write(std::ostream& out) {
    out << "<number " << value << ">";
}


SymbolNode::SymbolNode(const std::string &name) : name(name) {}


void SymbolNode::write(std::ostream& out) {
    out << "<symbol '" << name << "'>";
}


StringNode::StringNode(const std::string& value, char delimiter)
    : value(value), delimiter(delimiter) {}


void StringNode::write(std::ostream& out) {
    out << "<string " << delimiter << value << delimiter << ">";
}
