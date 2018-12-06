cmake_minimum_required(VERSION 3.12)
project(Lab_2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Lab_2
        src/main.cpp
        src/Lex.h
        src/Syntax.h src/G.h src/GTrie.h)