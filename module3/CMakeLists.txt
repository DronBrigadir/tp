cmake_minimum_required(VERSION 3.14)
project(module3)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-std=c++0x -ggdb3 -Wall")

include_directories(./task1/includes)

set(TASK1_SRC
        ./task1/src/ListGraph.cpp
        ./task1/src/main.cpp
        ./task1/src/Utils.cpp
        ./task1/src/MatrixGraph.cpp
        ./task1/src/ArcGraph.cpp
        ./task1/src/SetGraph.cpp)

add_executable(task1 ${TASK1_SRC})
add_executable(task2 ./num_of_min_paths.cpp)
add_executable(task3 ./cities.cpp)