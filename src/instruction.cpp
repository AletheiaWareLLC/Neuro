#include <Neuro/action.h>
#include <Neuro/instruction.h>
#include <Neuro/network.h>
#include <Neuro/neuron.h>
#include <Neuro/state.h>

bool Not::execute(Network &nn, Neuron &n, State &s, Action &a) {
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
  // Transition to State
  n.state = state;
  return true;
}

bool Push::execute(Network &nn, Neuron &n, State &s, Action &a) {
  // Push Constant
  n.stack.push(constant);
  // Increment Program Counter
  a.pc++;
  return true;
}

bool Drop::execute(Network &nn, Neuron &n, State &s, Action &a) {
  // Pop Constant
  n.stack.pop();
  // Increment Program Counter
  a.pc++;
  return true;
}

bool Copy::execute(Network &nn, Neuron &n, State &s, Action &a) {
  // Copy
  n.stack.push(n.stack.top());
  // Increment Program Counter
  a.pc++;
  return true;
}

bool Swap::execute(Network &nn, Neuron &n, State &s, Action &a) {
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
