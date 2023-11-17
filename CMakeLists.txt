cmake_minimum_required(VERSION 3.4.1)
project(sudoku LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(solve
   solve.cpp
   sudoku.h
   sudoku.cpp
)

add_executable(random
   random.cpp
   sudoku.h
   sudoku.cpp
)

