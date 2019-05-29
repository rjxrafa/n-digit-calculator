cmake_minimum_required(VERSION 3.13)
project(n_digit_calculator)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES src/io.cpp
            src/add.cpp)

set(INCLUDES    includes/mylib.h
                includes/io.h
                includes/add.h)

add_executable(n_digit_calculator main.cpp ${SOURCES} ${INCLUDES})