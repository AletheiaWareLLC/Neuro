#ifndef NEURON_H
#define NEURON_H

#include <stack>
#include <vector>

#include <Neuro/data.h>
#include <Neuro/state.h>

class Neuron {
public:
  unsigned int id = 0;
  unsigned int state = 0;
  std::vector<State> states;
  std::stack<sbyte> stack;

  Neuron() : id(0), state(0) {}
  explicit Neuron(unsigned int id) : id(id), state(0) {}

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

  void emit(std::ostream &os);
  void dump();
};

#endif
