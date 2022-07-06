#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <ostream>
#include <sstream>
#include <string>

#include <Neuro/data.h>
#include <Neuro/labeller.h>

// Forward Declarations
class Network;
class Neuron;
class State;
class Action;

class Instruction {
public:
  Instruction() {}
  virtual ~Instruction() = default;

  virtual std::unique_ptr<Instruction> duplicate(Labeller &l) = 0;
  virtual bool execute(Network &nn, Neuron &n, State &s, Action &a) = 0;
  virtual bool emit(std::ostream &os) const = 0;
  virtual void addState(uint id) {}
  virtual void removeState(uint id) {}

  std::string str() const {
    std::stringstream ss;
    if (!emit(ss)) {
      return "";
    }
    return ss.str();
  }
};

class Not : public Instruction {
public:
  std::unique_ptr<Instruction> duplicate(Labeller &l) override {
    return std::make_unique<Not>();
  }
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) const override;
};

class And : public Instruction {
public:
  std::unique_ptr<Instruction> duplicate(Labeller &l) override {
    return std::make_unique<And>();
  }
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) const override;
};

class Or : public Instruction {
public:
  std::unique_ptr<Instruction> duplicate(Labeller &l) override {
    return std::make_unique<Or>();
  }
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) const override;
};

class Xor : public Instruction {
public:
  std::unique_ptr<Instruction> duplicate(Labeller &l) override {
    return std::make_unique<Xor>();
  }
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) const override;
};

class Lls : public Instruction {
public:
  std::unique_ptr<Instruction> duplicate(Labeller &l) override {
    return std::make_unique<Lls>();
  }
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) const override;
};

class Rls : public Instruction {
public:
  std::unique_ptr<Instruction> duplicate(Labeller &l) override {
    return std::make_unique<Rls>();
  }
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) const override;
};

class Add : public Instruction {
public:
  std::unique_ptr<Instruction> duplicate(Labeller &l) override {
    return std::make_unique<Add>();
  }
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) const override;
};

class Sub : public Instruction {
public:
  std::unique_ptr<Instruction> duplicate(Labeller &l) override {
    return std::make_unique<Sub>();
  }
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) const override;
};

class Mul : public Instruction {
public:
  std::unique_ptr<Instruction> duplicate(Labeller &l) override {
    return std::make_unique<Mul>();
  }
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) const override;
};

class Div : public Instruction {
public:
  std::unique_ptr<Instruction> duplicate(Labeller &l) override {
    return std::make_unique<Div>();
  }
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) const override;
};

class Mod : public Instruction {
public:
  std::unique_ptr<Instruction> duplicate(Labeller &l) override {
    return std::make_unique<Mod>();
  }
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) const override;
};

class Ras : public Instruction {
public:
  std::unique_ptr<Instruction> duplicate(Labeller &l) override {
    return std::make_unique<Ras>();
  }
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) const override;
};

class Jump : public Instruction {
public:
  enum class ConditionCode {
    EZ,
    NZ,
    LE,
    LZ,
  };
  Labeller &labeller;
  const ConditionCode code;
  const std::string label;

  Jump(Labeller &l, ConditionCode c, uint dest)
      : Jump(l, c, "l" + std::to_string(l.labelIndex++)) {
    // Add label to labeller
    labeller.labels[label] = dest;
  }

  Jump(Labeller &l, ConditionCode c, std::string lbl)
      : labeller(l), code(c), label(lbl) {}
  ~Jump() { labeller.labels.erase(label); }

  std::unique_ptr<Instruction> duplicate(Labeller &l) override {
    return std::make_unique<Jump>(l, code, l.labels[label]);
  }
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) const override;
};

class Goto : public Instruction {
public:
  uint state = 0;

  Goto(uint s) : state(s) {}

  std::unique_ptr<Instruction> duplicate(Labeller &l) override {
    return std::make_unique<Goto>(state);
  }
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) const override;
  void addState(uint id) override {
    if (state >= id) {
      state++;
    }
  }
  void removeState(uint id) override {
    if (state > id) {
      state--;
    }
  }
};

class Push : public Instruction {
public:
  const sint constant = 0;

  Push(sint c) : constant(c) {}

  std::unique_ptr<Instruction> duplicate(Labeller &l) override {
    return std::make_unique<Push>(constant);
  }
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) const override;
};

class Drop : public Instruction {
public:
  std::unique_ptr<Instruction> duplicate(Labeller &l) override {
    return std::make_unique<Drop>();
  }
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) const override;
};

class Copy : public Instruction {
public:
  std::unique_ptr<Instruction> duplicate(Labeller &l) override {
    return std::make_unique<Copy>();
  }
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) const override;
};

class Swap : public Instruction {
public:
  std::unique_ptr<Instruction> duplicate(Labeller &l) override {
    return std::make_unique<Swap>();
  }
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) const override;
};

class Send : public Instruction {
public:
  std::unique_ptr<Instruction> duplicate(Labeller &l) override {
    return std::make_unique<Send>();
  }
  bool execute(Network &nn, Neuron &n, State &s, Action &a) override;
  bool emit(std::ostream &os) const override;
};

#endif
