cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(NeuroSamples)

add_executable(NeuroSamples_Absolute absolute/absolute.cpp)

add_executable(NeuroSamples_Calculator calculator/calculator.cpp)

include_directories("../../../include")

add_executable(NeuroSamples_Not not/not.cpp)
