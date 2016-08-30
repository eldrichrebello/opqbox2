cmake_minimum_required(VERSION 3.0)
project(Acquisition)

set(LIBS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${CMAKE_SOURCE_DIR}/filters)

FILE(GLOB sources ${CMAKE_SOURCE_DIR}/src/*.cpp)
FILE(GLOB headers ${CMAKE_SOURCE_DIR}/lib/*.hpp)

FILE(GLOB filter_src ${CMAKE_SOURCE_DIR}/filters/*.cpp)
FILE(GLOB filter_inc ${CMAKE_SOURCE_DIR}/filters/*.lib)

add_executable(Acquisition ${sources} ${headers} ${filter_src} ${filter_inc} filters/LowPassFilter.cpp filters/LowPassFilter.hpp)

target_link_libraries (Acquisition pthread)
target_link_libraries (Acquisition hiredis)
