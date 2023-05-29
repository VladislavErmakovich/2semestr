cmake_minimum_required(VERSION 3.24)
project(decompressor C)

set(CMAKE_C_STANDARD 99)

add_executable(decompressor main.c fun.c fun.h)
