cmake_minimum_required(VERSION 3.5)

add_library(server
    CommandContext.cpp
    CommandContext.h
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
    IContextHandler.h
    IProducer.h
    IQueueManager.h
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
    Session.cpp
    Session.h
    StartSessionCommandHandler.cpp
    StartSessionCommandHandler.h
    ThreadPool.cpp
    ThreadPool.h
    Worker.cpp
    Worker.h)

target_link_libraries(server
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    protocol
)

set_target_properties(server PROPERTIES
    CMAKE_CXX_STANDARD 14
    CMAKE_CXX_STANDARD_REQUIRED ON
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set_target_properties(server PROPERTIES
        COMPILE_OPTIONS -Wpedantic -Wall -Wextra
    )
endif()
