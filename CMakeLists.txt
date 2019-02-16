cmake_minimum_required(VERSION 3.12)
project(echo)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_CXX_COMPILER "clang++-7")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter")

add_subdirectory(generator)
add_subdirectory(library)
add_subdirectory(test)
