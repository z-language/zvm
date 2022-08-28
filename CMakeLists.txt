cmake_minimum_required(VERSION 3.22)
project(vm C)

set(CMAKE_C_STANDARD 99)

add_executable(vm src/main.c src/stack/stack.c src/parser/parser.c)
