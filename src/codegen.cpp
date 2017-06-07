#include <iostream>
#include <sstream>

#include "llvm/ExecutionEngine/MCJIT.h"

#include "codegen.hpp"


static llvm::LLVMContext __context__;


CodeGen::CodeGen()
{
    module = std::unique_ptr<llvm::Module>(new llvm::Module("Kaleidoscope", __context__));
    builder = new llvm::IRBuilder<>(module->getContext());
}


llvm::LLVMContext& CodeGen::context() { return __context__; }


bool CodeGen::compile()
{
    std::string error;
    engine = llvm::EngineBuilder(std::move(module)).setErrorStr(&error).create();
    if (!engine) {
        std::cerr << "Error: " << error << std::endl;
        return false;
    }

    engine->finalizeObject();
    return true;
}


std::string CodeGen::anonymous_name()
{
    std::ostringstream os;
    os << "//anonymous_" << counter;
    counter++;
    return os.str();
}
