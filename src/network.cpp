#include <random>

#include <Neuro/network.h>

bool Network::generate(unsigned int neurons, unsigned int connections,
                      unsigned int states, unsigned int receivers,
                      unsigned int instructions) {
  std::random_device rseed;
  std::mt19937 rng(rseed());
  std::uniform_int_distribution<int> dist(std::numeric_limits<int>::min(),
                                          std::numeric_limits<int>::max());

  for (int nid = 0; nid < neurons; nid++) {
    Neuron n(nid);

    for (int sid = 0; sid < states; sid++) {
      State s(sid);

      for (int rid = 0; rid < receivers - 1; rid++) {
        Action a;
        int lid = 0;

        for (int iid = 0; iid < instructions; iid++) {
          // Generate random instructions
          switch ((unsigned int)dist(rng) % 19) {
          case 0:
            a.instructions.push_back(std::make_shared<Not>());
            break;
          case 1:
            a.instructions.push_back(std::make_shared<And>());
            break;
          case 2:
            a.instructions.push_back(std::make_shared<Or>());
            break;
          case 3:
            a.instructions.push_back(std::make_shared<Xor>());
            break;
          case 4:
            a.instructions.push_back(std::make_shared<Lls>());
            break;
          case 5:
            a.instructions.push_back(std::make_shared<Rls>());
            break;
          case 6:
            a.instructions.push_back(std::make_shared<Add>());
            break;
          case 7:
            a.instructions.push_back(std::make_shared<Sub>());
            break;
          case 8:
            a.instructions.push_back(std::make_shared<Mul>());
            break;
          case 9:
            a.instructions.push_back(std::make_shared<Div>());
            break;
          case 10:
            a.instructions.push_back(std::make_shared<Mod>());
            break;
          case 11:
            a.instructions.push_back(std::make_shared<Ras>());
            break;
          case 12: {
            auto cc = (Jump::ConditionCode)((unsigned int)dist(rng) % 4);
            auto label = "l" + std::to_string(lid++);
            a.instructions.push_back(std::make_shared<Jump>(cc, label));
            // Add label to action
            a.labels[label] = (unsigned int)dist(rng) % instructions + 1;
            break;
          }
          case 13:
            a.instructions.push_back(
                std::make_shared<Goto>((unsigned int)dist(rng) % states));
            break;
          case 14:
            a.instructions.push_back(std::make_shared<Push>((sbyte)dist(rng)));
            break;
          case 15:
            a.instructions.push_back(std::make_shared<Drop>());
            break;
          case 16:
            a.instructions.push_back(std::make_shared<Copy>());
            break;
          case 17:
            a.instructions.push_back(std::make_shared<Swap>());
            break;
          case 18:
            a.instructions.push_back(std::make_shared<Send>());
            break;
          }
        }

        if (rid == 0) {
          s.wildcard = a;
        } else {
          s.actions[(sbyte)dist(rng)] = a;
        }
      }

      n.states.push_back(s);
    }

    this->neurons.push_back(n);
  }
  for (int cid = 0; cid < connections; cid++) {
    this->connections[(unsigned int)dist(rng) % neurons].push_back(
        (unsigned int)dist(rng) % neurons);
  }
  return true;
}

void Network::reset() {
  for (Neuron &n : neurons) {
    // Revert to initial state
    n.state = 0;
    // Empty stack
    while (!n.stack.empty()) {
      n.stack.pop();
    }
    // Push stack initial value
    n.stack.push(0);
  }
  // Empty activation queue
  while (!queue.empty()) {
    queue.pop();
  }
}
