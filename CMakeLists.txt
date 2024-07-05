cmake_minimum_required(VERSION 3.30.0)

project(Mars VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

add_subdirectory(${CMAKE_SOURCE_DIR}/engine)
add_subdirectory(${CMAKE_SOURCE_DIR}/testbed)
