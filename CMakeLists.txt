cmake_minimum_required(VERSION 3.18)
project(Q-Vault LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Core Engine Library (JSON Parser + Query Compiler)
add_library(qvault_core 
    src/common.cpp
    src/json_parser.cpp
    src/query_lexer.cpp
    include/common.hpp
    include/json_parser.hpp
    include/query_lexer.hpp
)
target_include_directories(qvault_core PUBLIC include)

# Main Executable
add_executable(q_vault
    src/main.cpp
)
target_link_libraries(q_vault PRIVATE qvault_core)

# Tests
add_executable(test_parser
    tests/test_cpu_parser.cpp
)
target_link_libraries(test_parser PRIVATE qvault_core)

add_executable(test_lexer
    tests/test_lexer.cpp
)
target_link_libraries(test_lexer PRIVATE qvault_core)

# Compiler flags
if(MSVC)
    target_compile_options(qvault_core PRIVATE /W4)
    target_compile_options(q_vault PRIVATE /W4)
else()
    target_compile_options(qvault_core PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(q_vault PRIVATE -Wall -Wextra -Wpedantic)
endif()
