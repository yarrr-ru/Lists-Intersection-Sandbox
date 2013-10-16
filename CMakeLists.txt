cmake_minimum_required(VERSION 2.8)

set(PROJECT sandbox)

project(${PROJECT})

SET(CMAKE_C_FLAGS "-std=c99 -pedantic -Wall -Wshadow -Wpointer-arith -Wcast-qual -Wextra -Wold-style-definition -O2")

set(SOURCES
  src/main.c
  src/structures/list.c
  src/structures/lists.c
  src/structures/timer.c
  src/intersectors/dumb.c
  src/intersectors/dumb_bs.c
  src/algorithms/binary_search.c
)

include_directories(
  include
)

add_executable(${PROJECT} ${SOURCES})