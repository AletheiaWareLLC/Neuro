#ifndef NEURON_H
#define NEURON_H

#include <ostream>
#include <set>
#include <stack>
#include <vector>

#include <Neuro/data.h>
#include <Neuro/random.h>
#include <Neuro/state.h>

constexpr auto StackLimit = 1000;

class Neuron {
public:
  uint id = 0;
  uint state = 0;
  std::vector<std::unique_ptr<State>> states;
  std::stack<sbyte> stack;

  Neuron() : id(0), state(0) {}
  explicit Neuron(uint id) : id(id), state(0) {}
  Neuron(const Neuron &n) = delete;
  Neuron(Neuron &&n) = delete;
  ~Neuron() {}

  bool duplicate(const Neuron &n);

  bool generate(Random &rng, const std::set<sbyte> alphabet, const uint states,
                const uint actions, const uint instructions);

  bool mate(Random &rng, const Neuron &a, const Neuron &b);

  bool addState(Random &rng, const std::set<sbyte> alphabet, const uint actions,
                const uint instructions);

  bool removeState(const uint id);

  bool emit(std::ostream &os) const;
  void dump(std::ostream &out) const;
};

#endif
