#include <iostream>
#include <utility>

#include <Neuro/data.h>
#include <Neuro/network.h>
#include <Neuro/vm/vm.h>

bool VM::execute(Network &nn, Neuron &n, State &s, Action &a, uint &c) const {
  // Execute Action Instructions
  for (a.pc = 0; a.pc >= 0 && a.pc < a.instructions.size();) {
    // Check and Increment Cycle Counter
    if (++c > cycles) {
      std::cerr << "VM Error: Cycle Limit Exceeded" << std::endl;
      return false;
    }

    const auto pc = a.pc;
    // Execute Instruction
    if (!a.instructions[pc]->execute(nn, n, s, a)) {
      std::cerr << "VM Error: Instruction at pc " << pc << " Failed"
                << std::endl;
      return false;
    }
  }
  return true;
}

bool VM::execute(Network &nn, const std::vector<sint> &input,
                 std::vector<sint> &output, uint &c) const {
  nn.reset();

  const auto ns = nn.neurons.size();

  for (uint in = 0; in < input.size(); in++) {
    // Write Next Input Data
    nn.queue.push(std::make_pair(0, input[in]));

    while (!nn.queue.empty()) {
      // Process Next Activated Neuron
      auto [id, v] = nn.queue.front();
      nn.queue.pop();

      if (id >= nn.neurons.size()) {
        std::cerr << "VM Error: Neuron Out-of-Bounds: " << id << std::endl;
        return false;
      }

      Neuron &n = *nn.neurons[id].get();

      // Check Stack Overflow
      if (n.stack.size() >= StackLimit) {
        std::cerr << "VM Error: Stack Overflow" << std::endl;
        return false;
      }

      // Push Input
      n.stack.push(v);

      if (n.state >= n.states.size()) {
        std::cerr << "VM Error: State Out-of-Bounds: " << n.state << std::endl;
        return false;
      }

      // Get Current State
      State &s = *n.states[n.state].get();

      // Get Action for Input
      if (auto ia = s.actions.find(v); ia != s.actions.end()) {
        if (!execute(nn, n, s, *ia->second.get(), c)) {
          return false;
        }
      } else if (s.wildcard.has_value()) {
        if (!execute(nn, n, s, *s.wildcard.value(), c)) {
          return false;
        }
      } else {
        // Unhandled input
      }
    }

    // Read Next Output Data
    auto &on = *nn.neurons[ns - 1].get();

    // Check Stack Underflow
    if (on.stack.empty()) {
      std::cerr << "VM Error: Stack Underflow" << std::endl;
      return false;
    }

    const auto ov = on.stack.top();
    if (ov != 0) {
      output.push_back(ov);
    }
  }

  return true;
}
