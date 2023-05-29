cmake_minimum_required(VERSION 3.24)
project(lab4 C)

set(CMAKE_C_STANDARD 99)

add_executable(lab4 main.c fun.h fun.c)

target_link_libraries(lab4 winmm)
