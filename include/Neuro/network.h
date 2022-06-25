#ifndef NETWORK_H
#define NETWORK_H

#include <cstddef>
#include <map>
#include <ostream>
#include <queue>
#include <set>
#include <string>
#include <utility>
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

  std::string id() const;

  bool load(std::string name);

  bool generate(uint neurons = 3, uint connections = 5, uint states = 3,
                uint receivers = 3, uint instructions = 3);
  bool generateNeuron(uint id, uint states = 3, uint receivers = 3,
                      uint instructions = 3);
  bool generateConnection();

  bool mate(const Network &a, const Network &b);

  bool mutate();

  void reset();

  bool emit(std::ostream &os) const;
};

#endif
