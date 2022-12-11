cmake_minimum_required(VERSION 3.22)
project(libpoly)

set(CMAKE_CXX_STANDARD 20)
set(COMPILE_FLAGS "${COMPILE_FLAGS} -Wall -Wextra -Werror")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist)

include_directories(include)

add_library(poly STATIC src/poly/data.cpp src/poly/property.cpp)

add_executable(test src/test.cpp)
target_link_libraries(test poly)