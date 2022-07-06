#ifndef STATE_H
#define STATE_H

#include <map>
#include <optional>
#include <ostream>
#include <set>

#include <Neuro/action.h>
#include <Neuro/data.h>
#include <Neuro/random.h>

class State {
public:
  uint id = 0;
  std::map<sint, std::unique_ptr<Action>> actions;
  std::optional<std::unique_ptr<Action>> wildcard;

  State() : id(0) {}
  explicit State(uint id) : id(id) {}
  State(const State &s) = delete;
  State(State &&s) = delete;
  ~State() {}

  bool duplicate(const State &s);

  bool generate(Random &rng, const std::set<sint> alphabet, const uint states,
                const uint actions, const uint instructions);

  bool mate(Random &rng, const State &a, const State &b);

  bool addAction(Random &rng, const std::set<sint> alphabet, const uint states,
                 const uint instructions);

  bool removeAction(const sint id);

  bool emit(std::ostream &os) const;
};

#endif
