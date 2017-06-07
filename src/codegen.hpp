#pragma once


#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"


class CodeGen
{
private:
    int counter = 0;

public:
    std::unique_ptr<llvm::Module> module;
    llvm::IRBuilder<> *builder;
    llvm::ExecutionEngine *engine;

    CodeGen();
    llvm::LLVMContext& context();
    bool compile();
    std::string anonymous_name();
};
