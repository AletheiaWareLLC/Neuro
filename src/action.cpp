#include <iostream>

#include <Neuro/action.h>
#include <Neuro/random.h>

bool Action::generate(uint states, uint instructions) {
  for (int i = 0; i < instructions; i++) {
    this->instructions.push_back(generateInstruction(states));
  }
  return true;
}

std::shared_ptr<Instruction> Action::generateInstruction(uint states) {
  switch (udist(rng) % 20) {
  case 0:
    return std::make_shared<Not>();
  case 1:
    return std::make_shared<And>();
  case 2:
    return std::make_shared<Or>();
  case 3:
    return std::make_shared<Xor>();
  case 4:
    return std::make_shared<Lls>();
  case 5:
    return std::make_shared<Rls>();
  case 6:
    return std::make_shared<Add>();
  case 7:
    return std::make_shared<Sub>();
  case 8:
    return std::make_shared<Mul>();
  case 9:
    return std::make_shared<Div>();
  case 10:
    return std::make_shared<Mod>();
  case 11:
    return std::make_shared<Ras>();
  case 12: {
    const auto cc = (Jump::ConditionCode)(udist(rng) % 4);
    const auto label = "l" + std::to_string(labels.size());
    // Add label to action
    labels[label] = udist(rng) % (instructions.size() + 1);
    return std::make_shared<Jump>(cc, label);
  }
  case 13:
    return std::make_shared<Goto>(udist(rng) % states);
  case 14:
    return std::make_shared<Push>((sbyte)udist(rng));
  case 15:
    return std::make_shared<Drop>();
  case 16:
    return std::make_shared<Copy>();
  case 17:
    return std::make_shared<Swap>();
  default:
    return std::make_shared<Send>();
  }
}

bool Action::mutate(uint states) {
  // Default case is most likely mutation
  // Adding is twice as likely as removing
  switch (udist(rng) % 10) {
  case 0:
  case 1: {
    // Add Instruction
    std::cout << "Mutation: Add Instruction" << std::endl;
    instructions.push_back(generateInstruction(states));
    break;
  }
  case 2: {
    // Remove Instruction
    std::cout << "Mutation: Remove Instruction" << std::endl;
    // TODO
    break;
  }
  default: {
    // Replace Instruction
    std::cout << "Mutation: Replace Instruction" << std::endl;
    instructions[udist(rng) % instructions.size()] =
        generateInstruction(states);
    break;
  }
  }
  return true;
}
