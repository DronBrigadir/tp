cmake_minimum_required(VERSION 3.14)
project(module2)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-std=c++0x -ggdb3 -Wall -Werror -Wpedantic")

add_executable(task1_2 1_2\ hash_table.cpp)
add_executable(task2_2 2_2\ tree_pre_order.cpp)
add_executable(task4_1 4_1\ avl_soldiers.cpp)