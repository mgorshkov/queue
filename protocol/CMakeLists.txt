cmake_minimum_required(VERSION 3.5)

add_library(protocol
    Message.cpp
    Message.h
    ProtocolSerializer.cpp
    ProtocolSerializer.h)

set_target_properties(protocol PROPERTIES
    CMAKE_CXX_STANDARD 14
    CMAKE_CXX_STANDARD_REQUIRED ON
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set_target_properties(protocol PROPERTIES
        COMPILE_OPTIONS -Wpedantic -Wall -Wextra
    )
    add_definitions(-fPIC)
endif()
