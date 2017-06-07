#include <iostream>
#include <sstream>
#include <string>

#include "llvm/Support/TargetSelect.h"
#include "llvm/IR/Function.h"

#include "codegen.hpp"
#include "parser/tree.hpp"


int main(int argc, char **argv)
{
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    SourceTree tree;

    std::string input;
    std::cerr << "ready> ";
    while (std::getline(std::cin, input, '\n')) {
        std::istringstream iss(input);
        tree.parse(iss);

        CodeGen generator;
        for (auto node : tree.nodes)
            node->codegen(generator);
        llvm::Function *func = static_cast<llvm::Function*>(tree.final_node->codegen(generator));
        generator.module->dump();
        if (func && func->getName().startswith(llvm::StringRef("//anonymous"))) {
            generator.compile();
            void *func_ptr = generator.engine->getPointerToFunction(func);
            double (*to_run)() = (double(*)())(func_ptr);
            std::cerr << "result> " << to_run() << std::endl;
        }
        std::cerr << "ready> ";
    }

    return 0;
}
