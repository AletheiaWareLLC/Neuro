#include <gtest/gtest.h>

#include <Neuro/instruction.h>
#include <Neuro/network.h>

TEST(InstructionTest, Not) {
  Network nn;
  Neuron n;
  State s;
  Action a;
  Not i;

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(-10);
  ASSERT_TRUE(i.execute(nn, n, s, a));

  ASSERT_EQ(1, n.stack.size());
  ASSERT_EQ(9, n.stack.top());

  // Program Counter Incremented
  ASSERT_EQ(1, a.pc);
}

TEST(InstructionTest, And) {
  Network nn;
  Neuron n;
  State s;
  Action a;
  And i;

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(-10);

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(5);

  ASSERT_TRUE(i.execute(nn, n, s, a));

  ASSERT_EQ(1, n.stack.size());
  ASSERT_EQ(4, n.stack.top());

  // Program Counter Incremented
  ASSERT_EQ(1, a.pc);
}

TEST(InstructionTest, Or) {
  Network nn;
  Neuron n;
  State s;
  Action a;
  Or i;

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(-10);

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(5);

  ASSERT_TRUE(i.execute(nn, n, s, a));

  ASSERT_EQ(1, n.stack.size());
  ASSERT_EQ(-9, n.stack.top());

  // Program Counter Incremented
  ASSERT_EQ(1, a.pc);
}

TEST(InstructionTest, Xor) {
  Network nn;
  Neuron n;
  State s;
  Action a;
  Xor i;

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(-10);

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(5);

  ASSERT_TRUE(i.execute(nn, n, s, a));

  ASSERT_EQ(1, n.stack.size());
  ASSERT_EQ(-13, n.stack.top());

  // Program Counter Incremented
  ASSERT_EQ(1, a.pc);
}

TEST(InstructionTest, Lls) {
  Network nn;
  Neuron n;
  State s;
  Action a;
  Lls i;

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(-10);
  ASSERT_TRUE(i.execute(nn, n, s, a));

  ASSERT_EQ(1, n.stack.size());
  ASSERT_EQ(-20, n.stack.top());

  // Program Counter Incremented
  ASSERT_EQ(1, a.pc);
}

TEST(InstructionTest, Rls) {
  Network nn;
  Neuron n;
  State s;
  Action a;
  Rls i;

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(-10);
  ASSERT_TRUE(i.execute(nn, n, s, a));

  ASSERT_EQ(1, n.stack.size());
  ASSERT_EQ(123, n.stack.top());

  // Program Counter Incremented
  ASSERT_EQ(1, a.pc);
}

TEST(InstructionTest, Add) {
  Network nn;
  Neuron n;
  State s;
  Action a;
  Add i;

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(-10);

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(5);

  ASSERT_TRUE(i.execute(nn, n, s, a));

  ASSERT_EQ(1, n.stack.size());
  ASSERT_EQ(-5, n.stack.top());

  // Program Counter Incremented
  ASSERT_EQ(1, a.pc);
}

TEST(InstructionTest, Sub) {
  Network nn;
  Neuron n;
  State s;
  Action a;
  Sub i;

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(-10);

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(5);

  ASSERT_TRUE(i.execute(nn, n, s, a));

  ASSERT_EQ(1, n.stack.size());
  ASSERT_EQ(-15, n.stack.top());

  // Program Counter Incremented
  ASSERT_EQ(1, a.pc);
}

TEST(InstructionTest, Mul) {
  Network nn;
  Neuron n;
  State s;
  Action a;
  Mul i;

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(-10);

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(5);

  ASSERT_TRUE(i.execute(nn, n, s, a));

  ASSERT_EQ(1, n.stack.size());
  ASSERT_EQ(-50, n.stack.top());

  // Program Counter Incremented
  ASSERT_EQ(1, a.pc);
}

TEST(InstructionTest, Div) {
  Network nn;
  Neuron n;
  State s;
  Action a;
  Div i;

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(-10);

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(5);

  ASSERT_TRUE(i.execute(nn, n, s, a));

  ASSERT_EQ(1, n.stack.size());
  ASSERT_EQ(-2, n.stack.top());

  // Program Counter Incremented
  ASSERT_EQ(1, a.pc);
}

TEST(InstructionTest, Mod) {
  Network nn;
  Neuron n;
  State s;
  Action a;
  Mod i;

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(-10);

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(5);

  ASSERT_TRUE(i.execute(nn, n, s, a));

  ASSERT_EQ(1, n.stack.size());
  ASSERT_EQ(0, n.stack.top());

  // Program Counter Incremented
  ASSERT_EQ(1, a.pc);
}

TEST(InstructionTest, Ras) {
  Network nn;
  Neuron n;
  State s;
  Action a;
  Ras i;

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(-10);
  ASSERT_TRUE(i.execute(nn, n, s, a));

  ASSERT_EQ(1, n.stack.size());
  ASSERT_EQ(-5, n.stack.top());
  // Program Counter Incremented
  ASSERT_EQ(1, a.pc);
}

TEST(InstructionTest, JumpEZ) {
  Network nn;
  Neuron n;
  State s;
  Action a;
  std::string label = "label0";
  Jump i(Jump::ConditionCode::EZ, label);

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(0);

  // Label Not Found
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(0);
  a.labels[label] = 10;

  // Branch Taken
  ASSERT_TRUE(i.execute(nn, n, s, a));

  ASSERT_EQ(10, a.pc);

  // Reset
  a.pc = 0;

  n.stack.push(1);

  // Branch Not Taken
  ASSERT_TRUE(i.execute(nn, n, s, a));

  // Program Counter Incremented
  ASSERT_EQ(1, a.pc);
}

TEST(InstructionTest, JumpNZ) {
  Network nn;
  Neuron n;
  State s;
  Action a;
  std::string label = "label0";
  Jump i(Jump::ConditionCode::NZ, label);

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(1);

  // Label Not Found
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(1);
  a.labels[label] = 10;

  // Branch Taken
  ASSERT_TRUE(i.execute(nn, n, s, a));

  ASSERT_EQ(10, a.pc);

  // Reset
  a.pc = 0;

  n.stack.push(0);

  // Branch Not Taken
  ASSERT_TRUE(i.execute(nn, n, s, a));

  // Program Counter Incremented
  ASSERT_EQ(1, a.pc);
}

TEST(InstructionTest, JumpLE) {
  Network nn;
  Neuron n;
  State s;
  Action a;
  std::string label = "label0";
  Jump i(Jump::ConditionCode::LE, label);

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(0);

  // Label Not Found
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(0);
  a.labels[label] = 10;

  // Branch Taken
  ASSERT_TRUE(i.execute(nn, n, s, a));

  ASSERT_EQ(10, a.pc);

  // Reset
  a.pc = 0;

  n.stack.push(-1);

  // Branch Taken
  ASSERT_TRUE(i.execute(nn, n, s, a));

  ASSERT_EQ(10, a.pc);

  // Reset
  a.pc = 0;

  n.stack.push(1);

  // Branch Not Taken
  ASSERT_TRUE(i.execute(nn, n, s, a));

  // Program Counter Incremented
  ASSERT_EQ(1, a.pc);
}

TEST(InstructionTest, JumpLZ) {
  Network nn;
  Neuron n;
  State s;
  Action a;
  std::string label = "label0";
  Jump i(Jump::ConditionCode::LZ, label);

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(-1);

  // Label Not Found
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(-1);
  a.labels[label] = 10;

  // Branch Taken
  ASSERT_TRUE(i.execute(nn, n, s, a));

  ASSERT_EQ(10, a.pc);

  // Reset
  a.pc = 0;

  n.stack.push(0);

  // Branch Not Taken
  ASSERT_TRUE(i.execute(nn, n, s, a));

  // Program Counter Incremented
  ASSERT_EQ(1, a.pc);

  // Reset
  a.pc = 0;

  n.stack.push(1);

  // Branch Not Taken
  ASSERT_TRUE(i.execute(nn, n, s, a));

  // Program Counter Incremented
  ASSERT_EQ(1, a.pc);
}

TEST(InstructionTest, JumpUnrecognized) {
  Network nn;
  Neuron n;
  State s;
  Action a;
  std::string label = "label0";
  Jump i((Jump::ConditionCode)10, label);

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(1);

  // Unrecognized Condition Code
  ASSERT_FALSE(i.execute(nn, n, s, a));
}

TEST(InstructionTest, Goto) {
  auto nn = std::make_unique<Network>();
  auto n = std::make_unique<Neuron>();
  auto s = std::make_unique<State>();
  auto a = std::make_unique<Action>();
  Goto i(1);

  // State Out-of-Bounds
  ASSERT_FALSE(i.execute(*nn, *n, *s, *a));

  n->states.push_back(std::move(s));
  n->states.push_back(std::make_unique<State>(1));

  ASSERT_TRUE(i.execute(*nn, *n, *s, *a));

  ASSERT_EQ(1, n->state);

  // Execution Terminated
  ASSERT_EQ(-1, a->pc);
}

TEST(InstructionTest, Push) {
  Network nn;
  Neuron n;
  State s;
  Action a;
  auto c = 100;
  Push i(c);

  for (int i = 0; i < StackLimit; i++) {
    n.stack.push(i);
  }

  // Stack Overflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  while (!n.stack.empty()) {
    n.stack.pop();
  }

  ASSERT_TRUE(i.execute(nn, n, s, a));

  ASSERT_EQ(1, n.stack.size());
  ASSERT_EQ(c, n.stack.top());

  // Program Counter Incremented
  ASSERT_EQ(1, a.pc);
}

TEST(InstructionTest, Drop) {
  Network nn;
  Neuron n;
  State s;
  Action a;
  Drop i;

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(1);

  ASSERT_TRUE(i.execute(nn, n, s, a));

  ASSERT_TRUE(n.stack.empty());

  // Program Counter Incremented
  ASSERT_EQ(1, a.pc);
}

TEST(InstructionTest, Copy) {
  Network nn;
  Neuron n;
  State s;
  Action a;
  Copy i;

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(1);

  ASSERT_TRUE(i.execute(nn, n, s, a));

  ASSERT_EQ(2, n.stack.size());
  ASSERT_EQ(1, n.stack.top());
  n.stack.pop();
  ASSERT_EQ(1, n.stack.top());

  // Program Counter Incremented
  ASSERT_EQ(1, a.pc);
}

TEST(InstructionTest, Swap) {
  Network nn;
  Neuron n;
  State s;
  Action a;
  Swap i;

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(7);

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(9);

  ASSERT_TRUE(i.execute(nn, n, s, a));

  ASSERT_EQ(2, n.stack.size());
  ASSERT_EQ(7, n.stack.top());
  n.stack.pop();
  ASSERT_EQ(9, n.stack.top());

  // Program Counter Incremented
  ASSERT_EQ(1, a.pc);
}

TEST(InstructionTest, Send) {
  Network nn;
  Neuron n;
  State s;
  Action a;
  Send i;

  // Stack Underflow
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(7);

  // No Links
  ASSERT_FALSE(i.execute(nn, n, s, a));

  n.stack.push(7);
  // Link Neuron to itself
  nn.links[0].insert(0);

  ASSERT_TRUE(i.execute(nn, n, s, a));

  ASSERT_EQ(1, nn.queue.size());
  ASSERT_EQ(0, nn.queue.front().first);
  ASSERT_EQ(7, nn.queue.front().second);

  // Program Counter Incremented
  ASSERT_EQ(1, a.pc);
}
