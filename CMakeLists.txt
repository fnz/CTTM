cmake_minimum_required(VERSION 3.4)

project(CTTM)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -ftemplate-depth-100000000")

include_directories(".")

add_executable(2_state_busy_beaver examples/2_state_busy_beaver.cpp)
add_executable(3_state_busy_beaver examples/3_state_busy_beaver.cpp)
add_executable(4_state_busy_beaver examples/4_state_busy_beaver.cpp)
add_executable(5_state_busy_beaver examples/5_state_busy_beaver.cpp)
add_executable(binary_increment examples/binary_increment.cpp)
add_executable(first examples/first.cpp)
add_executable(inverse examples/inverse.cpp)