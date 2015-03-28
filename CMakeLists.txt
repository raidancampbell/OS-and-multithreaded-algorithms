cmake_minimum_required(VERSION 2.8.4)
project(asn1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES PR03/src/main.c)
add_executable(asn1 ${SOURCE_FILES} PR03/src/main.c)