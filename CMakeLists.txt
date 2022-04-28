cmake_minimum_required(VERSION 3.21)
project(Microdb)

set(CMAKE_CXX_STANDARD 20)

add_executable(microdb main.cpp idx.h db.h db.cpp)
add_executable(performance_test test.cpp idx.h db.h db.cpp)
