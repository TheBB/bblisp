#pragma once

#include <iostream>
#include <vector>

#include "llvm/IR/Value.h"

#include "codegen.hpp"


class ASTNode
{
public:
    virtual ~ASTNode() {};
    virtual void write(std::ostream&);
    virtual llvm::Value *codegen(CodeGen&) = 0;
};


std::ostream& operator<<(std::ostream& out, ASTNode& node);


class NumberNode: public ASTNode
{
    double value;

public:
    NumberNode(double value);
    void write(std::ostream&);
    llvm::Value *codegen(CodeGen&);
};


class SymbolNode: public ASTNode
{
    std::string name;

public:
    SymbolNode(const std::string& name);
    void write(std::ostream&);
    llvm::Value *codegen(CodeGen&);
    std::string str();
    llvm::Twine twine();
    llvm::StringRef stringref();
};


class StringNode: public ASTNode
{
    std::string value;
    char delimiter;

public:
    StringNode(const std::string& value, char delimiter);
    void write(std::ostream&);
    llvm::Value *codegen(CodeGen&);
};


class BinaryNode: public ASTNode
{
public:
    enum class Operator {
        assign,
        plus,
        minus,
        gt,
        lt,
        multiply,
        divide,
    };

private:
    Operator op;
    ASTNode *lhs, *rhs;

public:
    BinaryNode(Operator op, ASTNode *lhs, ASTNode *rhs);
    void write(std::ostream&);
    llvm::Value *codegen(CodeGen&);
};


class UnaryNode: public ASTNode
{
public:
    enum class Operator {
        minus,
    };

private:
    Operator op;
    ASTNode *operand;

public:
    UnaryNode(Operator op, ASTNode *operand);
    void write(std::ostream&);
    llvm::Value *codegen(CodeGen&);
};


class CallNode: public ASTNode
{
private:
    SymbolNode *name;
    std::vector<ASTNode*> arguments;

public:
    CallNode(SymbolNode *name, std::vector<ASTNode*> &arguments);
    void write(std::ostream&);
    llvm::Value *codegen(CodeGen&);
};


class BranchNode: public ASTNode
{
    ASTNode *condition, *true_br, *false_br;

public:
    BranchNode(ASTNode *condition, ASTNode *true_br, ASTNode *false_br);
    void write(std::ostream&);
    llvm::Value *codegen(CodeGen&);
};


class LoopNode: public ASTNode
{
    SymbolNode *symbol;
    ASTNode *start, *stop, *step, *body;

public:
    LoopNode(SymbolNode*, ASTNode*, ASTNode*, ASTNode*, ASTNode*);
    void write(std::ostream&);
    llvm::Value *codegen(CodeGen&);
};


typedef std::pair<SymbolNode*, ASTNode*> binding_t;
typedef std::vector<binding_t> bindings_t;

class LetNode: public ASTNode
{
    bindings_t bindings;
    ASTNode *body;

public:
    LetNode(const bindings_t& bindings, ASTNode *body);
    void write(std::ostream&);
    llvm::Value *codegen(CodeGen&);
};


class PrototypeNode: public ASTNode
{
    SymbolNode *name;
    std::string temp_name;
    std::vector<SymbolNode*> arguments;

public:
    PrototypeNode(SymbolNode *name, const std::vector<SymbolNode*> &arguments);
    void write(std::ostream&);
    llvm::Function *codegen(CodeGen&);
};


class FunctionNode: public ASTNode
{
    PrototypeNode *prototype;
    ASTNode *body;

public:
    FunctionNode(PrototypeNode *prototype, ASTNode *body);
    void write(std::ostream&);
    llvm::Value *codegen(CodeGen&);
};
