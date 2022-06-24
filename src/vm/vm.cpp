#include <cstddef>

#include <Neuro/data.h>
#include <Neuro/network.h>
#include <Neuro/vm/vm.h>

bool VM::execute(Network &nn, Neuron &n, State &s, Action &a, unsigned int &c) {
  // Execute Action Instructions
  for (a.pc = 0; a.pc >= 0 && a.pc < a.instructions.size();) {
    // Check and Increment Cycle Counter
    if (c++ > cycles) {
      std::cerr << "Error: Cycle Limit Exceeded" << std::endl;
      return false;
    }

    // Execute Instruction
    if (!a.instructions[a.pc]->execute(nn, n, s, a)) {
      std::cerr << "Error: Instruction at pc " << a.pc << " Failed"
                << std::endl;
      return false;
    }
  }
  return true;
}

bool VM::execute(Network &nn, const std::vector<sbyte> &input,
                 std::vector<sbyte> &output) {
  nn.reset();

  auto ns = nn.neurons.size();

  // Write Input Data
  for (int i = 0; i < input.size() && i < ns; i++) {
    nn.queue.push(std::make_pair(i, input[i]));
  }

  unsigned int c = 0;

  // Iterate Activated Neurons
  while (!nn.queue.empty()) {
    auto [id, v] = nn.queue.front();
    nn.queue.pop();
    Neuron &n = nn.neurons[id];

    // Push Input
    n.stack.push(v);

    // Get Current State
    State &s = n.states[n.state];

    // Get Action for Input
    if (auto ia = s.actions.find(v); ia != s.actions.end()) {
      if (!execute(nn, n, s, ia->second, c)) {
        return false;
      }
    } else if (s.wildcard.has_value()) {
      if (!execute(nn, n, s, s.wildcard.value(), c)) {
        return false;
      }
    } else {
      // Unhandled input
    }
  }

  // Read Output Data
  for (int i = 0; i < output.size() && i < ns; i++) {
    output[i] = nn.neurons[ns - i - 1].stack.top();
  }

  return true;
}
