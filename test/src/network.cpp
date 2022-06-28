#include <sstream>

#include <gtest/gtest.h>

#include <Neuro/network.h>
#include <Neuro/random.h>
#include <Neuro/vm/vm.h>

constexpr auto network = "neuron 0 {\n"
                         "\tstate 0 {\n"
                         "\t\taction {\n"
                         "\t\t\tsend\n"
                         "\t\t}\n"
                         "\t}\n"
                         "}\n"
                         "\n"
                         "neuron 1 {\n"
                         "\tstate 0 {\n"
                         "\t\taction {\n"
                         "\t\t\tnot\n"
                         "\t\t}\n"
                         "\t}\n"
                         "}\n"
                         "\n"
                         "link 0 {1}\n"
                         "\n";
constexpr auto id = 6282755881505895648ul;

TEST(NetworkTest, EmptyID) {
  Network nn;
  std::stringstream ss;
  ASSERT_TRUE(nn.emit(ss));
  const std::hash<std::string> hasher;
  ASSERT_EQ(10599700700371220778ul, hasher(ss.str()));
}

TEST(NetworkTest, LoadID) {
  std::istringstream in(network);
  Network nn;
  ASSERT_TRUE(nn.load(in));

  std::stringstream ss;
  ASSERT_TRUE(nn.emit(ss));
  const std::hash<std::string> hasher;
  ASSERT_EQ(id, hasher(ss.str()));
}

TEST(NetworkTest, LoadEmit) {
  std::istringstream in(network);
  Network nn;
  ASSERT_TRUE(nn.load(in));

  std::stringstream ss;
  nn.emit(ss);
  ASSERT_EQ(network, ss.str());
}

TEST(NetworkTest, Generate) {
  FakeRandom rng;
  rng.sints.push(-1); // Wildcard Action
  rng.sints.push(-1); // Wildcard Action
  rng.uints.push(33); // Send Instruction
  rng.uints.push(0);  // Not Instruction
  rng.uints.push(0);  // Link Source
  rng.uints.push(1);  // Link Destination

  const auto neurons = 2;
  const auto states = 1;
  const auto actions = 1;
  const auto instructions = 1;
  const auto links = 1;
  Network nn;
  ASSERT_TRUE(nn.generate(rng, neurons, states, actions, instructions, links));

  std::stringstream ss;
  ASSERT_TRUE(nn.emit(ss));
  ASSERT_EQ(network, ss.str());
}

TEST(NetworkTest, RemoveNeuron) {
  std::istringstream in("neuron 0 {state 0 {action {send}}}"
                        "neuron 1 {state 0 {action {or}}}"
                        "neuron 2 {state 0 {action {xor}}}"
                        "link 0 {1}"
                        "link 1 {1}"
                        "link 1 {2}");
  Network nn;
  ASSERT_TRUE(nn.load(in));
  ASSERT_EQ(3, nn.neurons.size());
  ASSERT_EQ(2, nn.links.size());
  ASSERT_EQ(1, nn.links[0].size());
  ASSERT_EQ(2, nn.links[1].size());

  ASSERT_TRUE(nn.removeNeuron(1));
  ASSERT_EQ(2, nn.neurons.size());
  ASSERT_EQ(0, nn.neurons[0]->id); // Neuron 0 should remain Neuron 0
  ASSERT_EQ(1, nn.neurons[1]->id); // Neuron 2 should become Neuron 1
  ASSERT_TRUE(nn.links.empty());
}

TEST(NetworkTest, RemoveLink) {
  std::istringstream in("neuron 0 {state 0 {action {send}}}"
                        "neuron 1 {state 0 {action {or}}}"
                        "neuron 2 {state 0 {action {xor}}}"
                        "link 0 {1}"
                        "link 1 {1}"
                        "link 1 {2}");
  Network nn;
  ASSERT_TRUE(nn.load(in));
  ASSERT_EQ(3, nn.neurons.size());
  ASSERT_EQ(2, nn.links.size());
  ASSERT_EQ(1, nn.links[0].size());
  ASSERT_EQ(2, nn.links[1].size());

  ASSERT_TRUE(nn.removeLink(0, 0));
  ASSERT_EQ(3, nn.neurons.size());
  ASSERT_EQ(1, nn.links.size());
  ASSERT_EQ(2, nn.links[1].size());
}

TEST(NetworkTest, Mate) {
  FakeRandom rng;
  rng.uints.push(1); // Neuron 0 State 0 Wildcard Instruction Split
  rng.uints.push(0); // Neuron 1 State 0 Wildcard Instruction Split
  rng.uints.push(0); // Link Split (0 from A, rest from B)

  std::istringstream ain("neuron 0 {state 0 {action 0 {send}}}"
                         "neuron 1 {state 0 {action 1 {or}}}"
                         "link 1 {0}");
  Network an;
  ASSERT_TRUE(an.load(ain));

  std::istringstream bin("neuron 0 {state 0 {action 2 {and}}}"
                         "neuron 1 {state 0 {action 3 {not}}}"
                         "link 0 {1}");
  Network bn;
  ASSERT_TRUE(bn.load(bin));
  Network cn;
  ASSERT_TRUE(cn.mate(rng, an, bn));
  std::stringstream ss;
  ASSERT_TRUE(cn.emit(ss));
  ASSERT_EQ("neuron 0 {\n"
            "\tstate 0 {\n"
            "\t\taction 0 {\n"
            "\t\t\tsend\n"
            "\t\t}\n"
            "\t}\n"
            "}\n"
            "\n"
            "neuron 1 {\n"
            "\tstate 0 {\n"
            "\t\taction 1 {\n"
            "\t\t\tnot\n"
            "\t\t}\n"
            "\t}\n"
            "}\n"
            "\n"
            "link 0 {1}\n"
            "\n",
            ss.str());
}

TEST(NetworkTest, MateUnequalNeuronsA) {
  FakeRandom rng;
  rng.uints.push(1); // Neuron Split (1 from A, rest from B)
  rng.uints.push(0); // Link Split (0 from A, rest from B)

  std::istringstream ain("neuron 0 {state 0 {action {send}}}"
                         "neuron 1 {state 0 {action {or}}}"
                         "link 1 {0}");
  Network an;
  ASSERT_TRUE(an.load(ain));

  std::istringstream bin("neuron 0 {state 0 {action {and}}}"
                         "link 0 {0}");
  Network bn;
  ASSERT_TRUE(bn.load(bin));
  Network cn;
  ASSERT_TRUE(cn.mate(rng, an, bn));
  ASSERT_EQ(2, cn.neurons.size());
  ASSERT_EQ(1, cn.links.size());
}

TEST(NetworkTest, MateUnequalNeuronsB) {
  FakeRandom rng;
  rng.uints.push(1); // Neuron Split (1 from A, rest from B)
  rng.uints.push(0); // Link Split (0 from A, rest from B)

  std::istringstream ain("neuron 0 {state 0 {action {send}}}"
                         "link 0 {0}");
  Network an;
  ASSERT_TRUE(an.load(ain));

  std::istringstream bin("neuron 0 {state 0 {action {and}}}"
                         "neuron 1 {state 0 {action {not}}}"
                         "link 0 {1}");
  Network bn;
  ASSERT_TRUE(bn.load(bin));
  Network cn;
  ASSERT_TRUE(cn.mate(rng, an, bn));
  ASSERT_EQ(2, cn.neurons.size());
  ASSERT_EQ(1, cn.links.size());
}

TEST(NetworkTest, MateUnequalLinksA) {
  FakeRandom rng;
  rng.uints.push(1); // Neuron Split (1 from A, rest from B)
  rng.uints.push(0); // Link Split (0 from A, rest from B)

  std::istringstream ain("neuron 0 {state 0 {action {send}}}"
                         "neuron 1 {state 0 {action {or}}}"
                         "link 1 {0}");
  Network an;
  ASSERT_TRUE(an.load(ain));

  std::istringstream bin("neuron 0 {state 0 {action {and}}}"
                         "neuron 1 {state 0 {action {not}}}");
  Network bn;
  ASSERT_TRUE(bn.load(bin));
  Network cn;
  ASSERT_TRUE(cn.mate(rng, an, bn));
  ASSERT_EQ(2, cn.neurons.size());
  ASSERT_EQ(1, cn.links.size());
}

TEST(NetworkTest, MateUnequalLinksB) {
  FakeRandom rng;
  rng.uints.push(1); // Neuron Split (1 from A, rest from B)
  rng.uints.push(0); // Link Split (0 from A, rest from B)

  std::istringstream ain("neuron 0 {state 0 {action {send}}}"
                         "neuron 1 {state 0 {action {or}}}");
  Network an;
  ASSERT_TRUE(an.load(ain));

  std::istringstream bin("neuron 0 {state 0 {action {and}}}"
                         "neuron 1 {state 0 {action {not}}}"
                         "link 0 {1}");
  Network bn;
  ASSERT_TRUE(bn.load(bin));
  Network cn;
  ASSERT_TRUE(cn.mate(rng, an, bn));
  ASSERT_EQ(2, cn.neurons.size());
  ASSERT_EQ(1, cn.links.size());
}

TEST(NetworkTest, Mutate) {
  FakeRandom rng;

  Network nn;

  // Adding
  rng.uints.push(0);                 // Add Neuron
  rng.uints.push(nn.neurons.size()); // At the End
  rng.sints.push(-1);                // Wildcard Action
  rng.uints.push(33);                // Send Instruction
  rng.uints.push(0);                 // Link from Self from Self
  rng.uints.push(0);                 // Link to Self from Self
  ASSERT_TRUE(nn.mutate(rng, 1, 1, 1));

  ASSERT_EQ(1, nn.neurons.size());
  auto &n0 = *nn.neurons[0];
  ASSERT_EQ(1, n0.states.size());
  auto &n0s0 = *n0.states[0];
  ASSERT_TRUE(n0s0.wildcard);
  ASSERT_EQ(0, n0s0.actions.size());
  ASSERT_EQ(1, nn.links.size());
  ASSERT_EQ(0, *nn.links[0].begin());

  rng.uints.push(0);                 // Add Neuron
  rng.uints.push(nn.neurons.size()); // At the End
  rng.sints.push(-1);                // Wildcard Action
  rng.uints.push(16);                // Goto Instruction
  rng.uints.push(1);                 // Goto State ID
  rng.uints.push(1);                 // Link from 1 to Self
  rng.uints.push(0);                 // Link to 0 from Self
  ASSERT_TRUE(nn.mutate(rng, 1, 1, 1));

  ASSERT_EQ(2, nn.neurons.size());
  auto &n1 = *nn.neurons[1];
  ASSERT_EQ(1, n1.states.size());
  auto &n1s0 = *n1.states[0];
  ASSERT_TRUE(n1s0.wildcard);
  ASSERT_EQ(0, n1s0.actions.size());
  ASSERT_EQ(2, nn.links.size());
  ASSERT_EQ(1, nn.links[0].size());
  ASSERT_EQ(0, *nn.links[0].begin());
  ASSERT_EQ(2, nn.links[1].size());
  auto itr = nn.links[1].begin();
  ASSERT_EQ(0, *itr);
  itr++;
  ASSERT_EQ(1, *itr);

  // Both Neurons have a single State with a Wildcard Action

  rng.uints.push(2);                // Add State
  rng.uints.push(0);                // Neuron ID
  rng.uints.push(n0.states.size()); // At the End
  rng.sints.push(10);               // Pattern Action
  rng.uints.push(0);                // Not Instruction
  ASSERT_TRUE(nn.mutate(rng, 1, 1, 1));

  ASSERT_EQ(2, n0.states.size());
  auto &n0s1 = *n0.states[1];
  ASSERT_FALSE(n0s1.wildcard);
  ASSERT_EQ(1, n0s1.actions.size());
  ASSERT_EQ(10, n0s1.actions.begin()->first);

  rng.uints.push(2);                // Add State
  rng.uints.push(1);                // Neuron ID
  rng.uints.push(n1.states.size()); // At the End
  rng.sints.push(11);               // Pattern Action
  rng.uints.push(2);                // Or Instruction
  ASSERT_TRUE(nn.mutate(rng, 1, 1, 1));

  ASSERT_EQ(2, n1.states.size());
  auto &n1s1 = *n1.states[1];
  ASSERT_FALSE(n1s1.wildcard);
  ASSERT_EQ(1, n1s1.actions.size());
  ASSERT_EQ(11, n1s1.actions.begin()->first);

  // Both Neurons have two States, State 0 with a Wildcard Action, and State 1
  // with a Pattern Action (10, 11 resp.)

  rng.uints.push(4);  // Add Action
  rng.uints.push(0);  // Neuron ID
  rng.uints.push(0);  // State ID
  rng.sints.push(20); // Pattern Action
  rng.uints.push(3);  // Xor Instruction
  ASSERT_TRUE(nn.mutate(rng, 1, 1, 1));
  ASSERT_EQ(1, n0s0.actions.size());
  ASSERT_EQ(20, n0s0.actions.begin()->first);

  rng.uints.push(4);  // Add Action
  rng.uints.push(1);  // Neuron ID
  rng.uints.push(0);  // State ID
  rng.sints.push(21); // Pattern Action
  rng.uints.push(4);  // Lls Instruction
  ASSERT_TRUE(nn.mutate(rng, 1, 1, 1));
  ASSERT_EQ(1, n1s0.actions.size());
  ASSERT_EQ(21, n1s0.actions.begin()->first);

  // Both Neurons have two States, State 0 with a Wildcard Action and a Pattern
  // Action (20, 21 resp), and State 1 with a Pattern Action (10, 11 resp.)

  rng.uints.push(6); // Add Link
  rng.uints.push(0); // Source Neuron
  rng.uints.push(1); // Destination Neuron
  ASSERT_TRUE(nn.mutate(rng, 1, 1, 1));
  ASSERT_EQ(2, nn.links.size());
  ASSERT_EQ(2, nn.links[0].size());
  itr = nn.links[0].begin();
  ASSERT_EQ(0, *itr);
  itr++;
  ASSERT_EQ(1, *itr);
  ASSERT_EQ(2, nn.links[1].size());
  itr = nn.links[1].begin();
  ASSERT_EQ(0, *itr);
  itr++;
  ASSERT_EQ(1, *itr);

  // Both Neurons have a link to each other.

  // Mutate
  rng.uints.push(8); // Mutate Action
  rng.uints.push(0); // Neuron ID
  rng.uints.push(1); // State ID
  rng.sints.push(0); // Pattern Action (First Action in Map)
  rng.uints.push(0); // Mutation Type
  rng.uints.push(0); // Not Instruction
  ASSERT_TRUE(nn.mutate(rng, 1, 1, 1));
  auto &n0s1a10 = *n0s1.actions[10];
  ASSERT_EQ(2, n0s1a10.instructions.size());

  // Removing
  rng.uints.push(7); // Remove Link
  rng.uints.push(0); // First Source in Map
  rng.uints.push(0); // First Destination in Set
  ASSERT_TRUE(nn.mutate(rng, 1, 1, 1));
  ASSERT_EQ(2, nn.links.size());
  ASSERT_EQ(1, nn.links[0].size());
  ASSERT_EQ(1, *nn.links[0].begin());
  ASSERT_EQ(2, nn.links[1].size());
  itr = nn.links[1].begin();
  ASSERT_EQ(0, *itr);
  itr++;
  ASSERT_EQ(1, *itr);

  rng.uints.push(5);  // Remove Action
  rng.uints.push(0);  // Neuron ID
  rng.uints.push(0);  // State ID
  rng.sints.push(-1); // Wildcard Action
  ASSERT_TRUE(nn.mutate(rng, 1, 1, 1));
  ASSERT_FALSE(n0s0.wildcard);

  rng.uints.push(5);  // Remove Action
  rng.uints.push(1);  // Neuron ID
  rng.uints.push(0);  // State ID
  rng.sints.push(21); // Pattern Action
  ASSERT_TRUE(nn.mutate(rng, 1, 1, 1));
  ASSERT_TRUE(n1s0.actions.empty());

  rng.uints.push(3); // Remove State
  rng.uints.push(0); // Neuron ID
  rng.uints.push(0); // State ID
  ASSERT_TRUE(nn.mutate(rng, 1, 1, 1));
  ASSERT_EQ(1, n0.states.size());
  ASSERT_EQ(0, n1s0.id);
  ASSERT_TRUE(n1s0.wildcard);
  auto go2 =
      std::dynamic_pointer_cast<Goto>(n1s0.wildcard.value()->instructions[0]);
  ASSERT_NE(nullptr, go2);
  ASSERT_EQ(0, go2->state);

  rng.uints.push(1); // Remove Neuron
  rng.uints.push(0); // Neuron ID
  ASSERT_TRUE(nn.mutate(rng, 1, 1, 1));
  ASSERT_EQ(1, nn.neurons.size());
  auto &n0new = *nn.neurons[0];
  ASSERT_EQ(0, n0new.id);

  ASSERT_EQ(1, nn.links.size());
  ASSERT_EQ(1, nn.links[0].size());
  ASSERT_EQ(0, *nn.links[0].begin());
}

TEST(NetworkTest, Reset) {
  auto n = std::make_unique<Neuron>();
  n->state = 1;
  n->stack.push(1);
  Network nn;
  nn.neurons.push_back(std::move(n));
  nn.queue.push(std::make_pair<sbyte, uint>(7, 0));

  nn.reset();

  auto &n0 = *nn.neurons[0];
  ASSERT_EQ(0, n0.state);
  ASSERT_EQ(1, n0.stack.size());
  ASSERT_EQ(0, n0.stack.top());

  ASSERT_TRUE(nn.queue.empty());
}

TEST(NetworkTest, EndToEnd_SendNot) {
  std::istringstream in(network);
  Network nn;
  ASSERT_TRUE(nn.load(in));

  VM vm(10);

  std::vector<sbyte> input{0, 1, 1, 0, 1};

  std::vector<sbyte> output;

  uint c = 0;
  ASSERT_TRUE(vm.execute(nn, input, output, c));

  ASSERT_EQ(10, c);
  ASSERT_EQ(5, output.size());

  ASSERT_EQ(-1, output[0]);
  ASSERT_EQ(-2, output[1]);
  ASSERT_EQ(-2, output[2]);
  ASSERT_EQ(-1, output[3]);
  ASSERT_EQ(-2, output[4]);
}
