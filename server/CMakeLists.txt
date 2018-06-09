cmake_minimum_required(VERSION 3.5)

add_executable(queue_server
    CommandContext.cpp
    CommandContext.h
    CommandHandler.cpp
    CommandHandler.h
    CommandExecutor.cpp
    CommandExecutor.h
    CompleteCommand.cpp
    CompleteCommand.h
    Context.cpp
    Context.h
    Consumer.cpp
    Consumer.h
    DequeueCommandHandler.cpp
    DequeueCommandHandler.h
    EnqueueCommandHandler.cpp
    EnqueueCommandHandler.h
    IConsumer.h
    IProducer.h
    IQueueManager.h
    IShrink.h
    Producer.cpp
    Producer.h
    Queue.cpp
    Queue.h
    QueueManager.cpp
    QueueManager.h
    QueueListCommandHandler.cpp
    QueueListCommandHandler.h
    QueueStorage.cpp
    QueueStorage.h
    Server.cpp
    Server.h
    ServerMain.cpp
    Session.cpp
    Session.h
    ThreadPool.cpp
    ThreadPool.h
    ThreadedActor.cpp
    ThreadedActor.h
    ThreadedConsumer.h
    ThreadedProducer.h)

set(THREADS_PREFER_PTHREAD_FLAG ON)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)

set_target_properties(queue_server PROPERTIES
    COMPILE_OPTIONS -Wpedantic -Wall -Wextra
    CMAKE_CXX_STANDARD 14
    CMAKE_CXX_STANDARD_REQUIRED ON
)

include_directories(${Boost_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/common ${CMAKE_SOURCE_DIR}/protocol)

target_link_libraries(queue_server
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    protocol
)

install(TARGETS queue_server DESTINATION bin)
