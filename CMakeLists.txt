cmake_minimum_required(VERSION 3.18)
project(G-Vault LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Phase 0: Hello GPU Kernel
add_executable(phase0_hello_kernel 
    src/phase0_hello.cu
)
set_target_properties(phase0_hello_kernel PROPERTIES LANGUAGE CUDA)

# Phase 1: CPU JSON Parser Library
add_library(json_parser_cpu 
    src/json_parser.cpp
    include/json_parser.h
    include/common.h
)
target_include_directories(json_parser_cpu PUBLIC include)

# Phase 1: CPU Parser Executable
add_executable(json_parser_cpu_main
    src/main.cpp
)
target_link_libraries(json_parser_cpu_main PRIVATE json_parser_cpu)

# Phase 1: CPU Parser Tests
add_executable(test_cpu_parser
    tests/test_cpu_parser.cpp
)
target_link_libraries(test_cpu_parser PRIVATE json_parser_cpu)

# Compiler flags
if(MSVC)
    target_compile_options(json_parser_cpu PRIVATE /W4)
    target_compile_options(json_parser_cpu_main PRIVATE /W4)
else()
    target_compile_options(json_parser_cpu PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(json_parser_cpu_main PRIVATE -Wall -Wextra -Wpedantic)
endif()
