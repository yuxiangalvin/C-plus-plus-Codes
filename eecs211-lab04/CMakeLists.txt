cmake_minimum_required(VERSION 3.3)
project(lab4 CXX)
include(.eecs211/CMakeLists.txt)

add_program(lab4 lab4.cpp circus.cpp)

add_test_program(circus_test circus_test.cpp circus.cpp)
