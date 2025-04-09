cmake_minimum_required(VERSION 3.10)
project(weedmate)

set(CMAKE_C_FLAGS "-O3 -s -Wall -Wextra -Wpedantic")

add_executable(weedmate 
    code/main.c 
    code/functions.c
)
target_link_libraries(weedmate m)
