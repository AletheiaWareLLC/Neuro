#include <iostream>

#include <Neuro/action.h>
#include <Neuro/instruction.h>
#include <Neuro/network.h>
#include <Neuro/neuron.h>
#include <Neuro/state.h>

bool Not::execute(Network &nn, Neuron &n, State &s, Action &a) {
  // Check Stack Underflow
  if (n.stack.size() < 1) {
    std::cerr << "Not Error: Stack Underflow" << std::endl;
    return false;
  }

  // Get Operand
  const auto v = n.stack.top();
  // Pop Operand
  n.stack.pop();
  // Push Result
  n.stack.push(~v);
  // Increment Program Counter
  a.pc++;
  return true;
}

bool And::execute(Network &nn, Neuron &n, State &s, Action &a) {
  // Check Stack Underflow
  if (n.stack.size() < 2) {
    std::cerr << "And Error: Stack Underflow" << std::endl;
    return false;
  }

  // Get Second Operand
  const auto w = n.stack.top();
  // Pop Second Operand
  n.stack.pop();
  // Get First Operand
  const auto v = n.stack.top();
  // Pop First Operand
  n.stack.pop();
  // Push Result
  n.stack.push(v & w);
  // Increment Program Counter
  a.pc++;
  return true;
}

bool Or::execute(Network &nn, Neuron &n, State &s, Action &a) {
  // Check Stack Underflow
  if (n.stack.size() < 2) {
    std::cerr << "Or Error: Stack Underflow" << std::endl;
    return false;
  }

  // Get Second Operand
  const auto w = n.stack.top();
  // Pop Second Operand
  n.stack.pop();
  // Get First Operand
  const auto v = n.stack.top();
  // Pop First Operand
  n.stack.pop();
  // Push Result
  n.stack.push(v | w);
  // Increment Program Counter
  a.pc++;
  return true;
}

bool Xor::execute(Network &nn, Neuron &n, State &s, Action &a) {
  // Check Stack Underflow
  if (n.stack.size() < 2) {
    std::cerr << "Xor Error: Stack Underflow" << std::endl;
    return false;
  }

  // Get Second Operand
  const auto w = n.stack.top();
  // Pop Second Operand
  n.stack.pop();
  // Get First Operand
  const auto v = n.stack.top();
  // Pop First Operand
  n.stack.pop();
  // Push Result
  n.stack.push(v ^ w);
  // Increment Program Counter
  a.pc++;
  return true;
}

bool Lls::execute(Network &nn, Neuron &n, State &s, Action &a) {
  // Check Stack Underflow
  if (n.stack.size() < 1) {
    std::cerr << "Lls Error: Stack Underflow" << std::endl;
    return false;
  }

  // Get Operand
  const auto v = n.stack.top();
  // Pop Operand
  n.stack.pop();
  // Push Result
  n.stack.push(v << 1);
  // Increment Program Counter
  a.pc++;
  return true;
}

bool Rls::execute(Network &nn, Neuron &n, State &s, Action &a) {
  // Check Stack Underflow
  if (n.stack.size() < 1) {
    std::cerr << "Rls Error: Stack Underflow" << std::endl;
    return false;
  }

  // Get Operand
  const auto v = n.stack.top();
  // Pop Operand
  n.stack.pop();
  // Push Result
  n.stack.push(((ubyte)v) >> 1);
  // Increment Program Counter
  a.pc++;
  return true;
}

bool Add::execute(Network &nn, Neuron &n, State &s, Action &a) {
  // Check Stack Underflow
  if (n.stack.size() < 2) {
    std::cerr << "Add Error: Stack Underflow" << std::endl;
    return false;
  }

  // Get Second Operand
  const auto w = n.stack.top();
  // Pop Second Operand
  n.stack.pop();
  // Get First Operand
  const auto v = n.stack.top();
  // Pop First Operand
  n.stack.pop();
  // Push Result
  n.stack.push(v + w);
  // Increment Program Counter
  a.pc++;
  return true;
}

bool Sub::execute(Network &nn, Neuron &n, State &s, Action &a) {
  // Check Stack Underflow
  if (n.stack.size() < 2) {
    std::cerr << "Sub Error: Stack Underflow" << std::endl;
    return false;
  }

  // Get Second Operand
  const auto w = n.stack.top();
  // Pop Second Operand
  n.stack.pop();
  // Get First Operand
  const auto v = n.stack.top();
  // Pop First Operand
  n.stack.pop();
  // Push Result
  n.stack.push(v - w);
  // Increment Program Counter
  a.pc++;
  return true;
}

bool Mul::execute(Network &nn, Neuron &n, State &s, Action &a) {
  // Check Stack Underflow
  if (n.stack.size() < 2) {
    std::cerr << "Mul Error: Stack Underflow" << std::endl;
    return false;
  }

  // Get Second Operand
  const auto w = n.stack.top();
  // Pop Second Operand
  n.stack.pop();
  // Get First Operand
  const auto v = n.stack.top();
  // Pop First Operand
  n.stack.pop();
  // Push Result
  n.stack.push(v * w);
  // Increment Program Counter
  a.pc++;
  return true;
}

bool Div::execute(Network &nn, Neuron &n, State &s, Action &a) {
  // Check Stack Underflow
  if (n.stack.size() < 2) {
    std::cerr << "Div Error: Stack Underflow" << std::endl;
    return false;
  }

  // Get Second Operand
  const auto w = n.stack.top();
  // Pop Second Operand
  n.stack.pop();
  // Get First Operand
  const auto v = n.stack.top();
  // Pop First Operand
  n.stack.pop();
  // Check divisor
  if (w == 0) {
    std::cerr << "Division Error: Division by Zero" << std::endl;
    return false;
  }
  // Push Result
  n.stack.push(v / w);
  // Increment Program Counter
  a.pc++;
  return true;
}

bool Mod::execute(Network &nn, Neuron &n, State &s, Action &a) {
  // Check Stack Underflow
  if (n.stack.size() < 2) {
    std::cerr << "Mod Error: Stack Underflow" << std::endl;
    return false;
  }

  // Get Second Operand
  const auto w = n.stack.top();
  // Pop Second Operand
  n.stack.pop();
  // Get First Operand
  const auto v = n.stack.top();
  // Pop First Operand
  n.stack.pop();
  // Check divisor
  if (w == 0) {
    std::cerr << "Modulo Error: Modulo by Zero" << std::endl;
    return false;
  }
  // Push Result
  n.stack.push(v % w);
  // Increment Program Counter
  a.pc++;
  return true;
}

bool Ras::execute(Network &nn, Neuron &n, State &s, Action &a) {
  // Check Stack Underflow
  if (n.stack.size() < 1) {
    std::cerr << "Ras Error: Stack Underflow" << std::endl;
    return false;
  }

  // Get Operand
  const auto v = n.stack.top();
  // Pop Operand
  n.stack.pop();
  // Push Result
  n.stack.push(v >> 1);
  // Increment Program Counter
  a.pc++;
  return true;
}

bool Jump::execute(Network &nn, Neuron &n, State &s, Action &a) {
  // Check Stack Underflow
  if (n.stack.size() < 1) {
    std::cerr << "Jump Error: Stack Underflow" << std::endl;
    return false;
  }

  // Get Operand
  const auto v = n.stack.top();
  // Pop Operand
  n.stack.pop();

  auto jump = false;
  switch (code) {
  case Jump::ConditionCode::EZ:
    jump = v == 0;
    break;
  case Jump::ConditionCode::NZ:
    jump = v != 0;
    break;
  case Jump::ConditionCode::LE:
    jump = (v & 0b10000000) || (v == 0);
    break;
  case Jump::ConditionCode::LZ:
    jump = (v & 0b10000000);
    break;
  default:
    std::cerr << "Jump Error: Unrecognized Condition Code: " << (int)code
              << std::endl;
    return false;
  }
  if (jump) {
    a.pc = a.labels[label];
  } else {
    // Increment Program Counter
    a.pc++;
  }
  return true;
}

bool Goto::execute(Network &nn, Neuron &n, State &s, Action &a) {
  // Terminate Execution
  a.pc = -1;
  // Check State Out-of-Bounds
  if (state >= n.states.size()) {
    std::cerr << "Goto Error: State Out-of-Bounds" << std::endl;
    return false;
  }
  // Transition to State
  n.state = state;
  return true;
}

bool Push::execute(Network &nn, Neuron &n, State &s, Action &a) {
  // Check Stack Overflow
  if (n.stack.size() >= 10) {
    std::cerr << "Push Error: Stack Overflow" << std::endl;
    return false;
  }

  // Push Constant
  n.stack.push(constant);
  // Increment Program Counter
  a.pc++;
  return true;
}

bool Drop::execute(Network &nn, Neuron &n, State &s, Action &a) {
  // Check Stack Underflow
  if (n.stack.size() < 1) {
    std::cerr << "Drop Error: Stack Underflow" << std::endl;
    return false;
  }

  // Pop Constant
  n.stack.pop();
  // Increment Program Counter
  a.pc++;
  return true;
}

bool Copy::execute(Network &nn, Neuron &n, State &s, Action &a) {
  // Check Stack Underflow
  if (n.stack.size() < 1) {
    std::cerr << "Copy Error: Stack Underflow" << std::endl;
    return false;
  }

  // Check Stack Overflow
  if (n.stack.size() >= 10) {
    std::cerr << "Copy Error: Stack Overflow" << std::endl;
    return false;
  }

  // Copy
  n.stack.push(n.stack.top());
  // Increment Program Counter
  a.pc++;
  return true;
}

bool Swap::execute(Network &nn, Neuron &n, State &s, Action &a) {
  // Check Stack Underflow
  if (n.stack.size() < 2) {
    std::cerr << "Swap Error: Stack Underflow" << std::endl;
    return false;
  }

  // Swap
  const auto w = n.stack.top();
  n.stack.pop();
  const auto v = n.stack.top();
  n.stack.pop();
  n.stack.push(w);
  n.stack.push(v);
  // Increment Program Counter
  a.pc++;
  return true;
}

bool Send::execute(Network &nn, Neuron &n, State &s, Action &a) {
  // Check Stack Underflow
  if (n.stack.size() < 1) {
    std::cerr << "Send Error: Stack Underflow" << std::endl;
    return false;
  }

  // Get Operand
  const auto v = n.stack.top();
  // Pop Operand
  n.stack.pop();

  if (const auto itr = nn.connections.find(n.id); itr == nn.connections.end()) {
    std::cerr << "Send Error: Neuron has no Connections" << std::endl;
    return false;
  } else {
    // Send Value via all Connections
    for (const auto c : itr->second) {
      nn.queue.push(std::make_pair(c, v));
    }
  }
  // Increment Program Counter
  a.pc++;
  return true;
}
