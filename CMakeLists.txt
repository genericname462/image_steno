cmake_minimum_required(VERSION 3.3)
project(image_steno)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -march=native -Wall -Wextra -O2 -g")

include_directories(include)

add_executable(image_steno_cli image_steno/image_steno_cli.c image_steno/image_steno.c)

add_executable(image_steno_test tests/image_steno_test.c image_steno/image_steno.c)