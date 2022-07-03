#include <gtest/gtest.h>

#include <Neuro/alphabet.h>
#include <Neuro/neuron.h>
#include <Neuro/random.h>

TEST(NeuronTest, Duplicate) {
  FakeRandom rng;
  Neuron a(1);

  rng.sints.push(0);  // Not Instruction
  rng.sints.push(-1); // Wildcard
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1, 1));

  Neuron b;
  ASSERT_TRUE(b.duplicate(a));
  ASSERT_EQ(1, b.id);
  ASSERT_EQ(1, b.states.size());
}

TEST(NeuronTest, Generate) {
  FakeRandom rng;
  Neuron n;

  rng.sints.push(0);  // Not Instruction
  rng.sints.push(-1); // Wildcard
  ASSERT_TRUE(n.generate(rng, numeric, 1, 1, 1));
  ASSERT_EQ(1, n.states.size());
}

TEST(NeuronTest, Mate) {
  FakeRandom rng;

  Neuron a;
  rng.sints.push(0);  // Not Instruction
  rng.sints.push(-1); // Wildcard
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1, 1));
  ASSERT_EQ(1, a.states.size());
  rng.sints.push(2); // Or Instruction
  rng.sints.push(1); // Pattern Action
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1, 1));
  ASSERT_EQ(2, a.states.size());

  Neuron b;
  rng.sints.push(1); // And Instruction
  rng.sints.push(1); // Pattern Action
  ASSERT_TRUE(b.generate(rng, numeric, 1, 1, 1));
  ASSERT_EQ(1, b.states.size());
  rng.sints.push(3);  // Xor Instruction
  rng.sints.push(-1); // Wildcard
  ASSERT_TRUE(b.generate(rng, numeric, 1, 1, 1));
  ASSERT_EQ(2, b.states.size());

  rng.uints.push(1); // State 0 Split Index
  rng.uints.push(0); // State 1 Split Index
  Neuron c;
  ASSERT_TRUE(c.mate(rng, a, b));
  ASSERT_EQ(2, c.states.size());
  auto &s0 = *c.states[0];
  ASSERT_TRUE(s0.wildcard);
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Not>(
                         s0.wildcard.value()->instructions[0]));
  ASSERT_EQ(1, s0.actions.size());
  auto &s0a0 = s0.actions['1'];
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<And>(s0a0->instructions[0]));
  auto &s1 = *c.states[1];
  ASSERT_TRUE(s1.wildcard);
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Xor>(
                         s1.wildcard.value()->instructions[0]));
  ASSERT_EQ(1, s1.actions.size());
  auto &s1a0 = s1.actions['1'];
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Or>(s1a0->instructions[0]));
}

TEST(NeuronTest, MateUnequalStatesA) {
  FakeRandom rng;

  Neuron a;
  rng.sints.push(0);  // Not Instruction
  rng.sints.push(-1); // Wildcard
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1, 1));
  ASSERT_EQ(1, a.states.size());
  rng.sints.push(2); // Or Instruction
  rng.sints.push(1); // Pattern Action
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1, 1));
  ASSERT_EQ(2, a.states.size());
  rng.sints.push(1); // And Instruction
  rng.sints.push(2); // Pattern Action
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1, 1));
  ASSERT_EQ(3, a.states.size());

  Neuron b;
  rng.sints.push(1); // And Instruction
  rng.sints.push(1); // Pattern Action
  ASSERT_TRUE(b.generate(rng, numeric, 1, 1, 1));
  ASSERT_EQ(1, b.states.size());
  rng.sints.push(3);  // Xor Instruction
  rng.sints.push(-1); // Wildcard
  ASSERT_TRUE(b.generate(rng, numeric, 1, 1, 1));
  ASSERT_EQ(2, b.states.size());

  rng.uints.push(1); // State 0 Split Index
  rng.uints.push(0); // State 1 Split Index
  rng.uints.push(0); // State 2 Split Index
  Neuron c;
  ASSERT_TRUE(c.mate(rng, a, b));
  ASSERT_EQ(3, c.states.size());
  auto &s0 = *c.states[0];
  ASSERT_TRUE(s0.wildcard);
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Not>(
                         s0.wildcard.value()->instructions[0]));
  ASSERT_EQ(1, s0.actions.size());
  auto &s0a0 = s0.actions['1'];
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<And>(s0a0->instructions[0]));
  auto &s1 = *c.states[1];
  ASSERT_TRUE(s1.wildcard);
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Xor>(
                         s1.wildcard.value()->instructions[0]));
  ASSERT_EQ(1, s1.actions.size());
  auto &s1a0 = s1.actions['1'];
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Or>(s1a0->instructions[0]));
  auto &s2 = *c.states[2];
  ASSERT_FALSE(s2.wildcard);
  ASSERT_EQ(1, s2.actions.size());
  auto &s2a0 = s2.actions['2'];
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<And>(s2a0->instructions[0]));
}

TEST(NeuronTest, MateUnequalStatesB) {
  FakeRandom rng;

  Neuron a;
  rng.sints.push(0);  // Not Instruction
  rng.sints.push(-1); // Wildcard
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1, 1));
  ASSERT_EQ(1, a.states.size());
  rng.sints.push(2); // Or Instruction
  rng.sints.push(1); // Pattern Action
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1, 1));
  ASSERT_EQ(2, a.states.size());

  Neuron b;
  rng.sints.push(1); // And Instruction
  rng.sints.push(1); // Pattern Action
  ASSERT_TRUE(b.generate(rng, numeric, 1, 1, 1));
  ASSERT_EQ(1, b.states.size());
  rng.sints.push(3);  // Xor Instruction
  rng.sints.push(-1); // Wildcard
  ASSERT_TRUE(b.generate(rng, numeric, 1, 1, 1));
  ASSERT_EQ(2, b.states.size());
  rng.sints.push(1); // And Instruction
  rng.sints.push(2); // Pattern Action
  ASSERT_TRUE(b.generate(rng, numeric, 1, 1, 1));
  ASSERT_EQ(3, b.states.size());

  rng.uints.push(1); // State 0 Split Index
  rng.uints.push(0); // State 1 Split Index
  rng.uints.push(0); // State 2 Split Index
  Neuron c;
  ASSERT_TRUE(c.mate(rng, a, b));
  ASSERT_EQ(3, c.states.size());
  auto &s0 = *c.states[0];
  ASSERT_TRUE(s0.wildcard);
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Not>(
                         s0.wildcard.value()->instructions[0]));
  ASSERT_EQ(1, s0.actions.size());
  auto &s0a0 = s0.actions['1'];
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<And>(s0a0->instructions[0]));
  auto &s1 = *c.states[1];
  ASSERT_TRUE(s1.wildcard);
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Xor>(
                         s1.wildcard.value()->instructions[0]));
  ASSERT_EQ(1, s1.actions.size());
  auto &s1a0 = s1.actions['1'];
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Or>(s1a0->instructions[0]));
  auto &s2 = *c.states[2];
  ASSERT_FALSE(s2.wildcard);
  ASSERT_EQ(1, s2.actions.size());
  auto &s2a0 = s2.actions['2'];
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<And>(s2a0->instructions[0]));
}

TEST(NeuronTest, AddRemoveState) {
  FakeRandom rng;
  Neuron n;

  // Add state with wildcard action at index 0
  rng.uints.push(0);  // State Index
  rng.sints.push(16); // Goto Instruction
  rng.uints.push(0);  // State
  rng.sints.push(-1); // Wildcard Action
  ASSERT_TRUE(n.addState(rng, numeric, 1, 1));
  ASSERT_EQ(1, n.states.size());
  {
    const auto &s0 = *n.states[0];
    ASSERT_TRUE(s0.wildcard);
    ASSERT_EQ(0, s0.actions.size());
  }

  // Add state with pattern action at index 0
  rng.uints.push(0); // State Index
  rng.sints.push(0); // Not Instruction
  rng.sints.push(1); // Pattern Action
  ASSERT_TRUE(n.addState(rng, numeric, 1, 1));
  ASSERT_EQ(2, n.states.size());
  {
    auto &s0 = *n.states[0];
    ASSERT_FALSE(s0.wildcard);
    ASSERT_EQ(1, s0.actions.size());

    // Assert wildcard state has been shifted up
    const auto &s1 = *n.states[1];
    ASSERT_TRUE(s1.wildcard);
    ASSERT_EQ(0, s1.actions.size());

    const auto &s1a = *s1.wildcard.value();
    const auto &s1ai0 = std::dynamic_pointer_cast<Goto>(s1a.instructions[0]);
    ASSERT_NE(nullptr, s1ai0);
    ASSERT_EQ(1, s1ai0->state);
  }

  // Remove state at index 0
  ASSERT_TRUE(n.removeState(0));
  ASSERT_EQ(1, n.states.size());
  {
    // Assert wildcard state has been shifted down
    const auto &s0 = *n.states[0];
    ASSERT_TRUE(s0.wildcard);
    ASSERT_EQ(0, s0.actions.size());

    const auto &s0a = *s0.wildcard.value();
    const auto &s0ai0 = std::dynamic_pointer_cast<Goto>(s0a.instructions[0]);
    ASSERT_NE(nullptr, s0ai0);
    ASSERT_EQ(0, s0ai0->state);
  }
}
