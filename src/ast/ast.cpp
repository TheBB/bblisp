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
