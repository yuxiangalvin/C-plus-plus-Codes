cmake_minimum_required(VERSION 3.3)
project(lab5 CXX)
include(.eecs211/CMakeLists.txt)

add_program(lab5 lab5.cpp)

add_test_program(lab5_test lab5_test.cpp Practice.cpp ListNode.cpp Dog.cpp)
