#include <Neuro/action.h>
#include <Neuro/instruction.h>
#include <Neuro/network.h>
#include <Neuro/neuron.h>
#include <Neuro/state.h>

bool Network::emit(std::ostream &os) {
  for (auto n : neurons) {
    if (!n.emit(os)) {
      return false;
    }
    os << '\n';
  }

  for (auto [k, v] : connections) {
    for (auto c : v) {
      os << "connect " << k << " " << c << '\n';
    }
  }
  os << '\n';
  return true;
}

bool Neuron::emit(std::ostream &os) {
  os << "neuron " << id << " {\n";
  for (auto s : states) {
    if (!s.emit(os)) {
      return false;
    }
  }
  os << "}\n";
  return true;
}

bool State::emit(std::ostream &os) {
  os << "\tstate " << id << " {\n";
  for (auto [p, a] : actions) {
    os << "\t\treceive " << (int)p << " ";
    if (!a.emit(os)) {
      return false;
    }
  }
  if (wildcard) {
    os << "\t\treceive ";
    if (!wildcard->emit(os)) {
      return false;
    }
  }
  os << "\t}\n";
  return true;
}

bool Action::emit(std::ostream &os) {
  // Reverse label map
  std::map<unsigned int, std::vector<std::string>> ls;
  for (auto [k, v] : labels) {
    ls[v].push_back(k);
  }
  os << "{\n";
  int pc = 0;
  for (auto i : instructions) {
    for (auto l : ls[pc]) {
      os << "\t\t#" << l << '\n';
    }
    os << "\t\t\t";
    if (!i->emit(os)) {
      return false;
    }
    pc++;
  }
  for (auto l : ls[pc]) {
    os << "\t\t#" << l << '\n';
  }
  os << "\t\t}\n";
  return true;
}

bool Not::emit(std::ostream &os) {
  os << "not\n";
  return true;
}

bool And::emit(std::ostream &os) {
  os << "and\n";
  return true;
}

bool Or::emit(std::ostream &os) {
  os << "or\n";
  return true;
}

bool Xor::emit(std::ostream &os) {
  os << "xor\n";
  return true;
}

bool Lls::emit(std::ostream &os) {
  os << "lls\n";
  return true;
}

bool Rls::emit(std::ostream &os) {
  os << "rls\n";
  return true;
}

bool Add::emit(std::ostream &os) {
  os << "add\n";
  return true;
}

bool Sub::emit(std::ostream &os) {
  os << "sub\n";
  return true;
}

bool Mul::emit(std::ostream &os) {
  os << "mul\n";
  return true;
}

bool Div::emit(std::ostream &os) {
  os << "div\n";
  return true;
}

bool Mod::emit(std::ostream &os) {
  os << "mod\n";
  return true;
}

bool Ras::emit(std::ostream &os) {
  os << "ras\n";
  return true;
}

bool Jump::emit(std::ostream &os) {
  switch (code) {
  case Jump::ConditionCode::EZ:
    os << "jez ";
    break;
  case Jump::ConditionCode::NZ:
    os << "jnz ";
    break;
  case Jump::ConditionCode::LE:
    os << "jle ";
    break;
  case Jump::ConditionCode::LZ:
    os << "jlz ";
    break;
  default:
    std::cerr << "Emit Error: Unrecognized Jump Condition Code: " << (int)code
              << std::endl;
    return false;
  }
  os << "#" << label << '\n';
  return true;
}

bool Goto::emit(std::ostream &os) {
  os << "goto " << (int)state << '\n';
  return true;
}

bool Push::emit(std::ostream &os) {
  os << "push " << (int)constant << '\n';
  return true;
}

bool Drop::emit(std::ostream &os) {
  os << "drop\n";
  return true;
}

bool Copy::emit(std::ostream &os) {
  os << "copy\n";
  return true;
}

bool Swap::emit(std::ostream &os) {
  os << "swap\n";
  return true;
}

bool Send::emit(std::ostream &os) {
  os << "send\n";
  return true;
}
