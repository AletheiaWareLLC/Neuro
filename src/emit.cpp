#include <Neuro/action.h>
#include <Neuro/instruction.h>
#include <Neuro/network.h>
#include <Neuro/neuron.h>
#include <Neuro/state.h>

void Network::emit(std::ostream &os) {
  for (auto n : neurons) {
    n.emit(os);
    os << '\n';
  }

  for (auto [k, v] : connections) {
    for (auto c : v) {
      os << "connect " << k << " " << c << '\n';
    }
  }
  os << '\n';
}

void Neuron::emit(std::ostream &os) {
  os << "neuron " << id << " {\n";
  for (auto s : states) {
    s.emit(os);
  }
  os << "}\n";
}

void State::emit(std::ostream &os) {
  os << "\tstate " << id << " {\n";
  for (auto [p, a] : actions) {
    os << "\t\trecieve " << p << " ";
    a.emit(os);
  }
  if (wildcard) {
    os << "\t\trecieve ";
    wildcard->emit(os);
  }
  os << "\t}\n";
}

void Action::emit(std::ostream &os) {
  os << "{\n";
  for (auto i : instructions) {
    os << "\t\t\t";
    i->emit(os);
  }
  os << "\t\t}\n";
}

void Not::emit(std::ostream &os) { os << "not\n"; }

void And::emit(std::ostream &os) { os << "and\n"; }

void Or::emit(std::ostream &os) { os << "or\n"; }

void Xor::emit(std::ostream &os) { os << "xor\n"; }

void Lls::emit(std::ostream &os) { os << "lls\n"; }

void Rls::emit(std::ostream &os) { os << "rls\n"; }

void Add::emit(std::ostream &os) { os << "add\n"; }

void Sub::emit(std::ostream &os) { os << "sub\n"; }

void Mul::emit(std::ostream &os) { os << "mul\n"; }

void Div::emit(std::ostream &os) { os << "div\n"; }

void Mod::emit(std::ostream &os) { os << "mod\n"; }

void Ras::emit(std::ostream &os) { os << "ras\n"; }

void Jump::emit(std::ostream &os) {
  switch (code) {
  case Jump::ConditionCode::EZ:
    os << "jez ";
  case Jump::ConditionCode::NZ:
    os << "jnz ";
  case Jump::ConditionCode::LE:
    os << "jle ";
  case Jump::ConditionCode::LZ:
    os << "jlz ";
  }
  os << "#" << label << '\n';
}

void Goto::emit(std::ostream &os) { os << "goto " << state << '\n'; }

void Push::emit(std::ostream &os) { os << "push " << constant << '\n'; }

void Drop::emit(std::ostream &os) { os << "drop\n"; }

void Copy::emit(std::ostream &os) { os << "copy\n"; }

void Swap::emit(std::ostream &os) { os << "swap\n"; }

void Send::emit(std::ostream &os) { os << "send\n"; }
