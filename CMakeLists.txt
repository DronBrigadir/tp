cmake_minimum_required(VERSION 3.9)
project(task C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-std=c99 -ggdb3 -Wall")

include_directories(project/include)

add_executable(task ./project/src/main.c)