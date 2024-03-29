cmake_minimum_required(VERSION 3.4.3)
project(SimpleFrontend)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Now build our tools
add_executable(mila main.cpp 
               lexer.hpp lexer.cpp 
               parser.hpp parser.cpp 
               token.hpp token.cpp 
               misc.cpp misc.hpp
               astnode.cpp astnode.hpp
               codegen.hpp codegen.cpp 
               printer.hpp printer.cpp
               visitor.hpp visitor.cpp)

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)

# Link against LLVM libraries
target_link_libraries(mila ${llvm_libs})
