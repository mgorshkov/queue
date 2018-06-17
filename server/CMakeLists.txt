add_executable(queue_server
    ServerMain.cpp)

set_target_properties(queue_server PROPERTIES
    CMAKE_CXX_STANDARD 14
    CMAKE_CXX_STANDARD_REQUIRED ON
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set_target_properties(queue_server PROPERTIES
        COMPILE_OPTIONS -Wpedantic -Wall -Wextra
    )
endif()

target_link_libraries(queue_server
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    server
)
