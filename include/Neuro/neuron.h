#ifndef NEURON_H
#define NEURON_H

#include <ostream>
#include <stack>
#include <vector>

#include <Neuro/data.h>
#include <Neuro/state.h>

class Neuron {
public:
  uint id = 0;
  uint state = 0;
  std::vector<State> states;
  std::stack<sbyte> stack;

  Neuron() : id(0), state(0) {}
  explicit Neuron(uint id) : id(id), state(0) {}
  Neuron(const Neuron &n)
      : id(n.id), state(n.state), states(n.states), stack(n.stack) {}
  Neuron(Neuron &&n)
      : id(n.id), state(n.state), states(n.states), stack(n.stack) {}
  ~Neuron() {}
  Neuron &operator=(const Neuron &n) {
    id = n.id;
    state = n.state;
    states = n.states;
    stack = n.stack;
    return *this;
  }
  Neuron &operator=(Neuron &&n) {
    id = n.id;
    state = n.state;
    states = n.states;
    stack = n.stack;
    return *this;
  }

  bool generate(uint states = 3, uint receivers = 3, uint instructions = 3);

  bool emit(std::ostream &os) const;
  void dump() const;
};

#endif
