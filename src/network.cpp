#include <iostream>
#include <iterator>
#include <set>
#include <sstream>
#include <unordered_map>

#include <Neuro/dsl/lexer.h>
#include <Neuro/dsl/parser.h>
#include <Neuro/network.h>
#include <Neuro/random.h>

std::string Network::id() const {
  std::hash<std::string> hasher;
  std::stringstream ss;
  size_t hash = 0;
  if (emit(ss)) {
    hash = hasher(ss.str());
  }
  return std::to_string(hash);
}

bool Network::load(std::string name) {
  std::ifstream in(name);
  Lexer lexer(in);
  Parser parser(lexer);
  const auto result = parser.parseNetwork(*this);
  in.close();
  if (!result) {
    std::cerr << "Network Error: Load Failed" << std::endl;
    return false;
  }
  return true;
}

bool Network::generate(uint neurons, uint connections, uint states,
                       uint receivers, uint instructions) {

  for (int nid = 0; nid < neurons; nid++) {
    if (!generateNeuron(nid, states, receivers, instructions)) {
      return false;
    }
  }
  for (int cid = 0; cid < connections; cid++) {
    if (!generateConnection()) {
      return false;
    }
  }
  return true;
}

bool Network::generateConnection() {
  const auto ns = neurons.size();
  if (ns <= 0) {
    std::cerr << "Network Error: Network has no Neurons" << std::endl;
    return false;
  }
  const auto source = udist(rng) % ns;
  const auto destination = udist(rng) % ns;
  this->connections[source].insert(destination);
  return true;
}

bool Network::generateNeuron(uint id, uint states, uint receivers,
                             uint instructions) {
  Neuron n(id);

  if (!n.generate(states, receivers, instructions)) {
    std::cerr << "Network Error: Neuron Generation Failed" << std::endl;
    return false;
  }

  this->neurons.push_back(n);
  return true;
}

bool Network::mate(const Network &a, const Network &b) {
  std::uniform_int_distribution<uint> ndist(
      0, std::max(a.neurons.size(), b.neurons.size()));
  std::uniform_int_distribution<uint> cdist(
      0, std::max(a.connections.size(), b.connections.size()));

  auto split = ndist(rng);

  uint nid = 0;
  for (; nid < split && nid < a.neurons.size(); nid++) {
    neurons.push_back(a.neurons[nid]);
  }
  for (; nid < b.neurons.size(); nid++) {
    neurons.push_back(b.neurons[nid]);
  }

  split = cdist(rng);
  uint cid = 0;
  for (; cid < split && cid < a.connections.size(); cid++) {
    const auto cs = a.connections.find(cid);
    if (cs != a.connections.end()) {
      connections[cid].insert(cs->second.begin(), cs->second.end());
    }
  }
  for (; cid < b.connections.size(); cid++) {
    const auto cs = b.connections.find(cid);
    if (cs != b.connections.end()) {
      connections[cid].insert(cs->second.begin(), cs->second.end());
    }
  }
  return true;
}

bool Network::mutate() {
  // Default case is most likely mutation
  // Adding is twice as likely as removing
  switch (udist(rng) % 20) {
  case 0:
  case 1: {
    // Add Connection
    std::cout << "Mutation: Add Connection" << std::endl;
    if (!generateConnection()) {
      return false;
    }
    break;
  }
  case 2: {
    // Remove Connection
    std::cout << "Mutation: Remove Connection" << std::endl;
    const auto cs = connections.size();
    if (cs <= 0) {
      std::cerr << "Network Error: Network has no Connections" << std::endl;
      return false;
    }
    const auto source = udist(rng) % cs;
    std::set<uint> &set = connections[source];
    const auto ss = set.size();
    if (ss <= 0) {
      std::cerr << "Network Error: Neuron " << source << " has no Connections"
                << std::endl;
      return false;
    }
    auto itr = set.begin();
    std::advance(itr, udist(rng) % ss);
    set.erase(itr);
    break;
  }
  case 3:
  case 4: {
    // Add Neuron
    std::cout << "Mutation: Add Neuron" << std::endl;
    if (!generateNeuron(neurons.size())) {
      return false;
    }
    break;
  }
  case 5: {
    // Delete Neuron
    std::cout << "Mutation: Remove Neuron" << std::endl;
    // TODO need to update all following neuron's ids and connections
    break;
  }
  case 6:
  case 7: {
    // Add State
    std::cout << "Mutation: Add State" << std::endl;
    const auto ns = neurons.size();
    if (ns <= 0) {
      std::cerr << "Network Error: Network has no Neurons" << std::endl;
      return false;
    }
    auto n = neurons[udist(rng) % ns];
    const auto ss = n.states.size();
    State s(ss);

    if (!s.generate(ss + 1)) {
      std::cerr << "Network Error: State Generation Failed" << std::endl;
      return false;
    }

    n.states.push_back(s);
    break;
  }
  case 8: {
    // Remove State
    std::cout << "Mutation: Remove State" << std::endl;
    // TODO need to update all following state's ids and gotos
    break;
  }
  case 9:
  case 10: {
    // Add Action
    std::cout << "Mutation: Add Action" << std::endl;
    const auto ns = neurons.size();
    if (ns <= 0) {
      std::cerr << "Network Error: Network has no Neurons" << std::endl;
      return false;
    }
    auto n = neurons[udist(rng) % ns];
    const auto ss = n.states.size();
    if (ss <= 0) {
      std::cerr << "Network Error: Neuron has no States" << std::endl;
      return false;
    }
    auto s = n.states[udist(rng) % ss];
    Action a;

    if (!a.generate(ss)) {
      std::cerr << "Network Error: Action Generation Failed" << std::endl;
      return false;
    }

    s.actions[(sbyte)udist(rng)] = a;
    break;
  }
  case 11: {
    // Remove Action
    std::cout << "Mutation: Remove Action" << std::endl;

    const auto ns = neurons.size();
    if (ns <= 0) {
      std::cerr << "Network Error: Network has no Neurons" << std::endl;
      return false;
    }
    auto n = neurons[udist(rng) % ns];
    const auto ss = n.states.size();
    if (ss <= 0) {
      std::cerr << "Network Error: Neuron has no States" << std::endl;
      return false;
    }
    auto s = n.states[udist(rng) % ss];
    auto r = udist(rng);
    if (r == 0) {
      s.wildcard.reset();
    } else {
      const auto as = s.actions.size();
      if (as <= 0) {
        std::cerr << "Network Error: State has no Actions" << std::endl;
        return false;
      }
      auto itr = s.actions.begin();
      std::advance(itr, r % as);
      s.actions.erase(itr);
    }
    break;
  }
  default: {
    // Mutate Action
    std::cout << "Mutation: Mutate Action" << std::endl;
    const auto ns = neurons.size();
    if (ns <= 0) {
      std::cerr << "Network Error: Network has no Neurons" << std::endl;
      return false;
    }
    auto n = neurons[udist(rng) % ns];
    const auto ss = n.states.size();
    if (ss <= 0) {
      std::cerr << "Network Error: Neuron has no States" << std::endl;
      return false;
    }
    auto s = n.states[udist(rng) % ss];
    auto r = udist(rng);
    if (r == 0) {
      if (!s.wildcard->mutate(ss)) {
        return false;
      }
    } else {
      const auto as = s.actions.size();
      if (as <= 0) {
        std::cerr << "Network Error: State has no Actions" << std::endl;
        return false;
      }
      auto itr = s.actions.begin();
      std::advance(itr, r % as);
      if (!itr->second.mutate(ss)) {
        return false;
      }
    }
    break;
  }
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
