cmake_minimum_required(VERSION 3.27)
project(Minesweeper C)

set(CMAKE_C_STANDARD 23)

add_executable(Minesweeper main.c
        minesweeper_functions.c
        minesweeper_functions.h
)

add_subdirectory(lib/haywardstd)
target_link_libraries(Minesweeper LINK_PUBLIC HAYWARDSTD)