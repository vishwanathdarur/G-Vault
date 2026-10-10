cmake_minimum_required(VERSION 3.18)
project(Q-Vault LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Phase 1: JSON Parser Library
add_library(json_parser 
    src/common.cpp
    src/json_parser.cpp
    include/json_parser.hpp
    include/common.hpp
)
target_include_directories(json_parser PUBLIC include)

# Phase 1: CPU Parser Executable
add_executable(q_vault
    src/main.cpp
)
target_link_libraries(q_vault PRIVATE json_parser)

# Phase 1: CPU Parser Tests
add_executable(test_parser
    tests/test_cpu_parser.cpp
)
target_link_libraries(test_parser PRIVATE json_parser)

# Compiler flags
if(MSVC)
    target_compile_options(json_parser PRIVATE /W4)
    target_compile_options(q_vault PRIVATE /W4)
else()
    target_compile_options(json_parser PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(q_vault PRIVATE -Wall -Wextra -Wpedantic)
endif()
