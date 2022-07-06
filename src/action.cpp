#include <iostream>

#include <Neuro/action.h>
#include <Neuro/random.h>

bool Action::duplicate(const Action &a) {
  for (auto &i : a.instructions) {
    instructions.push_back(i->duplicate(*this));
  }
  labels.insert(a.labels.cbegin(), a.labels.cend());
  return true;
}

bool Action::generate(Random &rng, const std::set<sint> alphabet,
                      const uint states, const uint instructions) {
  for (int i = 0; i < instructions; i++) {
    this->instructions.push_back(
        generateInstruction(rng, alphabet, states, instructions + 1));
  }
  return true;
}

std::unique_ptr<Instruction>
Action::generateInstruction(Random &rng, const std::set<sint> alphabet,
                            const uint states, const uint range) {
  // To reduce stack underflows, instructions that increase stack size are
  // artificially made more likely.
  // Send is also made artificially more likely to increase communication
  // between neurons.
  const auto i = rng.nextUnsignedInt();
  switch (i % 38) {
  case 0:
    return std::make_unique<Not>(); // Stack size unchanged
  case 1:
    return std::make_unique<And>(); // Stack size decreased
  case 2:
    return std::make_unique<Or>(); // Stack size decreased
  case 3:
    return std::make_unique<Xor>(); // Stack size decreased
  case 4:
    return std::make_unique<Lls>(); // Stack size unchanged
  case 5:
    return std::make_unique<Rls>(); // Stack size unchanged
  case 6:
    return std::make_unique<Add>(); // Stack size decreased
  case 7:
    return std::make_unique<Sub>(); // Stack size decreased
  case 8:
    return std::make_unique<Mul>(); // Stack size decreased
  case 9:
    return std::make_unique<Div>(); // Stack size decreased
  case 10:
    return std::make_unique<Mod>(); // Stack size decreased
  case 11:
    return std::make_unique<Ras>(); // Stack size unchanged
  case 12:
    return std::make_unique<Jump>(*this, Jump::ConditionCode::EZ,
                                  rng.nextUnsignedInt() %
                                      range); // Stack size decreased
  case 13:
    return std::make_unique<Jump>(*this, Jump::ConditionCode::NZ,
                                  rng.nextUnsignedInt() %
                                      range); // Stack size decreased
  case 14:
    return std::make_unique<Jump>(*this, Jump::ConditionCode::LE,
                                  rng.nextUnsignedInt() %
                                      range); // Stack size decreased
  case 15:
    return std::make_unique<Jump>(*this, Jump::ConditionCode::LZ,
                                  rng.nextUnsignedInt() %
                                      range); // Stack size decreased
  case 16:
    return std::make_unique<Goto>(rng.nextUnsignedInt() %
                                  states); // Stack size unchanged
  case 17:
  case 18:
  case 19:
  case 20:
  case 21:
  case 22:
  case 23:
  case 24:
  case 25: { // Push is 9x more likely
    auto constant = 0;
    if (const auto as = alphabet.size(); as > 0) {
      auto itr = alphabet.cbegin();
      std::advance(itr, rng.nextUnsignedInt() % as);
      constant = *itr;
    }
    return std::make_unique<Push>(constant); // Stack size increased
  }
  case 26:
    return std::make_unique<Drop>(); // Stack size decreased
  case 27:
  case 28:
  case 29:
  case 30:
  case 31:                           // Copy is 5x more likely
    return std::make_unique<Copy>(); // Stack size increased
  case 32:
    return std::make_unique<Swap>(); // Stack size unchanged
  default:
    return std::make_unique<Send>(); // Stack size decreased
  }
}

bool Action::mate(Random &rng, const Action &a, const Action &b) {
  const auto limit = std::max(a.instructions.size(), b.instructions.size());
  if (limit == 0) {
    return false;
  }
  const auto split = rng.nextUnsignedInt() % (limit + 1);
  uint iid = 0;
  // Neuron 0 until split are taken from A
  for (; iid < split && iid < a.instructions.size(); iid++) {
    const auto &i = a.instructions[iid];
    instructions.push_back(i->duplicate(*this));
  }
  // The rest of the Neurons are taken from B
  for (; iid < b.instructions.size(); iid++) {
    const auto &i = b.instructions[iid];
    instructions.push_back(i->duplicate(*this));
  }
  // If A has more Neurons than B, top up child from A
  for (; iid < a.instructions.size(); iid++) {
    const auto &i = a.instructions[iid];
    instructions.push_back(i->duplicate(*this));
  }
  return true;
}

bool Action::mutate(Random &rng, const std::set<sint> alphabet,
                    const uint states) {
  // Default case is twice as likely
  switch (rng.nextUnsignedInt() % 4) {
  case 0: {
    // Add Instruction
    instructions.push_back(
        generateInstruction(rng, alphabet, states, instructions.size() + 1));
    break;
  }
  case 1: {
    const auto is = instructions.size();
    if (is == 0) {
      return false;
    }
    // Remove Instruction
    auto itr = instructions.begin();
    std::advance(itr, rng.nextUnsignedInt() % is);

    instructions.erase(itr);
    break;
  }
  default: {
    const auto is = instructions.size();
    if (is == 0) {
      return false;
    }

    const auto iid = rng.nextUnsignedInt() % is;

    // Replace Instruction
    instructions[iid] =
        generateInstruction(rng, alphabet, states, instructions.size() + 1);
    break;
  }
  }
  return true;
}
