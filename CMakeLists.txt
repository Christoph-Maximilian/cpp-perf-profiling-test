cmake_minimum_required(VERSION 3.13)
project(perfstuff)

set(CMAKE_CXX_STANDARD 14)

add_executable(perfstuff main.cpp profiling.h profiling.cpp)