#ifndef STATE_H
#define STATE_H

#include <map>
#include <optional>
#include <ostream>

#include <Neuro/action.h>
#include <Neuro/data.h>

class State {
public:
  uint id = 0;
  std::map<sbyte, Action> actions;
  std::optional<Action> wildcard;

  State() : id(0) {}
  explicit State(uint id) : id(id) {}
  State(const State &s) : id(s.id), actions(s.actions), wildcard(s.wildcard) {}
  State(State &&s) : id(s.id), actions(s.actions), wildcard(s.wildcard) {}
  ~State() {}
  State &operator=(const State &s) {
    id = s.id;
    actions = s.actions;
    wildcard = s.wildcard;
    return *this;
  }
  State &operator=(State &&s) {
    id = s.id;
    actions = s.actions;
    wildcard = s.wildcard;
    return *this;
  }

  bool generate(uint states, uint receivers = 3, uint instructions = 3);

  bool emit(std::ostream &os) const;
};

#endif
