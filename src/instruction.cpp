#include <iostream>

#include <Neuro/action.h>
#include <Neuro/instruction.h>
#include <Neuro/network.h>
#include <Neuro/neuron.h>
#include <Neuro/state.h>

std::ostream &error(Network &nn, Neuron &n, State &s, Action &a) {
  std::cerr << "Instruction Error: ";
  // std::cerr << "Instruction Error: Network " << nn.id();
  // std::cerr << std::endl;
  // n.dump(std::cerr);
  return std::cerr;
}

bool Not::execute(Network &nn, Neuron &n, State &s, Action &a) {
  // Check Stack Underflow
  if (n.stack.size() < 1) {
    error(nn, n, s, a) << "Not: Stack Underflow" << std::endl;
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
    error(nn, n, s, a) << "And: Stack Underflow" << std::endl;
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
    error(nn, n, s, a) << "Or: Stack Underflow" << std::endl;
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
    error(nn, n, s, a) << "Xor: Stack Underflow" << std::endl;
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
    error(nn, n, s, a) << "Lls: Stack Underflow" << std::endl;
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
    error(nn, n, s, a) << "Rls: Stack Underflow" << std::endl;
    return false;
  }

  // Get Operand
  const auto v = n.stack.top();
  // Pop Operand
  n.stack.pop();
  // Push Result
  n.stack.push(((uint)v) >> 1);
  // Increment Program Counter
  a.pc++;
  return true;
}

bool Add::execute(Network &nn, Neuron &n, State &s, Action &a) {
  // Check Stack Underflow
  if (n.stack.size() < 2) {
    error(nn, n, s, a) << "Add: Stack Underflow" << std::endl;
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
    error(nn, n, s, a) << "Sub: Stack Underflow" << std::endl;
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
    error(nn, n, s, a) << "Mul: Stack Underflow" << std::endl;
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
    error(nn, n, s, a) << "Div: Stack Underflow" << std::endl;
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
    error(nn, n, s, a) << "Div: Division by Zero" << std::endl;
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
    error(nn, n, s, a) << "Mod: Stack Underflow" << std::endl;
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
    error(nn, n, s, a) << "Mod: Modulo by Zero" << std::endl;
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
    error(nn, n, s, a) << "Ras: Stack Underflow" << std::endl;
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
    error(nn, n, s, a) << "Jump: Stack Underflow" << std::endl;
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
    error(nn, n, s, a) << "Jump: Unrecognized Condition Code: " << (int)code
                       << std::endl;
    return false;
  }
  if (jump) {
    auto itr = a.labels.find(label);
    if (itr == a.labels.end()) {
      error(nn, n, s, a) << "Jump: Label Not Found: " << label << std::endl;
      return false;
    }
    a.pc = itr->second;
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
    error(nn, n, s, a) << "Goto: State Out-of-Bounds" << std::endl;
    return false;
  }
  // Transition to State
  n.state = state;
  return true;
}

bool Push::execute(Network &nn, Neuron &n, State &s, Action &a) {
  // Check Stack Overflow
  if (n.stack.size() >= StackLimit) {
    error(nn, n, s, a) << "Push: Stack Overflow" << std::endl;
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
    error(nn, n, s, a) << "Drop: Stack Underflow" << std::endl;
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
    error(nn, n, s, a) << "Copy: Stack Underflow" << std::endl;
    return false;
  }

  // Check Stack Overflow
  if (n.stack.size() >= StackLimit) {
    error(nn, n, s, a) << "Copy: Stack Overflow" << std::endl;
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
    error(nn, n, s, a) << "Swap: Stack Underflow" << std::endl;
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
    error(nn, n, s, a) << "Send: Stack Underflow" << std::endl;
    return false;
  }

  // Get Operand
  const auto v = n.stack.top();
  // Pop Operand
  n.stack.pop();

  if (const auto itr = nn.links.find(n.id); itr == nn.links.end()) {
    error(nn, n, s, a) << "Send: Neuron has no Links" << std::endl;
    return false;
  } else {
    // Send Value via all Links
    for (const auto c : itr->second) {
      nn.queue.push(std::make_pair(c, v));
    }
  }
  // Increment Program Counter
  a.pc++;
  return true;
}
