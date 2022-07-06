#include <iostream>

#include <Neuro/random.h>
#include <Neuro/state.h>

bool State::duplicate(const State &s) {
  id = s.id;
  for (const auto &[np, na] : s.actions) {
    auto a = std::make_unique<Action>();

    if (!a->duplicate(*na.get())) {
      std::cerr << "State Error: Pattern Action Duplication Failed: " << np
                << std::endl;
      return false;
    }

    actions[np] = std::move(a);
  }
  if (s.wildcard) {
    auto a = std::make_unique<Action>();

    if (!a->duplicate(*s.wildcard.value())) {
      std::cerr << "State Error: Wildcard Action Duplication Failed"
                << std::endl;
      return false;
    }

    wildcard = std::move(a);
  }
  return true;
}

bool State::generate(Random &rng, const std::set<sint> alphabet,
                     const uint states, const uint actions,
                     const uint instructions) {

  for (int i = 0; i < actions; i++) {
    auto a = std::make_unique<Action>();

    if (!a->generate(rng, alphabet, states, instructions)) {
      std::cerr << "State Error: Action Generation Failed" << std::endl;
      return false;
    }

    const auto pattern = rng.nextSignedInt();
    if (pattern < 0) {
      wildcard = std::move(a);
    } else {
      auto p = 0;
      if (const auto as = alphabet.size(); as > 0) {
        auto itr = alphabet.cbegin();
        std::advance(itr, pattern % as);
        p = *itr;
      }
      this->actions[p] = std::move(a);
    }
  }
  return true;
}

bool State::mate(Random &rng, const State &a, const State &b) {
  const auto as = a.actions.size();
  const auto bs = b.actions.size();
  auto itra = a.actions.cbegin();
  auto itrb = b.actions.cbegin();
  uint aid = 0;
  // Mate actions shared by parents
  for (; aid < as && aid < bs; aid++) {
    const auto pattern = (aid % 2 == 0 ? itra : itrb)->first;
    const Action &aa = *itra->second;
    const Action &ab = *itrb->second;
    itra++;
    itrb++;
    auto ac = std::make_unique<Action>();
    if (ac->mate(rng, aa, ab)) {
      actions[pattern] = std::move(ac);
    } else {
      return false;
    }
  }
  // Copy extra actions from a
  for (; aid < as; aid++) {
    const auto pattern = itra->first;
    const Action &aa = *itra->second;
    itra++;
    auto ac = std::make_unique<Action>();
    if (ac->duplicate(aa)) {
      actions[pattern] = std::move(ac);
    } else {
      return false;
    }
  }
  // Copy extra actions from b
  for (; aid < bs; aid++) {
    const auto pattern = itrb->first;
    const Action &ab = *itrb->second;
    itrb++;
    auto ac = std::make_unique<Action>();
    if (ac->duplicate(ab)) {
      actions[pattern] = std::move(ac);
    } else {
      return false;
    }
  }

  if (a.wildcard) {
    // Duplicate wildcard from a
    auto ac = std::make_unique<Action>();
    if (ac->duplicate(*a.wildcard->get())) {
      wildcard = std::move(ac);
    } else {
      return false;
    }
  } else if (b.wildcard) {
    // Duplicate wildcard from b
    auto ac = std::make_unique<Action>();
    if (ac->duplicate(*b.wildcard->get())) {
      wildcard = std::move(ac);
    } else {
      return false;
    }
  }

  return true;
}

bool State::addAction(Random &rng, const std::set<sint> alphabet,
                      const uint states, const uint instructions) {
  auto a = std::make_unique<Action>();

  if (!a->generate(rng, alphabet, states, instructions)) {
    std::cerr << "State Error: Action Generation Failed" << std::endl;
    return false;
  }

  const auto pattern = rng.nextSignedInt();
  if (pattern < 0) {
    wildcard = std::move(a);
  } else {
    auto p = 0;
    if (const auto as = alphabet.size(); as > 0) {
      auto itr = alphabet.cbegin();
      std::advance(itr, pattern % as);
      p = *itr;
    }
    actions[p] = std::move(a);
  }
  return true;
}

bool State::removeAction(const sint id) {
  if (id < 0) {
    wildcard.reset();
  } else {
    const auto as = actions.size();
    if (as <= 0) {
      std::cerr << "State Error: State has no Actions" << std::endl;
      return false;
    }
    auto itr = actions.begin();
    std::advance(itr, id % as);
    actions.erase(itr);
  }
  return true;
}
