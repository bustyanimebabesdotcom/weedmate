# Specify minimum version of cmake
cmake_minimum_required(VERSION 3.10)

# Project name
project(weedmate)

# Compiler
set(CMAKE_C_COMPILER gcc)

# Compiler flags
set(CMAKE_C_FLAGS "-Wall -Wextra -Wpedantic -Werror")

# Release flags
set(CMAKE_C_FLAGS_RELEASE "-O3")

# Debug flags
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -fsanitize=address -fno-omit-frame-pointer")

# C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Source files to include
include(${CMAKE_SOURCE_DIR}/sources.cmake)
add_executable(weedmate ${WEEDMATE_SOURCES})

# Include directories for namespace
target_include_directories(weedmate
  PRIVATE
	${CMAKE_SOURCE_DIR}/code/include
	${CMAKE_SOURCE_DIR}/code/input
)

# Link math library
target_link_libraries(weedmate m)
