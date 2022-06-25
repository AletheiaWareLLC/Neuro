#include <iostream>

#include <Neuro/random.h>
#include <Neuro/state.h>

bool State::generate(uint states, uint receivers, uint instructions) {
  for (int i = 0; i < receivers; i++) {
    Action a;

    if (!a.generate(states, instructions)) {
      std::cerr << "State Error: Action Generation Failed" << std::endl;
      return false;
    }

    if (i == 0) {
      wildcard = a;
    } else {
      actions[(sbyte)udist(rng)] = a;
    }
  }
  return true;
}
