cmake_minimum_required(VERSION 3.6)
project(FileSystem)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -O3")

set(SOURCE_FILES FileSystem.c FileSystem.h main.c)
add_executable(FileSystem ${SOURCE_FILES})