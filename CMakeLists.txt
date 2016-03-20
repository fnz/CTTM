cmake_minimum_required(VERSION 3.4)

project(CTTM)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(2_state_busy_beaver 2_state_busy_beaver.cpp)
add_executable(3_state_busy_beaver 3_state_busy_beaver.cpp)
add_executable(4_state_busy_beaver 4_state_busy_beaver.cpp)
add_executable(binary_increment binary_increment.cpp)
add_executable(first first.cpp)
add_executable(inverse inverse.cpp)