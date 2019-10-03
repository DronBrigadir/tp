cmake_minimum_required(VERSION 3.14)
project(task1 C)

set(CMAKE_C_STANDARD 11)

FILE(GLOB SRC project/src/*.c)

include_directories(project/include)

add_executable(task ${SRC})
