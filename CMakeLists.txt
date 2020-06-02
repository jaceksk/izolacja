cmake_minimum_required(VERSION 3.16)
project(izolacja)

set(CMAKE_CXX_STANDARD 11)

add_executable(izolacja main.cpp Map.cpp Map.h)
add_executable(izolacjaOneFile izolacja.cpp)