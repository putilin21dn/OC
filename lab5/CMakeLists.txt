cmake_minimum_required(VERSION 3.16.3)
set(CMAKE_C_STANDARD 99)
project(LR5)
include_directories(include)

add_library(dyn1 SHARED src/realisation1.c)
add_library(dyn2 SHARED src/realisation2.c)

add_executable(main1.out src/main.c include/realisation.h)
add_executable(main2.out src/main.c include/realisation.h)
target_link_libraries(main1.out dyn1)
target_link_libraries(main2.out dyn2)

add_executable(main_dyn.out src/main_dyn.c)
target_link_libraries(main_dyn.out ${CMAKE_DL_LIBS})