#include <Neuro/neuron.h>

void Neuron::dump() const {
  std::cout << "Neuron: " << id << " State: " << state
            << " Stack Size: " << stack.size()
            << " Stack Top: " << std::bitset<8>(stack.top()) << std::endl;
}