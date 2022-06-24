cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(PROJECT_NAME NeuroTests)

set(SOURCE_DIR src)

set(SOURCE 
  "${SOURCE_DIR}/gtest.cpp"
)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} ${SOURCE})

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

target_link_libraries(${PROJECT_NAME} gtest_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})