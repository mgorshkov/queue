cmake_minimum_required(VERSION 3.5)

add_executable(queue_server_test
    QueueMapTest.cpp
    QueueIntegrationTest.cpp
    Main.cpp
)

set_target_properties(queue_server_test PROPERTIES
    CMAKE_CXX_STANDARD 14
    CMAKE_CXX_STANDARD_REQUIRED ON
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set_target_properties(queue_server_test PROPERTIES
        COMPILE_OPTIONS -Wpedantic -Wall -Wextra
    )
endif()

target_link_libraries(queue_server_test
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    clientapi
    server
)

enable_testing()

add_test(queue_server_test, QueueServerTest)
