cmake_minimum_required(VERSION 3.10)
project(weedmate)

set(CMAKE_C_FLAGS "-O3 -s -Wall -Wextra -Wpedantic")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(weedmate
	code/main.c
	code/signals.c
	code/functions.c
	code/input/input.c
)


target_include_directories(weedmate
  PRIVATE
	${CMAKE_SOURCE_DIR}/code/include
	${CMAKE_SOURCE_DIR}/code/input
)
target_link_libraries(weedmate m)
