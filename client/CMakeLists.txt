cmake_minimum_required(VERSION 3.5)

add_executable(queue_client
    Client.cpp
    Client.h
    ClientMain.cpp)

set_target_properties(queue_client PROPERTIES
    CMAKE_CXX_STANDARD 14
    CMAKE_CXX_STANDARD_REQUIRED ON
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set_target_properties(queue_client PROPERTIES
        COMPILE_OPTIONS -Wpedantic -Wall -Wextra
    )
endif()

include_directories(${Boost_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/common ${CMAKE_SOURCE_DIR}/protocol ${CMAKE_SOURCE_DIR}/clientapi)

target_link_libraries(queue_client
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    protocol
    clientapi
)

install(TARGETS queue_client DESTINATION bin)
