#include <bitset>
#include <iostream>

#include <Neuro/neuron.h>

bool Neuron::generate(uint states, uint receivers, uint instructions) {
  for (int sid = 0; sid < states; sid++) {
    State s(sid);

    if (!s.generate(receivers, instructions)) {
      std::cerr << "Neuron Error: State Generation Failed" << std::endl;
      return false;
    }

    this->states.push_back(s);
  }
  return true;
}

void Neuron::dump() const {
  std::cout << "Neuron: " << id << " State: " << state
            << " Stack Size: " << stack.size();
  if (stack.size() > 0) {
    std::cout << " Stack Top: " << std::bitset<8>(stack.top());
  }
  std::cout << std::endl;
}