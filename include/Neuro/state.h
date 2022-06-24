#ifndef STATE_H
#define STATE_H

#include <map>
#include <optional>

#include <Neuro/action.h>
#include <Neuro/data.h>

class State {
public:
  unsigned int id = 0;
  std::map<sbyte, Action> actions;
  std::optional<Action> wildcard;

  State() : id(0) {}
  explicit State(unsigned int id) : id(id) {}
  State(const State &s) : id(s.id), actions(s.actions), wildcard(s.wildcard) {}
  State(State &&s) : id(s.id), actions(s.actions), wildcard(s.wildcard) {}
  ~State() {}
  State &operator=(const State &s) {
    std::cout << "Copy Assign" << std::endl;
    std::cout << this << std::endl;
    std::cout << &s << std::endl;
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

  void emit(std::ostream &os);
};

#endif
