#ifndef NETWORK_H
#define NETWORK_H

#include <cstddef>
#include <queue>
#include <vector>

#include <Neuro/data.h>
#include <Neuro/neuron.h>

class Network {
public:
  std::vector<Neuron> neurons;
  std::map<unsigned int, std::vector<unsigned int>> connections;
  std::queue<std::pair<unsigned int, sbyte>> queue;

  Network() {}
  Network(const Network &n)
      : neurons(n.neurons), connections(n.connections), queue(n.queue) {}
  Network(Network &&n)
      : neurons(n.neurons), connections(n.connections), queue(n.queue) {}
  ~Network() {}
  Network &operator=(const Network &n) {
    neurons = n.neurons;
    connections = n.connections;
    queue = n.queue;
    return *this;
  }
  Network &operator=(Network &&n) {
    neurons = n.neurons;
    connections = n.connections;
    queue = n.queue;
    return *this;
  }

  bool generate(unsigned int neurons, unsigned int connections,
               unsigned int states, unsigned int receivers,
               unsigned int instructions);

  void reset();
  bool emit(std::ostream &os);
};

#endif
