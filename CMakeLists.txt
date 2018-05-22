cmake_minimum_required(VERSION 3.5)
project(queue VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

configure_file(version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/version.h)

set(DEBUG_PRINT "0")

if (DEBUG_PRINT)
	message("DEBUG_PRINT is activated!")
	add_definitions(-DDEBUG_PRINT)
endif()

add_executable(join_server
	commandhandler.cpp
	commandhandler.h
	commandexecutor.cpp
	commandexecutor.h
	context.cpp
	context.h
	insertcommandhandler.cpp
	insertcommandhandler.h
	intersectcommandhandler.cpp
	intersectcommandhandler.h
	server.cpp
	server.h
	server_main.cpp
	session.cpp
	session.h
	symdiffcommandhandler.cpp
	symdiffcommandhandler.h
	truncatecommandhandler.cpp
	truncatecommandhandler.h
	utils.cpp
	utils.h)

add_executable(join_server_test
	tablemanager_test.cpp)

add_executable(join_client
	client.cpp
	client.h
	client_main.cpp)

add_library(join_server_lib
	tablemanager.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem unit_test_framework)

set_target_properties(join_server join_client join_server_test PROPERTIES
	COMPILE_OPTIONS -Wpedantic -Wall -Wextra
	CMAKE_CXX_STANDARD 14
	CMAKE_CXX_STANDARD_REQUIRED ON
	INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(join_server
	${Boost_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
	join_server_lib
)

target_link_libraries(join_server_test
	${Boost_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
	join_server_lib
)

target_link_libraries(join_client
	${Boost_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
)

install(TARGETS join_server DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT mikhail.gorshkov@gmail.com)

include(CPack)

enable_testing()

add_test(join_server_test, tablemanager_test)
