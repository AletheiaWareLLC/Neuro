#ifndef ACTION_H
#define ACTION_H

#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <Neuro/data.h>
#include <Neuro/instruction.h>

class Action {
public:
  unsigned int pc = 0;
  std::vector<std::shared_ptr<Instruction>> instructions;
  std::map<std::string, unsigned int> labels;
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
  bool emit(std::ostream &os);
};

#endif