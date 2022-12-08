cmake_minimum_required(VERSION 3.24.2)
set(CMAKE_C_STANDARD 99)
project(LR5)
include_directories(include)




add_compile_definitions(C_ID="${CMAKE_CXX_COMPILER_ID}")
add_compile_definitions(C_VERSION="${CMAKE_CXX_COMPILER_VERSION}")
add_compile_definitions(C_TIME="${CMAKE_GET_CURRENT_TIME}")
add_compile_definitions(C_DATE="${CMAKE_GET_CURRENT_DATE}")



add_library(dyn1 SHARED  src/realisation1.cpp include/realisation.h) 
add_library(dyn2 SHARED src/realisation2.cpp include/realisation.h)

add_executable(main1.out src/main.cpp)
add_executable(main2.out src/main.cpp)

target_link_libraries(main1.out PRIVATE dyn1)
target_link_libraries(main2.out PRIVATE dyn2)

add_executable(main_dyn.out src/main_dyn.c)