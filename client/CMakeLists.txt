cmake_minimum_required(VERSION 3.5)

add_executable(queue_client
    Client.cpp
    Client.h
    ClientMain.cpp
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
find_package(Boost REQUIRED COMPONENTS system filesystem unit_test_framework)

set_target_properties(queue_client PROPERTIES
#    COMPILE_OPTIONS -Wpedantic -Wall -Wextra
    CMAKE_CXX_STANDARD 14
    CMAKE_CXX_STANDARD_REQUIRED ON
)

include_directories(${Boost_INCLUDE_DIR} common)

target_link_libraries(queue_client
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    queue_lib
)

install(TARGETS queue_client DESTINATION bin)