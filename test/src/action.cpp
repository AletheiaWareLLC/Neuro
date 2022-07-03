#include <gtest/gtest.h>

#include <Neuro/action.h>
#include <Neuro/alphabet.h>
#include <Neuro/random.h>

TEST(ActionTest, Duplicate) {
  FakeRandom rng;
  Action a;

  rng.sints.push(0);  // Not Instruction
  rng.sints.push(1);  // And Instruction
  rng.sints.push(2);  // Or Instruction
  rng.sints.push(3);  // Xor Instruction
  rng.sints.push(4);  // Lls Instruction
  rng.sints.push(5);  // Rls Instruction
  rng.sints.push(6);  // Add Instruction
  rng.sints.push(7);  // Sub Instruction
  rng.sints.push(8);  // Mul Instruction
  rng.sints.push(9);  // Div Instruction
  rng.sints.push(10); // Mod Instruction
  rng.sints.push(11); // Ras Instruction
  rng.sints.push(12); // Jez Instruction
  rng.uints.push(1);  // Jump Label
  rng.sints.push(13); // Jne Instruction
  rng.uints.push(1);  // Jump Label
  rng.sints.push(14); // Jle Instruction
  rng.uints.push(1);  // Jump Label
  rng.sints.push(15); // Jlz Instruction
  rng.uints.push(1);  // Jump Label
  rng.sints.push(16); // Goto Instruction
  rng.uints.push(0);  // State
  rng.sints.push(17); // Push Instruction
  rng.uints.push(12); // Constant
  rng.sints.push(26); // Drop Instruction
  rng.sints.push(27); // Copy Instruction
  rng.sints.push(32); // Swap Instruction
  rng.sints.push(-1); // Send Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 22));

  Action b;
  ASSERT_TRUE(b.duplicate(a));
  ASSERT_EQ(22, b.instructions.size());
  ASSERT_EQ(4, b.labels.size());
  ASSERT_NE(b.labels.cend(), b.labels.find("l0"));
  ASSERT_NE(b.labels.cend(), b.labels.find("l1"));
  ASSERT_NE(b.labels.cend(), b.labels.find("l2"));
  ASSERT_NE(b.labels.cend(), b.labels.find("l3"));
}

TEST(ActionTest, GenerateNot) {
  FakeRandom rng;
  Action a;

  rng.sints.push(0); // Not Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Not>(a.instructions[0]));
}

TEST(ActionTest, GenerateAnd) {
  FakeRandom rng;
  Action a;

  rng.sints.push(1); // And Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<And>(a.instructions[0]));
}

TEST(ActionTest, GenerateOr) {
  FakeRandom rng;
  Action a;

  rng.sints.push(2); // Or Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Or>(a.instructions[0]));
}

TEST(ActionTest, GenerateXor) {
  FakeRandom rng;
  Action a;

  rng.sints.push(3); // Xor Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Xor>(a.instructions[0]));
}

TEST(ActionTest, GenerateLls) {
  FakeRandom rng;
  Action a;

  rng.sints.push(4); // Lls Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Lls>(a.instructions[0]));
}

TEST(ActionTest, GenerateRls) {
  FakeRandom rng;
  Action a;

  rng.sints.push(5); // Rls Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Rls>(a.instructions[0]));
}

TEST(ActionTest, GenerateAdd) {
  FakeRandom rng;
  Action a;

  rng.sints.push(6); // Add Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Add>(a.instructions[0]));
}

TEST(ActionTest, GenerateSub) {
  FakeRandom rng;
  Action a;

  rng.sints.push(7); // Sub Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Sub>(a.instructions[0]));
}

TEST(ActionTest, GenerateMul) {
  FakeRandom rng;
  Action a;

  rng.sints.push(8); // Mul Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Mul>(a.instructions[0]));
}

TEST(ActionTest, GenerateDiv) {
  FakeRandom rng;
  Action a;

  rng.sints.push(9); // Div Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Div>(a.instructions[0]));
}

TEST(ActionTest, GenerateMod) {
  FakeRandom rng;
  Action a;

  rng.sints.push(10); // Mod Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Mod>(a.instructions[0]));
}

TEST(ActionTest, GenerateRas) {
  FakeRandom rng;
  Action a;

  rng.sints.push(11); // Ras Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Ras>(a.instructions[0]));
}

TEST(ActionTest, GenerateJez) {
  FakeRandom rng;
  Action a;

  rng.sints.push(12); // Jez Instruction
  rng.uints.push(1);  // Jump Label
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  const auto jump = std::dynamic_pointer_cast<Jump>(a.instructions[0]);
  ASSERT_NE(nullptr, jump);

  // Check Jump Label
  ASSERT_EQ("l0", jump->label);
  ASSERT_EQ(1, a.labels.size());
  ASSERT_EQ(1, a.labels[jump->label]);
}

TEST(ActionTest, GenerateJne) {
  FakeRandom rng;
  Action a;

  rng.sints.push(13); // Jne Instruction
  rng.uints.push(1);  // Jump Label
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  const auto jump = std::dynamic_pointer_cast<Jump>(a.instructions[0]);
  ASSERT_NE(nullptr, jump);

  // Check Jump Label
  ASSERT_EQ("l0", jump->label);
  ASSERT_EQ(1, a.labels.size());
  ASSERT_EQ(1, a.labels[jump->label]);
}

TEST(ActionTest, GenerateJle) {
  FakeRandom rng;
  Action a;

  rng.sints.push(14); // Jle Instruction
  rng.uints.push(1);  // Jump Label
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  const auto jump = std::dynamic_pointer_cast<Jump>(a.instructions[0]);
  ASSERT_NE(nullptr, jump);

  // Check Jump Label
  ASSERT_EQ("l0", jump->label);
  ASSERT_EQ(1, a.labels.size());
  ASSERT_EQ(1, a.labels[jump->label]);
}

TEST(ActionTest, GenerateJlz) {
  FakeRandom rng;
  Action a;

  rng.sints.push(15); // Jlz Instruction
  rng.uints.push(1);  // Jump Label
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  const auto jump = std::dynamic_pointer_cast<Jump>(a.instructions[0]);
  ASSERT_NE(nullptr, jump);

  // Check Jump Label
  ASSERT_EQ("l0", jump->label);
  ASSERT_EQ(1, a.labels.size());
  ASSERT_EQ(1, a.labels[jump->label]);
}

TEST(ActionTest, GenerateGoto) {
  FakeRandom rng;
  Action a;

  rng.sints.push(16); // Goto Instruction
  rng.uints.push(0);  // State
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  const auto go2 = std::dynamic_pointer_cast<Goto>(a.instructions[0]);
  ASSERT_NE(nullptr, go2);

  // Check Goto State
  ASSERT_EQ(0, go2->state);
}

TEST(ActionTest, GeneratePush) {
  FakeRandom rng;
  Action a;

  // Push is 9x more probable

  rng.sints.push(17); // Push Instruction
  rng.uints.push(0);  // Constant
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  auto push = std::dynamic_pointer_cast<Push>(a.instructions[0]);
  ASSERT_NE(nullptr, push);

  // Check Push Constant
  ASSERT_EQ('0', push->constant);

  rng.sints.push(18); // Push Instruction
  rng.uints.push(1);  // Constant
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(2, a.instructions.size());
  push = std::dynamic_pointer_cast<Push>(a.instructions[1]);
  ASSERT_NE(nullptr, push);

  // Check Push Constant
  ASSERT_EQ('1', push->constant);

  rng.sints.push(19); // Push Instruction
  rng.uints.push(2);  // Constant
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(3, a.instructions.size());
  push = std::dynamic_pointer_cast<Push>(a.instructions[2]);
  ASSERT_NE(nullptr, push);

  // Check Push Constant
  ASSERT_EQ('2', push->constant);

  rng.sints.push(20); // Push Instruction
  rng.uints.push(3);  // Constant
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(4, a.instructions.size());
  push = std::dynamic_pointer_cast<Push>(a.instructions[3]);
  ASSERT_NE(nullptr, push);

  // Check Push Constant
  ASSERT_EQ('3', push->constant);

  rng.sints.push(21); // Push Instruction
  rng.uints.push(4);  // Constant
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(5, a.instructions.size());
  push = std::dynamic_pointer_cast<Push>(a.instructions[4]);
  ASSERT_NE(nullptr, push);

  // Check Push Constant
  ASSERT_EQ('4', push->constant);

  rng.sints.push(22); // Push Instruction
  rng.uints.push(5);  // Constant
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(6, a.instructions.size());
  push = std::dynamic_pointer_cast<Push>(a.instructions[5]);
  ASSERT_NE(nullptr, push);

  // Check Push Constant
  ASSERT_EQ('5', push->constant);

  rng.sints.push(23); // Push Instruction
  rng.uints.push(6);  // Constant
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(7, a.instructions.size());
  push = std::dynamic_pointer_cast<Push>(a.instructions[6]);
  ASSERT_NE(nullptr, push);

  // Check Push Constant
  ASSERT_EQ('6', push->constant);

  rng.sints.push(24); // Push Instruction
  rng.uints.push(7);  // Constant
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(8, a.instructions.size());
  push = std::dynamic_pointer_cast<Push>(a.instructions[7]);
  ASSERT_NE(nullptr, push);

  // Check Push Constant
  ASSERT_EQ('7', push->constant);

  rng.sints.push(25); // Push Instruction
  rng.uints.push(8);  // Constant
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(9, a.instructions.size());
  push = std::dynamic_pointer_cast<Push>(a.instructions[8]);
  ASSERT_NE(nullptr, push);

  // Check Push Constant
  ASSERT_EQ('8', push->constant);
}

TEST(ActionTest, GenerateDrop) {
  FakeRandom rng;
  Action a;

  rng.sints.push(26); // Drop Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Drop>(a.instructions[0]));
}

TEST(ActionTest, GenerateCopy) {
  FakeRandom rng;
  Action a;

  // Copy is 5x more probable

  rng.sints.push(27); // Copy Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Copy>(a.instructions[0]));

  rng.sints.push(28); // Copy Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(2, a.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Copy>(a.instructions[1]));

  rng.sints.push(29); // Copy Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(3, a.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Copy>(a.instructions[2]));

  rng.sints.push(30); // Copy Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(4, a.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Copy>(a.instructions[3]));

  rng.sints.push(31); // Copy Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(5, a.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Copy>(a.instructions[4]));
}

TEST(ActionTest, GenerateSwap) {
  FakeRandom rng;
  Action a;

  rng.sints.push(32); // Swap Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Swap>(a.instructions[0]));
}

TEST(ActionTest, GenerateSend) {
  FakeRandom rng;
  Action a;

  rng.sints.push(-1); // Send Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Send>(a.instructions[0]));
}

TEST(ActionTest, Mate) {
  FakeRandom rng;
  Action a;
  rng.sints.push(0); // Not Instruction
  rng.sints.push(1); // And Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 2));
  Action b;
  rng.sints.push(3);  // Xor Instruction
  rng.sints.push(12); // Jez Instruction
  rng.uints.push(1);  // Jump Label
  ASSERT_TRUE(b.generate(rng, numeric, 1, 2));

  rng.uints.push(1); // Split Index
  Action c;
  ASSERT_TRUE(c.mate(rng, a, b));

  ASSERT_EQ(2, c.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Not>(c.instructions[0]));
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Jump>(c.instructions[1]));
  ASSERT_EQ(1, c.labels.size());
}

TEST(ActionTest, MateUnequalInstructionsA) {
  FakeRandom rng;
  Action a;
  rng.sints.push(0); // Not Instruction
  rng.sints.push(1); // And Instruction
  rng.sints.push(2); // Or Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 3));
  Action b;
  rng.sints.push(3); // Xor Instruction
  ASSERT_TRUE(b.generate(rng, numeric, 1, 1));

  rng.uints.push(1); // Split Index
  Action c;
  ASSERT_TRUE(c.mate(rng, a, b));

  ASSERT_EQ(3, c.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Not>(c.instructions[0]));
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<And>(c.instructions[1]));
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Or>(c.instructions[2]));
  ASSERT_TRUE(c.labels.empty());
}

TEST(ActionTest, MateUnequalInstructionsB) {
  FakeRandom rng;
  Action a;
  rng.sints.push(0); // Not Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));
  Action b;
  rng.sints.push(3);  // Xor Instruction
  rng.sints.push(12); // Jez Instruction
  rng.uints.push(1);  // Jump Label
  rng.sints.push(1);  // And Instruction
  ASSERT_TRUE(b.generate(rng, numeric, 1, 3));

  rng.uints.push(1); // Split Index
  Action c;
  ASSERT_TRUE(c.mate(rng, a, b));

  ASSERT_EQ(3, c.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Not>(c.instructions[0]));
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Jump>(c.instructions[1]));
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<And>(c.instructions[2]));
  ASSERT_EQ(1, c.labels.size());
}

TEST(ActionTest, Mutate) {
  FakeRandom rng;
  Action a;

  // Add Instruction 0
  rng.uints.push(0);
  rng.sints.push(12); // Jez Instruction
  rng.uints.push(1);  // Jump Label
  ASSERT_TRUE(a.mutate(rng, numeric, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Jump>(a.instructions[0]));
  ASSERT_EQ(1, a.labels.size());

  // Add Instruction 1
  rng.uints.push(0);
  rng.sints.push(0); // Not Instruction
  ASSERT_TRUE(a.mutate(rng, numeric, 1));

  ASSERT_EQ(2, a.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<Not>(a.instructions[1]));

  // Remove Instruction 0
  rng.uints.push(1);
  rng.uints.push(0); // First Instruction
  ASSERT_TRUE(a.mutate(rng, numeric, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ(0, a.labels.size());

  // Replace Instruction 0
  rng.uints.push(2);
  rng.uints.push(0); // First Instruction
  rng.sints.push(1); // And Instruction
  ASSERT_TRUE(a.mutate(rng, numeric, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_NE(nullptr, std::dynamic_pointer_cast<And>(a.instructions[0]));
}
