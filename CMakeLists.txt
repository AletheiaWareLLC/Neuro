cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(PROJECT_NAME Neuro)

project(${PROJECT_NAME} VERSION 0.0.1)

set(CMAKE_STATIC_LIBRARY_PREFIX "")

add_subdirectory("src")

enable_testing()
add_subdirectory("test/src")
