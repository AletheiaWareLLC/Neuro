#include <gtest/gtest.h>

#include <Neuro/alphabet.h>
#include <Neuro/random.h>
#include <Neuro/state.h>

TEST(StateTest, Duplicate) {
  FakeRandom rng;
  State a(1);
  rng.sints.push(0);  // Not Instruction
  rng.sints.push(0);  // Pattern Action
  rng.sints.push(1);  // And Instruction
  rng.sints.push(1);  // Pattern Action
  rng.sints.push(0);  // Not Instruction
  rng.sints.push(-1); // Wildcard
  ASSERT_TRUE(a.generate(rng, numeric, 1, 3, 1));

  State b;
  ASSERT_TRUE(b.duplicate(a));

  ASSERT_EQ(1, b.id);
  ASSERT_EQ(2, b.actions.size());
  ASSERT_TRUE(b.wildcard);
}

TEST(StateTest, Generate) {
  FakeRandom rng;
  rng.sints.push(0);  // Not Instruction
  rng.sints.push(-1); // Wildcard

  State s;

  ASSERT_FALSE(s.wildcard);

  ASSERT_TRUE(s.generate(rng, numeric, 1, 1, 1));

  ASSERT_TRUE(s.wildcard);

  rng.sints.push(0); // Not Instruction
  rng.sints.push(0); // Pattern Action

  ASSERT_TRUE(s.generate(rng, numeric, 1, 1, 1));

  ASSERT_EQ(1, s.actions.size());
  ASSERT_EQ('0', s.actions.begin()->first);
}

TEST(StateTest, Mate) {
  FakeRandom rng;
  State a;
  rng.sints.push(0);  // Not Instruction
  rng.sints.push(-1); // Wildcard
  rng.sints.push(1);  // And Instruction
  rng.sints.push(1);  // Pattern Action
  ASSERT_TRUE(a.generate(rng, numeric, 1, 2, 1));
  State b;
  rng.sints.push(2);  // Or Instruction
  rng.sints.push(-1); // Wildcard
  rng.sints.push(3);  // Xor Instruction
  rng.sints.push(2);  // Pattern Action
  ASSERT_TRUE(b.generate(rng, numeric, 1, 2, 1));

  rng.uints.push(1); // Action 1 (Pattern) Split Index
  rng.uints.push(2); // Action 2 (Wildcard) Split Index
  State c;
  ASSERT_TRUE(c.mate(rng, a, b));

  ASSERT_EQ(1, c.actions.size());
  auto itr = c.actions.cbegin();
  ASSERT_EQ('1', itr->first);
  ASSERT_TRUE(c.wildcard);
}

TEST(StateTest, MateUnequalActionsA) {
  FakeRandom rng;
  State a;
  rng.sints.push(0); // Not Instruction
  rng.sints.push(0); // Pattern Action
  rng.sints.push(1); // And Instruction
  rng.sints.push(1); // Pattern Action
  ASSERT_TRUE(a.generate(rng, numeric, 1, 2, 1));
  State b;
  rng.sints.push(3); // Xor Instruction
  rng.sints.push(2); // Pattern Action
  ASSERT_TRUE(b.generate(rng, numeric, 1, 1, 1));

  rng.uints.push(1); // Action Split Index
  State c;
  ASSERT_TRUE(c.mate(rng, a, b));

  ASSERT_EQ(2, c.actions.size());
  auto itr = c.actions.cbegin();
  ASSERT_EQ('0', itr->first);
  itr++;
  ASSERT_EQ('1', itr->first);
  ASSERT_FALSE(c.wildcard);
}

TEST(StateTest, MateUnequalActionsB) {
  FakeRandom rng;
  State a;
  rng.sints.push(0); // Not Instruction
  rng.sints.push(0); // Pattern Action
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1, 1));
  State b;
  rng.sints.push(3); // Xor Instruction
  rng.sints.push(1); // Pattern Action
  rng.sints.push(1); // And Instruction
  rng.sints.push(2); // Pattern Action
  ASSERT_TRUE(b.generate(rng, numeric, 1, 2, 1));

  rng.uints.push(1); // Action Split Index
  State c;
  ASSERT_TRUE(c.mate(rng, a, b));

  ASSERT_EQ(2, c.actions.size());
  auto itr = c.actions.cbegin();
  ASSERT_EQ('0', itr->first);
  itr++;
  ASSERT_EQ('2', itr->first);
  ASSERT_FALSE(c.wildcard);
}

TEST(StateTest, MateWildcardA) {
  FakeRandom rng;
  State a;
  rng.sints.push(0);  // Not Instruction
  rng.sints.push(-1); // Wildcard
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1, 1));
  State b;

  State c;
  ASSERT_TRUE(c.mate(rng, a, b));

  ASSERT_TRUE(c.actions.empty());
  ASSERT_TRUE(c.wildcard);
}

TEST(StateTest, MateWildcardB) {
  FakeRandom rng;
  State a;
  State b;
  rng.sints.push(0);  // Not Instruction
  rng.sints.push(-1); // Wildcard
  ASSERT_TRUE(b.generate(rng, numeric, 1, 1, 1));

  State c;
  ASSERT_TRUE(c.mate(rng, a, b));

  ASSERT_TRUE(c.actions.empty());
  ASSERT_TRUE(c.wildcard);
}

TEST(StateTest, AddRemoveAction) {
  FakeRandom rng;
  State s;

  rng.sints.push(0); // Not Instruction
  rng.sints.push(0); // Pattern Action
  ASSERT_TRUE(s.addAction(rng, numeric, 1, 1));
  ASSERT_FALSE(s.wildcard);
  ASSERT_EQ(1, s.actions.size());
  rng.sints.push(1);  // And Instruction
  rng.sints.push(-1); // Wildcard
  ASSERT_TRUE(s.addAction(rng, numeric, 1, 1));
  ASSERT_TRUE(s.wildcard);
  ASSERT_EQ(1, s.actions.size());

  ASSERT_TRUE(s.removeAction(-1)); // Wildcard
  ASSERT_FALSE(s.wildcard);
  ASSERT_EQ(1, s.actions.size());

  ASSERT_TRUE(s.removeAction(0)); // Pattern Action Index
  ASSERT_TRUE(s.actions.empty());
}
