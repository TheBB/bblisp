file(GLOB KALEIDOSCOPE_SRCS *.cpp)

add_subdirectory(ast)
add_subdirectory(parser)

add_executable(kaleidoscope ${KALEIDOSCOPE_SRCS})
target_link_libraries(kaleidoscope ${REQ_LLVM_LIBRARIES} parser)
