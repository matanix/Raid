cmake_minimum_required(VERSION 3.15)
project(Raid C)

set(CMAKE_C_STANDARD 99)
include_directories(Common)

file(GLOB_RECURSE AGENT_SOURCES Agent/*.c Common/*.c)
file(GLOB_RECURSE STATION_SOURCES Station/*.c Common/*.c)

set(RaidAgent $(AGENT_SOURCES))
set(RaidStation $(STATION_SOURCES))

add_executable(RaidAgent ${AGENT_SOURCES})
add_executable(RaidStation ${STATION_SOURCES})