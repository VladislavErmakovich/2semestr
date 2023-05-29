cmake_minimum_required(VERSION 3.24)
project (lab1_1 C)

set(CMAKE_C_STANDARD 99)

add_executable(lab5 lab5/main.c lab5/fun.c lab5/fun.h lab5/fun_cache_hash.c lab5/fun_cache_hash.h)
add_executable(lab3 lab3/main.c lab3/fun.c lab3/fun.h)
add_executable(decompressor decompressor/main.c decompressor/fun.c decompressor/fun.h)
add_executable(123456789 123456789/main.c 123456789/fun.c 123456789/fun.h)