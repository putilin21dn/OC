cmake_minimum_required(VERSION 3.16.3)

project(LR3)
find_package(Threads)
add_executable(main.out src/main.cpp)
target_link_libraries(main.out ${CMAKE_THREAD_LIBS_INIT})
