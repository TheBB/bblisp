#include <iostream>

#include "ast.hpp"


std::ostream& operator<<(std::ostream& out, ASTNode& node)
{
    node.write(out);
    return out;
}


void ASTNode::write(std::ostream& out)
{
    out << "{node}";
}


NumberNode::NumberNode(double value) : value(value) {}


void NumberNode::write(std::ostream& out)
{
    out << "{num " << value << "}";
}


SymbolNode::SymbolNode(const std::string &name) : name(name) {}


void SymbolNode::write(std::ostream& out)
{
    out << "{sym " << name << "}";
}


StringNode::StringNode(const std::string& value, char delimiter)
    : value(value), delimiter(delimiter) {}


void StringNode::write(std::ostream& out)
{
    out << "{str " << delimiter << value << delimiter << "}";
}


BinaryNode::BinaryNode(Operator op, ASTNode *lhs, ASTNode *rhs)
    : op(op), lhs(lhs), rhs(rhs) {}


void BinaryNode::write(std::ostream& out)
{
    out << "{bop(";
    switch (op) {
    case Operator::assign: out << "="; break;
    case Operator::plus: out << "+"; break;
    case Operator::minus: out << "-"; break;
    case Operator::gt: out << ">"; break;
    case Operator::lt: out << "<"; break;
    case Operator::multiply: out << "*"; break;
    case Operator::divide: out << "/"; break;
    }
    out << ") " << (*lhs) << " " << (*rhs) << "}";
}


UnaryNode::UnaryNode(Operator op, ASTNode *operand)
    : op(op), operand(operand) {}


void UnaryNode::write(std::ostream& out)
{
    out << "{uop(";
    switch (op) {
    case Operator::minus: out << "-"; break;
    }
    out << ") " << (*operand) << "}";
}


BranchNode::BranchNode(ASTNode *condition, ASTNode *true_br, ASTNode *false_br)
    : condition(condition), true_br(true_br), false_br(false_br) {}


void BranchNode::write(std::ostream& out)
{
    out << "{if " << (*condition) << " " << (*true_br) << " " << (*false_br) << "}";
}


LoopNode::LoopNode(SymbolNode *symbol, ASTNode *start, ASTNode *stop, ASTNode *step, ASTNode *body)
    : symbol(symbol), start(start), stop(stop), step(step), body(body) {}


void LoopNode::write(std::ostream& out)
{
    out << "{for " << (*symbol) << " " << (*start) << " " << (*stop) << " ";
    if (step)
        out << (*step) << " ";
    out << (*body) << "}";
}


LetNode::LetNode(const bindings_t& bindings, ASTNode *body)
    : bindings(bindings), body(body) {}


void LetNode::write(std::ostream& out)
{
    out << "{let [";

    bool first = true;
    for (auto binding : bindings) {
        if (!first)
            out << ", ";
        out << (*binding.first) << " = " << (*binding.second);
        first = false;
    }

    out << "] " << (*body) << "}";
}


PrototypeNode::PrototypeNode(SymbolNode *name, const std::vector<SymbolNode*> &arguments)
    : name(name), arguments(arguments) {}


void PrototypeNode::write(std::ostream& out) {
    out << "{prt " << (*name) << " [";

    bool first = true;
    for (auto arg : arguments) {
        if (!first)
            out << ", ";
        out << (*arg);
        first = false;
    }

    out << "]}";
}


FunctionNode::FunctionNode(PrototypeNode *prototype, ASTNode *body)
    : prototype(prototype), body(body) {}


void FunctionNode::write(std::ostream& out) {
    out << "{def " << (*prototype) << " " << (*body) << "}";
}
