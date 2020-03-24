cmake_minimum_required(VERSION 3.15)
project(Raid C)

set(CMAKE_C_STANDARD 99)
include_directories(Common)

file(GLOB_RECURSE SOURCES Agent/*.c Common/*.c)
message(STATUS ${SOURCES})
set(Raid $(SOURCES))

add_executable(Raid ${SOURCES})