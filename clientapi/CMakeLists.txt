cmake_minimum_required(VERSION 3.5)

add_library(clientapi SHARED STATIC
    Api.h
    Api.cpp
    ConsumerApiClient.cpp
    ConsumerApiClient.h
    IApiClient.h
    IConsumerApiClient.h
    IProducerApiClient.h
    ProducerApiClient.cpp
    ProducerApiClient.h)

set_target_properties(clientapi PROPERTIES
    CMAKE_CXX_STANDARD 14
    CMAKE_CXX_STANDARD_REQUIRED ON
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set_target_properties(clientapi PROPERTIES
        COMPILE_OPTIONS -Wpedantic -Wall -Wextra
    )
endif()

target_link_libraries(clientapi
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    protocol
)

install(TARGETS clientapi DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/clientapi/Api.h DESTINATION /usr/local/include/queue/)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/clientapi/Handle.h DESTINATION /usr/local/include/queue/)
