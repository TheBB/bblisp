add_library(ast
  ast.cpp)

target_link_libraries(ast ${REQ_LLVM_LIBRARIES})
