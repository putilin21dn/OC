cmake_minimum_required(VERSION 3.24.2)


project(LR6-8)
find_package(Threads)
include_directories(include)
add_executable(client src/client.cpp)
target_link_libraries(client ${CMAKE_THREAD_LIBS_INIT})
add_executable(server src/server.cpp)
target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT})