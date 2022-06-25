#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <cstddef>
#include <iostream>
#include <string>

#include <Neuro/data.h>

// Forward Declarations
class Network;
class Neuron;
class State;
class Action;

class Instruction {
public:
  Instruction() {}
  virtual ~Instruction() = default;
  virtual bool execute(Network &nn, Neuron &n, State &s, Action &a) = 0;
  virtual bool emit(std::ostream &os) = 0;
};

class Not : public Instruction {
public:
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) override;
};

class And : public Instruction {
public:
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) override;
};

class Or : public Instruction {
public:
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) override;
};

class Xor : public Instruction {
public:
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) override;
};

class Lls : public Instruction {
public:
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) override;
};

class Rls : public Instruction {
public:
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) override;
};

class Add : public Instruction {
public:
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) override;
};

class Sub : public Instruction {
public:
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) override;
};

class Mul : public Instruction {
public:
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) override;
};

class Div : public Instruction {
public:
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) override;
};

class Mod : public Instruction {
public:
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) override;
};

class Ras : public Instruction {
public:
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) override;
};

class Jump : public Instruction {
public:
  enum class ConditionCode {
    EZ,
    NZ,
    LE,
    LZ,
  };
  ConditionCode code;
  std::string label;
  Jump(ConditionCode c, std::string l) : code(c), label(l) {}
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) override;
};

class Goto : public Instruction {
public:
  unsigned int state = 0;
  Goto(unsigned int s) : state(s) {}
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) override;
};

class Push : public Instruction {
public:
  sbyte constant = 0;
  Push(sbyte c) : constant(c) {}
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) override;
};

class Drop : public Instruction {
public:
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) override;
};

class Copy : public Instruction {
public:
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) override;
};

class Swap : public Instruction {
public:
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) override;
};

class Send : public Instruction {
public:
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) override;
};

#endif