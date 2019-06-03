cmake_minimum_required(VERSION 3.5.1)
project(n_digit_calculator)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 11)

set(SOURCES     src/io.cpp
                src/arithmetic.cpp)

set(INCLUDES    includes/mylib.h
                includes/io.h
                includes/arithmetic.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

add_executable(n_digit_calculator main.cpp ${SOURCES} ${INCLUDES})
