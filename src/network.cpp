#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <sstream>
#include <unordered_map>

#include <Neuro/dsl/lexer.h>
#include <Neuro/dsl/parser.h>
#include <Neuro/network.h>
#include <Neuro/random.h>

bool Network::load(std::istream &in) {
  Lexer lexer;
  if (!lexer.tokenize(in)) {
    return false;
  }
  Parser parser(lexer);
  const auto result = parser.parseNetwork(*this);
  if (!result) {
    std::cerr << "Network Error: Load Failed" << std::endl;
    return false;
  }
  return true;
}

bool Network::load(const std::string name) {
  std::ifstream in(name);
  const auto result = load(in);
  in.close();
  if (!result) {
    return false;
  }
  return true;
}

bool Network::generate(Random &rng, const std::set<sbyte> alphabet,
                       const uint neurons, const uint states,
                       const uint actions, const uint instructions,
                       const uint links) {
  for (int nid = 0; nid < neurons; nid++) {
    if (!generateNeuron(rng, alphabet, nid, states, actions, instructions)) {
      return false;
    }
  }
  for (int lid = 0; lid < links; lid++) {
    if (!generateLink(rng)) {
      return false;
    }
  }
  return true;
}

bool Network::generateNeuron(Random &rng, const std::set<sbyte> alphabet,
                             const uint id, const uint states,
                             const uint actions, const uint instructions) {
  auto n = std::make_unique<Neuron>(id);

  if (!n->generate(rng, alphabet, states, actions, instructions)) {
    std::cerr << "Network Error: Neuron Generation Failed" << std::endl;
    return false;
  }

  // Add neuron to end
  if (id == neurons.size()) {
    neurons.push_back(std::move(n));
    return true;
  }

  // Insert neuron at index

  // Update all following neuron's ids
  for (auto i = id; i < neurons.size(); i++) {
    neurons[i]->id++;
  }

  // Update links
  std::map<uint, std::set<uint>> ls;
  for (auto l : links) {
    auto k = l.first;
    if (k == id) {
      continue;
    }
    if (k > id) {
      k++;
    }
    for (auto d : l.second) {
      if (d == id) {
        continue;
      }
      if (d > id) {
        d++;
      }
      ls[k].insert(d);
    }
  }
  links.clear();
  links.insert(ls.cbegin(), ls.cend());

  auto itr = neurons.begin();
  std::advance(itr, id);
  neurons.insert(itr, std::move(n));
  return true;
}

bool Network::generateLink(Random &rng) {
  const auto ns = neurons.size();
  if (ns <= 0) {
    std::cerr << "Network Error: Network has no Neurons" << std::endl;
    return false;
  }
  const auto source = rng.nextUnsignedInt() % ns;
  const auto destination = rng.nextUnsignedInt() % ns;
  links[source].insert(destination);
  return true;
}

bool Network::mate(Random &rng, const Network &a, const Network &b) {
  const auto as = a.neurons.size();
  const auto bs = b.neurons.size();
  uint nid = 0;
  // Mate neurons shared by parents
  for (; nid < as && nid < bs; nid++) {
    const Neuron &na = *a.neurons[nid].get();
    const Neuron &nb = *b.neurons[nid].get();
    auto nc = std::make_unique<Neuron>(nid);
    if (nc->mate(rng, na, nb)) {
      neurons.push_back(std::move(nc));
    } else {
      return false;
    }
  }
  // Copy extra neurons from a
  for (; nid < as; nid++) {
    const Neuron &na = *a.neurons[nid].get();
    auto nc = std::make_unique<Neuron>();
    if (nc->duplicate(na)) {
      neurons.push_back(std::move(nc));
    } else {
      return false;
    }
  }
  // Copy extra neurons from b
  for (; nid < bs; nid++) {
    const Neuron &nb = *b.neurons[nid].get();
    auto nc = std::make_unique<Neuron>();
    if (nc->duplicate(nb)) {
      neurons.push_back(std::move(nc));
    } else {
      return false;
    }
  }

  const auto limit = std::max(a.links.size(), b.links.size());
  if (limit == 0) {
    return false;
  }
  const auto split = rng.nextUnsignedInt() % limit;
  uint lid = 0;
  // Link 0 until split are taken from A
  for (; lid < split && lid < a.links.size(); lid++) {
    auto itr = a.links.cbegin();
    std::advance(itr, lid);
    if (itr != a.links.cend()) {
      links[lid].insert(itr->second.begin(), itr->second.end());
    }
  }
  // The rest of the Links are taken from B
  for (; lid < b.links.size(); lid++) {
    auto itr = b.links.cbegin();
    std::advance(itr, lid);
    if (itr != b.links.cend()) {
      links[lid].insert(itr->second.begin(), itr->second.end());
    }
  }
  // If A has more Links than B, top up child from A
  for (; lid < a.links.size(); lid++) {
    auto itr = a.links.cbegin();
    std::advance(itr, lid);
    if (itr != a.links.cend()) {
      links[lid].insert(itr->second.begin(), itr->second.end());
    }
  }
  return true;
}

bool Network::mutate(Random &rng, const std::set<sbyte> alphabet,
                     const uint states, const uint actions,
                     const uint instructions) {
  // Default case is twice as likely
  switch (rng.nextUnsignedInt() % 10) {
  case 0: {
    // Add Neuron
    return addNeuron(rng, alphabet, states, actions, instructions);
  }
  case 1: {
    // Remove Neuron
    const auto ns = neurons.size();
    if (ns <= 1) {
      std::cerr << "Network Error: Network only has one Neuron" << std::endl;
      return false;
    }
    const auto nid = rng.nextUnsignedInt() % ns;
    return removeNeuron(nid);
  }
  case 2: {
    // Add State
    const auto ns = neurons.size();
    if (ns <= 0) {
      std::cerr << "Network Error: Network has no Neurons" << std::endl;
      return false;
    }
    auto &n = neurons[rng.nextUnsignedInt() % ns];
    return n->addState(rng, alphabet, actions, instructions);
  }
  case 3: {
    // Remove State
    const auto ns = neurons.size();
    if (ns <= 0) {
      std::cerr << "Network Error: Network has no Neurons" << std::endl;
      return false;
    }
    auto &n = neurons[rng.nextUnsignedInt() % ns];
    const auto ss = n->states.size();
    if (ss <= 0) {
      std::cerr << "Network Error: Neuron has no States" << std::endl;
      return false;
    }
    const auto sid = rng.nextUnsignedInt() % ss;
    return n->removeState(sid);
  }
  case 4: {
    // Add Action
    const auto ns = neurons.size();
    if (ns <= 0) {
      std::cerr << "Network Error: Network has no Neurons" << std::endl;
      return false;
    }
    auto &n = neurons[rng.nextUnsignedInt() % ns];
    const auto ss = n->states.size();
    if (ss <= 0) {
      std::cerr << "Network Error: Neuron has no States" << std::endl;
      return false;
    }
    auto &s = n->states[rng.nextUnsignedInt() % ss];
    return s->addAction(rng, alphabet, ss, instructions);
  }
  case 5: {
    // Remove Action
    const auto ns = neurons.size();
    if (ns <= 0) {
      std::cerr << "Network Error: Network has no Neurons" << std::endl;
      return false;
    }
    const auto &n = neurons[rng.nextUnsignedInt() % ns];
    const auto ss = n->states.size();
    if (ss <= 0) {
      std::cerr << "Network Error: Neuron has no States" << std::endl;
      return false;
    }
    auto &s = n->states[rng.nextUnsignedInt() % ss];
    return s->removeAction(rng.nextSignedInt());
  }
  case 6: {
    // Add Link
    return generateLink(rng);
  }
  case 7: {
    // Remove Link
    return removeLink(rng.nextUnsignedInt(), rng.nextUnsignedInt());
  }
  default: {
    // Mutate Action
    const auto ns = neurons.size();
    if (ns <= 0) {
      std::cerr << "Network Error: Network has no Neurons" << std::endl;
      return false;
    }
    auto &n = neurons[rng.nextUnsignedInt() % ns];
    const auto ss = n->states.size();
    if (ss <= 0) {
      std::cerr << "Network Error: Neuron has no States" << std::endl;
      return false;
    }
    auto &s = n->states[rng.nextUnsignedInt() % ss];
    const auto r = rng.nextSignedInt();
    if (r < 0 && s->wildcard) {
      if (!s->wildcard.value()->mutate(rng, alphabet, ss)) {
        return false;
      }
    } else {
      const auto as = s->actions.size();
      if (as <= 0) {
        std::cerr << "Network Error: State has no Actions" << std::endl;
        return false;
      }
      auto itr = s->actions.begin();
      std::advance(itr, r % as);
      auto &a = itr->second;
      if (!a->mutate(rng, alphabet, ss)) {
        return false;
      }
    }
    break;
  }
  }
  return true;
}

bool Network::addNeuron(Random &rng, const std::set<sbyte> alphabet,
                        const uint states, const uint actions,
                        const uint instructions) {
  const auto nid = rng.nextUnsignedInt() % (neurons.size() + 1);

  if (!generateNeuron(rng, alphabet, nid, states, actions, instructions)) {
    std::cerr << "Network Error: Neuron Generation Failed" << std::endl;
    return false;
  }

  // Link To
  links[rng.nextUnsignedInt() % neurons.size()].insert(nid);
  // Link From
  links[nid].insert(rng.nextUnsignedInt() % neurons.size());

  return true;
}

bool Network::removeNeuron(const uint id) {
  // Update all following neuron's ids
  for (auto i = id; i < neurons.size(); i++) {
    neurons[i]->id--;
  }

  // Update links
  std::map<uint, std::set<uint>> cs;
  for (auto c : links) {
    auto k = c.first;
    if (k == id) {
      continue;
    }
    if (k > id) {
      k--;
    }
    for (auto d : c.second) {
      if (d == id) {
        continue;
      }
      if (d > id) {
        d--;
      }
      cs[k].insert(d);
    }
  }
  links.clear();
  links.insert(cs.cbegin(), cs.cend());

  // Erase neuron
  auto nitr = neurons.begin();
  std::advance(nitr, id);
  neurons.erase(nitr);
  return true;
}

bool Network::removeLink(const uint sOffset, const uint dOffset) {
  const auto ls = links.size();
  if (ls <= 0) {
    std::cerr << "Network Error: Network has no Links" << std::endl;
    return false;
  }
  auto itr = links.begin();
  std::advance(itr, sOffset % ls);
  const auto source = itr->first;
  std::set<uint> &set = itr->second;
  const auto ss = set.size();
  if (ss <= 0) {
    std::cerr << "Network Error: Neuron " << source << " has no Links"
              << std::endl;
    return false;
  }
  auto sitr = set.begin();
  std::advance(sitr, dOffset % ss);
  set.erase(sitr);
  if (set.size() == 0) {
    links.erase(itr);
  }
  return true;
}

void Network::reset() {
  for (const auto &n : neurons) {
    // Revert to initial state
    n->state = 0;
    // Empty stack
    while (!n->stack.empty()) {
      n->stack.pop();
    }
    // Push stack initial value
    n->stack.push(0);
  }
  // Empty activation queue
  while (!queue.empty()) {
    queue.pop();
  }
}
