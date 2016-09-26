cmake_minimum_required(VERSION 3.0)
project(Acquisition)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DBOOST_LOG_DYN_LINK")
set(LIBS "${CMAKE_CXX_FLAGS} ")

include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${CMAKE_SOURCE_DIR}/filters)
include_directories(${CMAKE_SOURCE_DIR}/proto)

FILE(GLOB sources ${CMAKE_SOURCE_DIR}/src/*.cpp)
FILE(GLOB headers ${CMAKE_SOURCE_DIR}/lib/*.hpp)

FILE(GLOB filter_src ${CMAKE_SOURCE_DIR}/filters/*.cpp)
FILE(GLOB filter_inc ${CMAKE_SOURCE_DIR}/filters/*.hpp)

FILE(GLOB proto_src ${CMAKE_SOURCE_DIR}/proto/*.cc)
FILE(GLOB proto_inc ${CMAKE_SOURCE_DIR}/proto/*.h)

add_executable(Acquisition ${sources} ${headers} ${filter_src} ${filter_inc} ${proto_src} ${proto_inc} lib/version.hpp lib/Pipeline.hpp)
target_link_libraries (Acquisition pthread)
target_link_libraries (Acquisition hiredis)
target_link_libraries (Acquisition protobuf)
target_link_libraries (Acquisition czmq)
target_link_libraries (Acquisition zmq)
target_link_libraries (Acquisition sodium)
target_link_libraries (Acquisition boost_log_setup)
target_link_libraries (Acquisition boost_log)
target_link_libraries (Acquisition boost_system)
target_link_libraries (Acquisition boost_thread)
set_target_properties(Acquisition PROPERTIES COMPILE_FLAGS "-DOPQ_DEBUG")
