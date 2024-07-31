cmake_minimum_required(VERSION 3.17.0)
project(string_view_tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(string_view_tests string_view_tests.cpp string_view.h)