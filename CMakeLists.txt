cmake_minimum_required(VERSION 3.0)
enable_language(CXX)
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")

project(kaleidoscope)

find_package(LLVM)
if(NOT LLVM_FOUND)
  message(FATAL_ERROR "Could not find LLVM.")
endif()

include_directories(src ${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

llvm_map_components_to_libnames(REQ_LLVM_LIBRARIES mcjit native)

add_subdirectory(src)
