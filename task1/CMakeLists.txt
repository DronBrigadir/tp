cmake_minimum_required(VERSION 3.9)
project(task1 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-std=c99 -ggdb3 -Wall")


SET(LIB_SRC_FILES
        ./project/src/main.c
        ./project/src/matrix.c
        ./project/src/test.c
        ./project/src/utils.c)

include_directories(project/include)

add_executable(task ${LIB_SRC_FILES})

# sets GTEST_INCLUDE_DIRS and GTEST_LIBRARIES
#find_package(GTest REQUIRED)
# adds the gtest include directory
#include_directories(${GTEST_INCLUDE_DIRS})

#add_executable(tests_gtest project/src/tests_gtest.c)

# links gtest
#target_link_libraries(tests_gtest ${GTEST_LIBRARIES})