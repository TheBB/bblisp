#include <iostream>

#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"

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


llvm::Value *NumberNode::codegen(CodeGen& generator)
{
    return llvm::ConstantFP::get(generator.context(), llvm::APFloat(value));
}


SymbolNode::SymbolNode(const std::string &name) : name(name) {}


void SymbolNode::write(std::ostream& out)
{
    out << "{sym " << name << "}";
}


llvm::Value *SymbolNode::codegen(CodeGen& generator)
{
    return nullptr;
}


llvm::Twine SymbolNode::twine()
{
    return llvm::Twine(name);
}


llvm::StringRef SymbolNode::stringref()
{
    return llvm::StringRef(name);
}


std::string SymbolNode::str()
{
    return name;
}


StringNode::StringNode(const std::string& value, char delimiter)
    : value(value), delimiter(delimiter) {}


void StringNode::write(std::ostream& out)
{
    out << "{str " << delimiter << value << delimiter << "}";
}


llvm::Value *StringNode::codegen(CodeGen& generator)
{
    return nullptr;
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


llvm::Value *BinaryNode::codegen(CodeGen& generator)
{
    return nullptr;
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


llvm::Value *UnaryNode::codegen(CodeGen& generator)
{
    return nullptr;
}


CallNode::CallNode(SymbolNode *name, std::vector<ASTNode*> &arguments)
    : name(name), arguments(arguments) {}


void CallNode::write(std::ostream& out)
{
    out << "{call " << (*name);
    bool first = true;
    for (auto argument : arguments) {
        if (!first)
            out << ", ";
        out << (*argument);
        first = false;
    }
    out << "}";
}


llvm::Value *CallNode::codegen(CodeGen& generator)
{
    llvm::Function *callee = generator.module->getFunction(name->stringref());

    std::vector<llvm::Value*> arg_values;
    for (auto argument : arguments)
        arg_values.push_back(argument->codegen(generator));

    return generator.builder->CreateCall(callee, arg_values);
}


BranchNode::BranchNode(ASTNode *condition, ASTNode *true_br, ASTNode *false_br)
    : condition(condition), true_br(true_br), false_br(false_br) {}


void BranchNode::write(std::ostream& out)
{
    out << "{if " << (*condition) << " " << (*true_br) << " " << (*false_br) << "}";
}


llvm::Value *BranchNode::codegen(CodeGen& generator)
{
    return nullptr;
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


llvm::Value *LoopNode::codegen(CodeGen& generator)
{
    return nullptr;
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


llvm::Value *LetNode::codegen(CodeGen& generator)
{
    return nullptr;
}


PrototypeNode::PrototypeNode(SymbolNode *name, const std::vector<SymbolNode*> &arguments)
    : name(name), arguments(arguments) {}


void PrototypeNode::write(std::ostream& out) {
    out << "{prt ";
    if (name)
        out << (*name) << " ";
    out << "[";

    bool first = true;
    for (auto arg : arguments) {
        if (!first)
            out << ", ";
        out << (*arg);
        first = false;
    }

    out << "]}";
}


llvm::Function *PrototypeNode::codegen(CodeGen& generator)
{
    llvm::Type *dbl = llvm::Type::getDoubleTy(generator.context());
    std::vector<llvm::Type*> doubles(arguments.size(), dbl);
    llvm::FunctionType *ftype = llvm::FunctionType::get(dbl, doubles, false);

    if (name)
        temp_name = name->str();
    else
        temp_name = generator.anonymous_name();

    llvm::Function *func = llvm::Function::Create(
        ftype, llvm::Function::ExternalLinkage, llvm::Twine(temp_name), generator.module.get()
    );

    auto vals = func->arg_begin();
    for (auto arg : arguments)
        (vals++)->setName(arg->twine());

    return func;
}


FunctionNode::FunctionNode(PrototypeNode *prototype, ASTNode *body)
    : prototype(prototype), body(body) {}


void FunctionNode::write(std::ostream& out) {
    out << "{def " << (*prototype) << " " << (*body) << "}";
}


llvm::Value *FunctionNode::codegen(CodeGen& generator)
{
    llvm::Function *func = prototype->codegen(generator);
    llvm::BasicBlock *blk = llvm::BasicBlock::Create(generator.context(), "entry", func);
    generator.builder->SetInsertPoint(blk);

    llvm::Value *retval = body->codegen(generator);
    generator.builder->CreateRet(retval);

    return func;
}
