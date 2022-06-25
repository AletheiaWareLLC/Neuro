cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(TARGET_NAME Neuro)

set(SOURCE_DIR src)

set(SOURCES
  "${SOURCE_DIR}/action.cpp"
  "${SOURCE_DIR}/emit.cpp"
  "${SOURCE_DIR}/instruction.cpp"
  "${SOURCE_DIR}/main.cpp"
  "${SOURCE_DIR}/network.cpp"
  "${SOURCE_DIR}/neuron.cpp"
  "${SOURCE_DIR}/state.cpp"
  "${SOURCE_DIR}/dsl/lexer.cpp"
  "${SOURCE_DIR}/dsl/parser.cpp"
  "${SOURCE_DIR}/dsl/token.cpp"
  "${SOURCE_DIR}/ea/ea.cpp"
  "${SOURCE_DIR}/ea/fitness.cpp"
  "${SOURCE_DIR}/vm/vm.cpp"
)

set(INCLUDE_DIR include)

project(${TARGET_NAME} VERSION 0.0.1)

include_directories("${INCLUDE_DIR}")

add_executable(${TARGET_NAME} ${SOURCES})
