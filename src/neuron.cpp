#include <Neuro/neuron.h>

void Neuron::dump() {
  std::cout << "Neuron: " << id << " State: " << state
            << " Stack: " << stack.size() << " [";
  auto first = true;
  while (!stack.empty()) {
    auto o = stack.top();
    stack.pop();
    if (first) {
      first = false;
    } else {
      std::cout << ", ";
    }
    std::cout << std::bitset<8>(o);
  }
  std::cout << "]" << std::endl;
}