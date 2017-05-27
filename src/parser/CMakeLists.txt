find_package(BISON)
find_package(FLEX)

BISON_TARGET(parser
  ${CMAKE_CURRENT_SOURCE_DIR}/grammar.yy
  ${CMAKE_CURRENT_SOURCE_DIR}/gen_parser.cpp)

FLEX_TARGET(lexer
  ${CMAKE_CURRENT_SOURCE_DIR}/lexer.ll
  ${CMAKE_CURRENT_SOURCE_DIR}/gen_lexer.cpp)

ADD_FLEX_BISON_DEPENDENCY(lexer parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(parser
  lexer.cpp
  tree.cpp
  ${BISON_parser_OUTPUTS}
  ${FLEX_lexer_OUTPUTS})

target_link_libraries(parser ast)

# Generated code has some harmless warning
set_source_files_properties(gen_lexer.cpp PROPERTIES COMPILE_FLAGS "-Wno-address")
