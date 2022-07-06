#include <gtest/gtest.h>

#include <Neuro/action.h>
#include <Neuro/alphabet.h>
#include <Neuro/random.h>

TEST(ActionTest, Duplicate) {
  FakeRandom rng;
  Action a;

  rng.uints.push(0);  // Not Instruction
  rng.uints.push(1);  // And Instruction
  rng.uints.push(2);  // Or Instruction
  rng.uints.push(3);  // Xor Instruction
  rng.uints.push(4);  // Lls Instruction
  rng.uints.push(5);  // Rls Instruction
  rng.uints.push(6);  // Add Instruction
  rng.uints.push(7);  // Sub Instruction
  rng.uints.push(8);  // Mul Instruction
  rng.uints.push(9);  // Div Instruction
  rng.uints.push(10); // Mod Instruction
  rng.uints.push(11); // Ras Instruction
  rng.uints.push(12); // Jez Instruction
  rng.uints.push(1);  // Jump Label
  rng.uints.push(13); // Jne Instruction
  rng.uints.push(1);  // Jump Label
  rng.uints.push(14); // Jle Instruction
  rng.uints.push(1);  // Jump Label
  rng.uints.push(15); // Jlz Instruction
  rng.uints.push(1);  // Jump Label
  rng.uints.push(16); // Goto Instruction
  rng.uints.push(0);  // State
  rng.uints.push(17); // Push Instruction
  rng.uints.push(12); // Constant
  rng.uints.push(26); // Drop Instruction
  rng.uints.push(27); // Copy Instruction
  rng.uints.push(32); // Swap Instruction
  rng.uints.push(33); // Send Instruction
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

  rng.uints.push(0); // Not Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ("not\n", a.instructions[0]->str());
}

TEST(ActionTest, GenerateAnd) {
  FakeRandom rng;
  Action a;

  rng.uints.push(1); // And Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ("and\n", a.instructions[0]->str());
}

TEST(ActionTest, GenerateOr) {
  FakeRandom rng;
  Action a;

  rng.uints.push(2); // Or Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ("or\n", a.instructions[0]->str());
}

TEST(ActionTest, GenerateXor) {
  FakeRandom rng;
  Action a;

  rng.uints.push(3); // Xor Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ("xor\n", a.instructions[0]->str());
}

TEST(ActionTest, GenerateLls) {
  FakeRandom rng;
  Action a;

  rng.uints.push(4); // Lls Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ("lls\n", a.instructions[0]->str());
}

TEST(ActionTest, GenerateRls) {
  FakeRandom rng;
  Action a;

  rng.uints.push(5); // Rls Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ("rls\n", a.instructions[0]->str());
}

TEST(ActionTest, GenerateAdd) {
  FakeRandom rng;
  Action a;

  rng.uints.push(6); // Add Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ("add\n", a.instructions[0]->str());
}

TEST(ActionTest, GenerateSub) {
  FakeRandom rng;
  Action a;

  rng.uints.push(7); // Sub Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ("sub\n", a.instructions[0]->str());
}

TEST(ActionTest, GenerateMul) {
  FakeRandom rng;
  Action a;

  rng.uints.push(8); // Mul Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ("mul\n", a.instructions[0]->str());
}

TEST(ActionTest, GenerateDiv) {
  FakeRandom rng;
  Action a;

  rng.uints.push(9); // Div Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ("div\n", a.instructions[0]->str());
}

TEST(ActionTest, GenerateMod) {
  FakeRandom rng;
  Action a;

  rng.uints.push(10); // Mod Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ("mod\n", a.instructions[0]->str());
}

TEST(ActionTest, GenerateRas) {
  FakeRandom rng;
  Action a;

  rng.uints.push(11); // Ras Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ("ras\n", a.instructions[0]->str());
}

TEST(ActionTest, GenerateJez) {
  FakeRandom rng;
  Action a;

  rng.uints.push(12); // Jez Instruction
  rng.uints.push(1);  // Jump Label
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ("jez $l0\n", a.instructions[0]->str());

  // Check Jump Label
  ASSERT_EQ(1, a.labels.size());
  ASSERT_EQ(1, a.labels["l0"]);
}

TEST(ActionTest, GenerateJne) {
  FakeRandom rng;
  Action a;

  rng.uints.push(13); // Jnz Instruction
  rng.uints.push(1);  // Jump Label
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ("jnz $l0\n", a.instructions[0]->str());

  // Check Jump Label
  ASSERT_EQ(1, a.labels.size());
  ASSERT_EQ(1, a.labels["l0"]);
}

TEST(ActionTest, GenerateJle) {
  FakeRandom rng;
  Action a;

  rng.uints.push(14); // Jle Instruction
  rng.uints.push(1);  // Jump Label
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ("jle $l0\n", a.instructions[0]->str());

  // Check Jump Label
  ASSERT_EQ(1, a.labels.size());
  ASSERT_EQ(1, a.labels["l0"]);
}

TEST(ActionTest, GenerateJlz) {
  FakeRandom rng;
  Action a;

  rng.uints.push(15); // Jlz Instruction
  rng.uints.push(1);  // Jump Label
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ("jlz $l0\n", a.instructions[0]->str());

  // Check Jump Label
  ASSERT_EQ(1, a.labels.size());
  ASSERT_EQ(1, a.labels["l0"]);
}

TEST(ActionTest, GenerateGoto) {
  FakeRandom rng;
  Action a;

  rng.uints.push(16); // Goto Instruction
  rng.uints.push(0);  // State
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ("goto 0\n", a.instructions[0]->str());
}

TEST(ActionTest, GeneratePush) {
  FakeRandom rng;
  Action a;

  // Push is 9x more probable

  rng.uints.push(17); // Push Instruction
  rng.uints.push(0);  // Constant
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ("push 48\n", a.instructions[0]->str());

  rng.uints.push(18); // Push Instruction
  rng.uints.push(1);  // Constant
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(2, a.instructions.size());
  ASSERT_EQ("push 49\n", a.instructions[1]->str());

  rng.uints.push(19); // Push Instruction
  rng.uints.push(2);  // Constant
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(3, a.instructions.size());
  ASSERT_EQ("push 50\n", a.instructions[2]->str());

  rng.uints.push(20); // Push Instruction
  rng.uints.push(3);  // Constant
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(4, a.instructions.size());
  ASSERT_EQ("push 51\n", a.instructions[3]->str());

  rng.uints.push(21); // Push Instruction
  rng.uints.push(4);  // Constant
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(5, a.instructions.size());
  ASSERT_EQ("push 52\n", a.instructions[4]->str());

  rng.uints.push(22); // Push Instruction
  rng.uints.push(5);  // Constant
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(6, a.instructions.size());
  ASSERT_EQ("push 53\n", a.instructions[5]->str());

  rng.uints.push(23); // Push Instruction
  rng.uints.push(6);  // Constant
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(7, a.instructions.size());
  ASSERT_EQ("push 54\n", a.instructions[6]->str());

  rng.uints.push(24); // Push Instruction
  rng.uints.push(7);  // Constant
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(8, a.instructions.size());
  ASSERT_EQ("push 55\n", a.instructions[7]->str());

  rng.uints.push(25); // Push Instruction
  rng.uints.push(8);  // Constant
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(9, a.instructions.size());
  ASSERT_EQ("push 56\n", a.instructions[8]->str());
}

TEST(ActionTest, GenerateDrop) {
  FakeRandom rng;
  Action a;

  rng.uints.push(26); // Drop Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ("drop\n", a.instructions[0]->str());
}

TEST(ActionTest, GenerateCopy) {
  FakeRandom rng;
  Action a;

  // Copy is 5x more probable

  rng.uints.push(27); // Copy Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ("copy\n", a.instructions[0]->str());

  rng.uints.push(28); // Copy Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(2, a.instructions.size());
  ASSERT_EQ("copy\n", a.instructions[1]->str());

  rng.uints.push(29); // Copy Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(3, a.instructions.size());
  ASSERT_EQ("copy\n", a.instructions[2]->str());

  rng.uints.push(30); // Copy Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(4, a.instructions.size());
  ASSERT_EQ("copy\n", a.instructions[3]->str());

  rng.uints.push(31); // Copy Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(5, a.instructions.size());
  ASSERT_EQ("copy\n", a.instructions[4]->str());
}

TEST(ActionTest, GenerateSwap) {
  FakeRandom rng;
  Action a;

  rng.uints.push(32); // Swap Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ("swap\n", a.instructions[0]->str());
}

TEST(ActionTest, GenerateSend) {
  FakeRandom rng;
  Action a;

  rng.uints.push(33); // Send Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ("send\n", a.instructions[0]->str());
}

TEST(ActionTest, Mate) {
  FakeRandom rng;
  Action a;
  rng.uints.push(0); // Not Instruction
  rng.uints.push(1); // And Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 2));
  Action b;
  rng.uints.push(3);  // Xor Instruction
  rng.uints.push(12); // Jez Instruction
  rng.uints.push(1);  // Jump Label
  ASSERT_TRUE(b.generate(rng, numeric, 1, 2));

  rng.uints.push(1); // Split Index
  Action c;
  ASSERT_TRUE(c.mate(rng, a, b));

  ASSERT_EQ(2, c.instructions.size());
  ASSERT_EQ("not\n", c.instructions[0]->str());
  ASSERT_EQ("jez $l0\n", c.instructions[1]->str());
  ASSERT_EQ(1, c.labels.size());
}

TEST(ActionTest, MateUnequalInstructionsA) {
  FakeRandom rng;
  Action a;
  rng.uints.push(0); // Not Instruction
  rng.uints.push(1); // And Instruction
  rng.uints.push(2); // Or Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 3));
  Action b;
  rng.uints.push(3); // Xor Instruction
  ASSERT_TRUE(b.generate(rng, numeric, 1, 1));

  rng.uints.push(1); // Split Index
  Action c;
  ASSERT_TRUE(c.mate(rng, a, b));

  ASSERT_EQ(3, c.instructions.size());
  ASSERT_EQ("not\n", c.instructions[0]->str());
  ASSERT_EQ("and\n", c.instructions[1]->str());
  ASSERT_EQ("or\n", c.instructions[2]->str());
  ASSERT_TRUE(c.labels.empty());
}

TEST(ActionTest, MateUnequalInstructionsB) {
  FakeRandom rng;
  Action a;
  rng.uints.push(0); // Not Instruction
  ASSERT_TRUE(a.generate(rng, numeric, 1, 1));
  Action b;
  rng.uints.push(3);  // Xor Instruction
  rng.uints.push(12); // Jez Instruction
  rng.uints.push(1);  // Jump Label
  rng.uints.push(1);  // And Instruction
  ASSERT_TRUE(b.generate(rng, numeric, 1, 3));

  rng.uints.push(1); // Split Index
  Action c;
  ASSERT_TRUE(c.mate(rng, a, b));

  ASSERT_EQ(3, c.instructions.size());
  ASSERT_EQ("not\n", c.instructions[0]->str());
  ASSERT_EQ("jez $l0\n", c.instructions[1]->str());
  ASSERT_EQ("and\n", c.instructions[2]->str());
  ASSERT_EQ(1, c.labels.size());
}

TEST(ActionTest, Mutate) {
  FakeRandom rng;
  Action a;

  // Add Instruction 0
  rng.uints.push(0);
  rng.uints.push(12); // Jez Instruction
  rng.uints.push(1);  // Jump Label
  ASSERT_TRUE(a.mutate(rng, numeric, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ("jez $l0\n", a.instructions[0]->str());
  ASSERT_EQ(1, a.labels.size());

  // Add Instruction 1
  rng.uints.push(0);
  rng.uints.push(0); // Not Instruction
  ASSERT_TRUE(a.mutate(rng, numeric, 1));

  ASSERT_EQ(2, a.instructions.size());
  ASSERT_EQ("not\n", a.instructions[1]->str());

  // Remove Instruction 0
  rng.uints.push(1);
  rng.uints.push(0); // First Instruction
  ASSERT_TRUE(a.mutate(rng, numeric, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ(0, a.labels.size());

  // Replace Instruction 0
  rng.uints.push(2);
  rng.uints.push(0); // First Instruction
  rng.uints.push(1); // And Instruction
  ASSERT_TRUE(a.mutate(rng, numeric, 1));

  ASSERT_EQ(1, a.instructions.size());
  ASSERT_EQ("and\n", a.instructions[0]->str());
}
