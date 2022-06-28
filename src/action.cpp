#include <iostream>

#include <Neuro/action.h>
#include <Neuro/random.h>

bool Action::duplicate(const Action &a) {
  // Instructions are never modified so they can be shared.
  instructions.assign(a.instructions.cbegin(), a.instructions.cend());
  labels.insert(a.labels.cbegin(), a.labels.cend());
  return true;
}

bool Action::generate(Random &rng, const uint states, const uint instructions) {
  for (int i = 0; i < instructions; i++) {
    this->instructions.push_back(
        generateInstruction(rng, states, instructions + 1));
  }
  return true;
}

std::shared_ptr<Instruction>
Action::generateInstruction(Random &rng, const uint states, const uint range) {
  // To reduce stack underflows, instructions that increase stack size are
  // artificially made more likely.
  // Send is also made artificially more likely to increase communication
  // between neurons.
  switch (rng.nextUnsignedInt() % 35) {
  case 0:
    return std::make_shared<Not>(); // Stack size unchanged
  case 1:
    return std::make_shared<And>(); // Stack size decreased
  case 2:
    return std::make_shared<Or>(); // Stack size decreased
  case 3:
    return std::make_shared<Xor>(); // Stack size decreased
  case 4:
    return std::make_shared<Lls>(); // Stack size unchanged
  case 5:
    return std::make_shared<Rls>(); // Stack size unchanged
  case 6:
    return std::make_shared<Add>(); // Stack size decreased
  case 7:
    return std::make_shared<Sub>(); // Stack size decreased
  case 8:
    return std::make_shared<Mul>(); // Stack size decreased
  case 9:
    return std::make_shared<Div>(); // Stack size decreased
  case 10:
    return std::make_shared<Mod>(); // Stack size decreased
  case 11:
    return std::make_shared<Ras>(); // Stack size unchanged
  case 12:
    return generateJump(rng, Jump::ConditionCode::EZ,
                        range); // Stack size decreased
  case 13:
    return generateJump(rng, Jump::ConditionCode::NZ,
                        range); // Stack size decreased
  case 14:
    return generateJump(rng, Jump::ConditionCode::LE,
                        range); // Stack size decreased
  case 15:
    return generateJump(rng, Jump::ConditionCode::LZ,
                        range); // Stack size decreased
  case 16:
    return std::make_shared<Goto>(rng.nextUnsignedInt() %
                                  states); // Stack size unchanged
  case 17:
  case 18:
  case 19:
  case 20:
  case 21:
  case 22:
  case 23:
  case 24:
  case 25: // Push is 9x more likely
    return std::make_shared<Push>(
        (sbyte)rng.nextUnsignedInt()); // Stack size increased
  case 26:
    return std::make_shared<Drop>(); // Stack size decreased
  case 27:
  case 28:
  case 29:
  case 30:
  case 31:                           // Copy is 5x more likely
    return std::make_shared<Copy>(); // Stack size increased
  case 32:
    return std::make_shared<Swap>(); // Stack size unchanged
  default:                           // Send is 2x more likely
    return std::make_shared<Send>(); // Stack size unchanged
  }
}

std::shared_ptr<Instruction> Action::createJump(const Jump::ConditionCode cc,
                                                const uint destination) {
  const auto label = "l" + std::to_string(labelIndex++);
  // Add label to action
  labels[label] = destination;
  return std::make_shared<Jump>(cc, label);
}

std::shared_ptr<Instruction> Action::generateJump(Random &rng,
                                                  const Jump::ConditionCode cc,
                                                  const uint range) {
  return createJump(cc, rng.nextUnsignedInt() % range);
}

bool Action::mate(Random &rng, const Action &a, const Action &b) {
  // Instructions are never modified so they can be shared.
  const auto limit = std::max(a.instructions.size(), b.instructions.size());
  if (limit == 0) {
    return false;
  }
  const auto split = rng.nextUnsignedInt() % (limit + 1);
  uint iid = 0;
  // Neuron 0 until split are taken from A
  for (; iid < split && iid < a.instructions.size(); iid++) {
    const auto i = a.instructions[iid];
    if (const auto j = std::dynamic_pointer_cast<Jump>(i); j) {
      instructions.push_back(createJump(j->code, a.labels.at(j->label)));
    } else {
      instructions.push_back(i);
    }
  }
  // The rest of the Neurons are taken from B
  for (; iid < b.instructions.size(); iid++) {
    const auto i = b.instructions[iid];
    if (const auto j = std::dynamic_pointer_cast<Jump>(i); j) {
      instructions.push_back(createJump(j->code, b.labels.at(j->label)));
    } else {
      instructions.push_back(i);
    }
  }
  // If A has more Neurons than B, top up child from A
  for (; iid < a.instructions.size(); iid++) {
    const auto i = a.instructions[iid];
    if (const auto j = std::dynamic_pointer_cast<Jump>(i); j) {
      instructions.push_back(createJump(j->code, a.labels.at(j->label)));
    } else {
      instructions.push_back(i);
    }
  }
  return true;
}

bool Action::mutate(Random &rng, const uint states) {
  // Default case is twice as likely
  switch (rng.nextUnsignedInt() % 4) {
  case 0: {
    // Add Instruction
    instructions.push_back(
        generateInstruction(rng, states, instructions.size() + 1));
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

    // If Instruction is Jump, remove Label
    if (const auto j = std::dynamic_pointer_cast<Jump>(*itr); j) {
      labels.erase(j->label);
    }

    instructions.erase(itr);
    break;
  }
  default: {
    const auto is = instructions.size();
    if (is == 0) {
      return false;
    }

    const auto iid = rng.nextUnsignedInt() % is;

    // If Instruction is Jump, remove Label
    if (const auto j = std::dynamic_pointer_cast<Jump>(instructions[iid]); j) {
      labels.erase(j->label);
    }

    // Replace Instruction
    instructions[iid] =
        generateInstruction(rng, states, instructions.size() + 1);
    break;
  }
  }
  return true;
}
