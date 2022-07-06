#ifndef ACTION_H
#define ACTION_H

#include <map>
#include <memory>
#include <ostream>
#include <set>
#include <string>
#include <vector>

#include <Neuro/data.h>
#include <Neuro/instruction.h>
#include <Neuro/labeller.h>
#include <Neuro/random.h>

class Action : public Labeller {
public:
  uint pc = 0;
  std::vector<std::unique_ptr<Instruction>> instructions;

  Action() {}
  Action(const Action &a) = delete;
  Action(Action &&a) = delete;
  ~Action() {}

  std::unique_ptr<Instruction> createJump(const Jump::ConditionCode cc,
                                          const uint destination);

  bool duplicate(const Action &a);

  bool generate(Random &rng, const std::set<sint> alphabet, const uint states,
                const uint instructions);
  std::unique_ptr<Instruction>
  generateInstruction(Random &rng, const std::set<sint> alphabet,
                      const uint states, const uint range);

  bool mate(Random &rng, const Action &a, const Action &b);

  bool mutate(Random &rng, const std::set<sint> alphabet, const uint states);

  bool emit(std::ostream &os) const;
};

#endif
