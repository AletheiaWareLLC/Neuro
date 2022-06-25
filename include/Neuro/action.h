#ifndef ACTION_H
#define ACTION_H

#include <map>
#include <ostream>
#include <string>
#include <vector>

#include <Neuro/data.h>
#include <Neuro/instruction.h>

class Action {
public:
  uint pc = 0;
  std::vector<std::shared_ptr<Instruction>> instructions;
  std::map<std::string, uint> labels;

  Action() {}
  Action(const Action &a)
      : pc(a.pc), instructions(a.instructions), labels(a.labels) {}
  Action(Action &&a)
      : pc(a.pc), instructions(a.instructions), labels(a.labels) {}
  ~Action() {}
  Action &operator=(const Action &a) {
    pc = a.pc;
    instructions = a.instructions;
    labels = a.labels;
    return *this;
  }
  Action &operator=(Action &&a) {
    pc = a.pc;
    instructions = a.instructions;
    labels = a.labels;
    return *this;
  }

  bool generate(uint states, uint instructions = 3);
  std::shared_ptr<Instruction> generateInstruction(uint states);

  bool mutate(uint states);

  bool emit(std::ostream &os) const;
};

#endif