#include <Neuro/action.h>
#include <Neuro/network.h>
#include <Neuro/neuron.h>
#include <Neuro/state.h>

class VM {
public:
  const unsigned int cycles;
  VM(const unsigned int c) : cycles(c) {}
  bool execute(Network &nn, Neuron &n, State &s, Action &a, unsigned int &c);
  bool execute(Network &nn, const std::vector<sbyte> &input,
               std::vector<sbyte> &output);
};
