cmake_minimum_required(VERSION 3.27)
project(Minesweeper C)

set(CMAKE_C_STANDARD 23)

add_executable(Minesweeper main.c
        minesweeper_functions.c
        minesweeper_functions.h
)
target_include_directories(Minesweeper PUBLIC "/Users/haywardhhayward/CLionProjects/haywardstd")
add_library(HAYWARDSTD STATIC IMPORTED)
set_target_properties(HAYWARDSTD PROPERTIES IMPORTED_LOCATION "/Users/haywardhhayward/CLionProjects/haywardstd/cmake-build-debug/libhaywardstd.a")
target_link_libraries(Minesweeper LINK_PUBLIC HAYWARDSTD)