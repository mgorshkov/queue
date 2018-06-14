cmake_minimum_required(VERSION 3.5)

add_library(clientapi SHARED
    Api.h
    Api.cpp
    ConsumerApiClient.cpp
    ConsumerApiClient.h
    IApiClient.h
    IConsumerApiClient.h
    IProducerApiClient.h
    ProducerApiClient.cpp
    ProducerApiClient.h)

set(THREADS_PREFER_PTHREAD_FLAG ON)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)

set_target_properties(clientapi PROPERTIES
    COMPILE_OPTIONS -Wpedantic -Wall -Wextra
    CMAKE_CXX_STANDARD 14
    CMAKE_CXX_STANDARD_REQUIRED ON
)

include_directories(${Boost_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/common ${CMAKE_SOURCE_DIR}/protocol)

target_link_libraries(clientapi
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    protocol
)

install(TARGETS clientapi DESTINATION bin)