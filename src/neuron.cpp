#include <bitset>
#include <iostream>

#include <Neuro/neuron.h>

bool Neuron::duplicate(const Neuron &n) {
  id = n.id;
  for (const auto &ns : n.states) {
    auto s = std::make_unique<State>();

    if (!s->duplicate(*ns.get())) {
      std::cerr << "Neuron Error: State Duplication Failed" << std::endl;
      return false;
    }

    states.push_back(std::move(s));
  }
  return true;
}

bool Neuron::generate(Random &rng, const std::set<sbyte> alphabet,
                      const uint states, const uint actions,
                      const uint instructions) {

  for (int sid = 0; sid < states; sid++) {
    auto s = std::make_unique<State>(sid);

    if (!s->generate(rng, alphabet, states, actions, instructions)) {
      std::cerr << "Neuron Error: State Generation Failed" << std::endl;
      return false;
    }

    this->states.push_back(std::move(s));
  }
  return true;
}

bool Neuron::mate(Random &rng, const Neuron &a, const Neuron &b) {
  const auto as = a.states.size();
  const auto bs = b.states.size();
  auto itra = a.states.cbegin();
  auto itrb = b.states.cbegin();
  uint sid = 0;
  // Mate states shared by parents
  for (; sid < as && sid < bs; sid++) {
    const State &sa = *itra->get();
    const State &sb = *itrb->get();
    itra++;
    itrb++;
    auto sc = std::make_unique<State>(sid);
    if (sc->mate(rng, sa, sb)) {
      states.push_back(std::move(sc));
    } else {
      return false;
    }
  }
  // Copy extra states from a
  for (; sid < as; sid++) {
    const State &sa = *itra->get();
    itra++;
    auto sc = std::make_unique<State>();
    if (sc->duplicate(sa)) {
      states.push_back(std::move(sc));
    } else {
      return false;
    }
  }
  // Copy extra states from b
  for (; sid < bs; sid++) {
    const State &sb = *itrb->get();
    itrb++;
    auto sc = std::make_unique<State>();
    if (sc->duplicate(sb)) {
      states.push_back(std::move(sc));
    } else {
      return false;
    }
  }
  return true;
}

bool Neuron::addState(Random &rng, const std::set<sbyte> alphabet,
                      const uint actions, const uint instructions) {
  const auto ss = states.size();

  const auto id = rng.nextUnsignedInt() % (ss + 1);

  auto s = std::make_unique<State>(id);

  if (!s->generate(rng, alphabet, ss + 1, actions, instructions)) {
    std::cerr << "Neuron Error: State Generation Failed" << std::endl;
    return false;
  }

  // Add state to end
  if (id == ss) {
    states.push_back(std::move(s));
    return true;
  }

  // Insert state at index

  // Update all following state's ids
  for (auto s = id; s < ss; s++) {
    states[s]->id++;
  }

  // Update Goto instructions
  for (auto &s : states) {
    if (s->wildcard) {
      auto &is = s->wildcard.value()->instructions;
      for (auto i = 0; i < is.size(); i++) {
        std::shared_ptr<Goto> go2 = std::dynamic_pointer_cast<Goto>(is[i]);
        if (go2 && go2->state < ss + 1 && go2->state >= id) {
          is[i] = std::make_unique<Goto>(go2->state + 1);
        }
      }
    }
    for (auto &[p, a] : s->actions) {
      auto &is = a->instructions;
      for (auto i = 0; i < is.size(); i++) {
        std::shared_ptr<Goto> go2 = std::dynamic_pointer_cast<Goto>(is[i]);
        if (go2 && go2->state < ss + 1 && go2->state >= id) {
          is[i] = std::make_unique<Goto>(go2->state + 1);
        }
      }
    }
  }
  auto itr = states.begin();
  std::advance(itr, id);
  states.insert(itr, std::move(s));
  return true;
}

bool Neuron::removeState(const uint id) {
  const auto ss = states.size();
  if (ss <= 1) {
    std::cerr << "Neuron Error: Neuron only has one State" << std::endl;
    return false;
  }

  // Update all following state's ids
  for (auto s = id; s < ss; s++) {
    states[s]->id--;
  }

  // Update Goto instructions
  for (auto &s : states) {
    if (s->wildcard) {
      auto &is = s->wildcard.value()->instructions;
      for (auto itr = is.begin(); itr != is.cend(); itr++) {
        std::shared_ptr<Goto> go2 = std::dynamic_pointer_cast<Goto>(*itr);
        if (go2 && go2->state > 0 && go2->state > id) {
          *itr = std::make_unique<Goto>(go2->state - 1);
        }
      }
    }
    for (auto &[p, a] : s->actions) {
      auto &is = a->instructions;
      for (auto itr = is.begin(); itr != is.cend(); itr++) {
        std::shared_ptr<Goto> go2 = std::dynamic_pointer_cast<Goto>(*itr);
        if (go2 && go2->state > 0 && go2->state > id) {
          *itr = std::make_unique<Goto>(go2->state - 1);
        }
      }
    }
  }

  // Erase state
  auto sitr = states.begin();
  std::advance(sitr, id);
  states.erase(sitr);
  return true;
}

void Neuron::dump(std::ostream &out) const {
  out << "Neuron: " << id << " State: " << state
      << " Stack Size: " << stack.size();
  if (stack.size() > 0) {
    out << " Stack Top: " << std::bitset<8>(stack.top());
  }
  out << std::endl;
}
