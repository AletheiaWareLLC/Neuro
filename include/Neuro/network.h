#ifndef NETWORK_H
#define NETWORK_H

#include <cstddef>
#include <istream>
#include <map>
#include <ostream>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include <Neuro/data.h>
#include <Neuro/neuron.h>
#include <Neuro/random.h>

class Network {
public:
  std::vector<std::unique_ptr<Neuron>> neurons;
  std::map<uint, std::set<uint>> links;
  std::queue<std::pair<uint, sbyte>> queue;

  Network() {}
  Network(const Network &n) = delete;
  Network(Network &&n) = delete;
  ~Network() {}

  bool load(std::istream &in);
  bool load(const std::string name);

  bool generate(Random &rng, const std::set<sbyte> alphabet, const uint neurons,
                const uint states, const uint actions, const uint instructions,
                const uint links);
  bool generateNeuron(Random &rng, const std::set<sbyte> alphabet,
                      const uint id, const uint states, const uint actions,
                      const uint instructions);
  bool generateLink(Random &rng);

  bool mate(Random &rng, const Network &a, const Network &b);

  bool mutate(Random &rng, const std::set<sbyte> alphabet, const uint states,
              const uint actions, const uint instructions);

  bool addNeuron(Random &rng, const std::set<sbyte> alphabet, const uint states,
                 const uint actions, const uint instructions);

  bool removeNeuron(const uint id);

  bool removeLink(const uint sOffset, const uint dOffset);

  void reset();

  bool emit(std::ostream &os) const;
};

#endif
