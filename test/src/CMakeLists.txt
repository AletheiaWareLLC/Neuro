set(LIBRARY_NAME libNeuro)

set(TARGET_NAME NeuroTests)

set(SOURCE 
  "action.cpp"
  "instruction.cpp"
  "network.cpp"
  "neuron.cpp"
  "state.cpp"
)

include_directories("../../include")

add_executable(${TARGET_NAME} ${SOURCE})

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

target_link_libraries(${TARGET_NAME} ${LIBRARY_NAME} gtest_main)

include(GoogleTest)
gtest_discover_tests(${TARGET_NAME})
