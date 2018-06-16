cmake_minimum_required(VERSION 3.5)

add_library(protocol
    ../common/Defines.h
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

macro(get_WIN32_WINNT version)
    if (WIN32 AND CMAKE_SYSTEM_VERSION)
        set(ver ${CMAKE_SYSTEM_VERSION})
        string(REPLACE "." "" ver ${ver})
        string(REGEX REPLACE "([0-9])" "0\\1" ver ${ver})

        set(${version} "0x${ver}")
    endif()
endmacro()

get_WIN32_WINNT(ver)
if (${ver})
    add_definitions(-D_WIN32_WINNT=${ver})
endif()

include_directories(${CMAKE_SOURCE_DIR}/common)
