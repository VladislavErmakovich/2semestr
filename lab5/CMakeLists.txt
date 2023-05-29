cmake_minimum_required(VERSION 3.24)
project(lab5 C)

set(CMAKE_C_STANDARD 99)

add_executable(lab5 main.c fun.h fun.c fun_cache_hash.h fun_cache_hash.c)
