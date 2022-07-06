#ifndef VM_H
#define VM_H

#include <vector>

#include <Neuro/action.h>
#include <Neuro/data.h>
#include <Neuro/network.h>
#include <Neuro/neuron.h>
#include <Neuro/state.h>

class VM {
public:
  const uint cycles;

  VM(const uint cs) : cycles(cs) {}

  bool execute(Network &nn, Neuron &n, State &s, Action &a, uint &c) const;
  bool execute(Network &nn, const std::vector<sint> &input,
               std::vector<sint> &output, uint &c) const;
};

#endif
