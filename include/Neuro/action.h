#ifndef ACTION_H
#define ACTION_H

#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include <Neuro/data.h>
#include <Neuro/instruction.h>
#include <Neuro/random.h>

class Action {
public:
  uint pc = 0;
  std::vector<std::shared_ptr<Instruction>> instructions;
  std::map<std::string, uint> labels;
  uint labelIndex = 0;

  Action() {}
  Action(const Action &a) = delete;
  Action(Action &&a) = delete;
  ~Action() {}

  std::shared_ptr<Instruction> createJump(const Jump::ConditionCode cc,
                                          const uint destination);

  bool duplicate(const Action &a);

  bool generate(Random &rng, const uint states, const uint instructions);
  std::shared_ptr<Instruction>
  generateInstruction(Random &rng, const uint states, const uint range);
  std::shared_ptr<Instruction>
  generateJump(Random &rng, const Jump::ConditionCode cc, const uint range);

  bool mate(Random &rng, const Action &a, const Action &b);

  bool mutate(Random &rng, const uint states);

  bool emit(std::ostream &os) const;
};

#endif
