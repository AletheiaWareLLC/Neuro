#include <Neuro/action.h>
#include <Neuro/network.h>
#include <Neuro/neuron.h>
#include <Neuro/state.h>

class VM {
public:
  const uint cycles;
  VM(const uint cs) : cycles(cs) {}
  bool execute(Network &nn, Neuron &n, State &s, Action &a, uint &c);
  bool execute(Network &nn, const std::vector<sbyte> &input,
               std::vector<sbyte> &output);
};
