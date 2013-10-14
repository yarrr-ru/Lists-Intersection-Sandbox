cmake_minimum_required(VERSION 2.8)

set(PROJECT sandbox)

project(${PROJECT})

SET(CMAKE_C_FLAGS "-std=c99 -pedantic -Wall -Wshadow -Wpointer-arith -Wcast-qual -Wextra -Wold-style-definition")

set(SOURCES
  src/main.c
)

include_directories(
  include
)

add_executable(${PROJECT} ${SOURCES})