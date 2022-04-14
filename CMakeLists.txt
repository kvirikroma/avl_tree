cmake_minimum_required(VERSION 3.10)
project(avl_tree VERSION 0.0.1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
add_compile_options(-Wall -O3 -gdwarf-3)

file(GLOB_RECURSE sources "demo.c" "src/*.c" "src/**/*.c" "submodules/**/src/*.c" "submodules/**/src/**/*.c")
include_directories("include")

add_executable(avl_tree ${sources})
