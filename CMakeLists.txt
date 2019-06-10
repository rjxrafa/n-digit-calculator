cmake_minimum_required(VERSION 3.5.1)
project(n_digit_calculator)

set(CMAKE_CXX_STANDARD 11)

set(SOURCES     src/io.cpp
                src/math.cpp)

set(INCLUDES    includes/mylib.h
                includes/io.h
                includes/math.h)

if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++)
endif()

add_executable(n_digit_calculator main.cpp ${SOURCES} ${INCLUDES})
