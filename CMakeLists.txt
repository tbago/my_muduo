cmake_minimum_required(VERSION 3.9)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_VISIBILITY_PRESET default)

project(MyMuduo)

option(MUDUO_BUILD_EXAMPLE "Build moduo example" ON)
option(MUDUO_BUILD_TESTS "Build muduo tests" ON)

# search and src file in subdirs
aux_source_directory("./base" SRC_BASE)
aux_source_directory("./net" SRC_NET)

set(MUDUO_LIBRARY_NAME "muduo")
add_library(${MUDUO_LIBRARY_NAME} STATIC ${SRC_BASE} ${SRC_NET})

if(MUDUO_BUILD_EXAMPLE)
    set(MUDUO_ECHO_EXAMPLE "echo_server")
    include_directories("./")
    add_executable(${MUDUO_ECHO_EXAMPLE} ./example/echo_server.cc)
    target_link_libraries(${MUDUO_ECHO_EXAMPLE} PRIVATE ${MUDUO_LIBRARY_NAME})
endif(MUDUO_BUILD_EXAMPLE)

if (MUDUO_BUILD_TESTS)
    enable_testing()
endif(MUDUO_BUILD_TESTS)



