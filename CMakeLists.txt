cmake_minimum_required(VERSION 3.9)
project(task C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_FLAGS "-std=c99 -ggdb3 -Wall -Werror -Wpedantic")
set(CMAKE_CXX_FLAGS "-std=c++0x")

ADD_LIBRARY(LIS_STAT_LIB STATIC ./project/src/seq_realisation.c)

SET(SRC_FILES
        ./project/src/main.c
        ./project/src/utils.c)

include_directories(./project/include)

add_executable(task ${SRC_FILES})
TARGET_LINK_LIBRARIES(task LIS_STAT_LIB)

enable_testing()

find_package(GTest REQUIRED)
find_package(Threads)

include_directories(${GTEST_INCLUDE_DIRS})
add_executable(gtest_test project/test/main.cpp project/test/test.cpp)
TARGET_LINK_LIBRARIES(gtest_test ${GTEST_BOTH_LIBRARIES} gtest pthread LIS_STAT_LIB)