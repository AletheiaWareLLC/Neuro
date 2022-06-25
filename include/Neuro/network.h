#ifndef NETWORK_H
#define NETWORK_H

#include <cstddef>
#include <map>
#include <queue>
#include <set>
#include <vector>

#include <Neuro/data.h>
#include <Neuro/neuron.h>

class Network {
public:
  std::vector<Neuron> neurons;
  std::map<uint, std::set<uint>> connections;
  std::queue<std::pair<uint, sbyte>> queue;

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

  bool generate(uint neurons, uint connections,
                uint states, uint receivers,
                uint instructions);

  void reset();
  bool emit(std::ostream &os) const;
};

#endif
