#include <Neuro/network.h>

int Network::generate(unsigned int neurons, unsigned int connections,
                      unsigned int states, unsigned int receivers,
                      unsigned int instructions) {
  // TODO
  return 0;
}

void Network::reset() {
  for (Neuron &n : neurons) {
    // Revert to initial state
    n.state = 0;
    // Empty stack
    while (!n.stack.empty()) {
      n.stack.pop();
    }
    // Push stack initial value
    n.stack.push(0);
  }
  // Empty activation queue
  while (!queue.empty()) {
    queue.pop();
  }
}
