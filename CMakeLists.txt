cmake_minimum_required(VERSION 3.22)
project(vm C)

set(CMAKE_C_STANDARD 99)

file(GLOB_RECURSE source ./src/*)

add_executable(vm ${source})
