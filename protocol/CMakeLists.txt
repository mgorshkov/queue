cmake_minimum_required(VERSION 3.5)

add_library(protocol
    ../common/Defines.h
    Message.h
    ProtocolSerializer.cpp
    ProtocolSerializer.h)

set_target_properties(protocol PROPERTIES
    COMPILE_OPTIONS -Wpedantic -Wall -Wextra
    CMAKE_CXX_STANDARD 14
    CMAKE_CXX_STANDARD_REQUIRED ON
)

add_definitions(-fPIC)

include_directories(${CMAKE_SOURCE_DIR}/common)
