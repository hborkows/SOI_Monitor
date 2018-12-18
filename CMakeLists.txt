cmake_minimum_required(VERSION 3.9)
project(Monitory)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(Monitory monitor.h main.cpp MyMonitor.cpp MyMonitor.h)
target_link_libraries(Monitory pthread rt)