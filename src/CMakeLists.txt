set(SOURCES
  "action.cpp"
  "emit.cpp"
  "instruction.cpp"
  "network.cpp"
  "neuron.cpp"
  "state.cpp"
  "dsl/lexer.cpp"
  "dsl/parser.cpp"
  "dsl/token.cpp"
  "ea/ea.cpp"
  "ea/fitness.cpp"
  "vm/vm.cpp"
)

include_directories("../include")

set(LIBRARY_NAME libNeuro)

add_library(${LIBRARY_NAME} ${SOURCES})

set(TARGET_NAME Neuro)

add_executable(${TARGET_NAME} "main.cpp")

target_link_libraries(${TARGET_NAME} ${LIBRARY_NAME})
