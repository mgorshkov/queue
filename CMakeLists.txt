cmake_minimum_required(VERSION 3.5)
project(queue VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

configure_file(version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/version.h)

set(DEBUG_PRINT "0")

if (DEBUG_PRINT)
	message("DEBUG_PRINT is activated!")
	add_definitions(-DDEBUG_PRINT)
endif()

add_executable(queue_server
	server/Context.cpp
	server/Context.h
	server/Consumer.cpp
	server/Consumer.h
	server/IConsumer.h
	server/IProducer.h
	server/IProducerApiServer.h
	server/Producer.cpp
	server/Producer.h
	server/ProducerApiServer.cpp
	server/ProducerApiServer.h
	server/Queue.cpp
	server/Queue.h
	server/Server.cpp
	server/Server.h
	server/ServerMain.cpp
	server/Session.cpp
	server/Session.h
	server/ThreadPool.cpp
	server/ThreadPool.h
	server/ThreadedActor.cpp
	server/ThreadedActor.h
	server/ThreadedConsumer.h
	server/ThreadedProducer.h)

add_executable(queue_server_test
	tests/QueueServerTest.cpp)

add_executable(queue_client
	client/Client.cpp
	client/Client.h
	client/ClientMain.cpp
	client/ConsumerApiClient.cpp
	client/ConsumerApiClient.h
	client/IApiClient.h
	client/IConsumerApiClient.h
	client/IProducerApiClient.h
	client/ProducerApiClient.cpp
	client/ProducerApiClient.h)

add_library(queue_lib
	protocol/Message.h
	protocol/ProtocolSerializer.cpp
	protocol/ProtocolSerializer.h)

set(THREADS_PREFER_PTHREAD_FLAG ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem unit_test_framework)

set_target_properties(queue_server queue_client queue_server_test PROPERTIES
	COMPILE_OPTIONS -Wpedantic -Wall -Wextra
	CMAKE_CXX_STANDARD 14
	CMAKE_CXX_STANDARD_REQUIRED ON
)

include_directories(${Boost_INCLUDE_DIR} common)

target_link_libraries(queue_server
	${Boost_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
	queue_lib
)

target_link_libraries(queue_server_test
	${Boost_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
	queue_lib
)

target_link_libraries(queue_client
	${Boost_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
	queue_lib
)

install(TARGETS queue_server DESTINATION bin)
install(TARGETS queue_client DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT mikhail.gorshkov@gmail.com)

include(CPack)

enable_testing()

add_test(queue_server_test, QueueServerTest)
