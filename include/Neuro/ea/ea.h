#ifndef EA_H
#define EA_H

#include <string>
#include <vector>

#include <Neuro/action.h>
#include <Neuro/data.h>
#include <Neuro/network.h>
#include <Neuro/neuron.h>
#include <Neuro/random.h>
#include <Neuro/state.h>

class EA {
public:
  const std::set<sint> alphabet;
  const std::string fitness;
  const uint cycles;
  const uint neurons;
  const uint states;
  const uint actions;
  const uint instructions;
  const uint links;
  std::map<std::string, std::unique_ptr<Network>> population;
  std::map<std::string, uint> sizes;
  std::map<std::string, uint> errors;
  std::set<std::string> failed;

  EA(const std::set<sint> a, const std::string ff, const uint cs, const uint ns,
     const uint ss, const uint as, const uint is, const uint ls)
      : alphabet(a), fitness(ff), cycles(cs), neurons(ns), states(ss),
        actions(as), instructions(is), links(ls) {}

  bool load(const std::string directory);

  bool repopulate(Random &rng, const uint limit, uint &randoms,
                  uint &duplicates, uint &defects);

  bool evaluate(Random &rng, std::string &best, std::string &worst, uint &min,
                uint &max, float &average);

  bool kill(const std::set<std::string> &victims, uint &deaths);

  bool kill(const std::string directory, const std::set<std::string> &victims,
            uint &deaths);

  bool reproduce(Random &rng, const uint limit, std::set<std::string> &parents,
                 uint &births, uint &duplicates, uint &defects);

  bool mutate(Random &rng, uint &mutations, uint &duplicates, uint &defects);

  bool save(const std::string directory, const std::string name,
            const std::unique_ptr<Network> &network);
};

#endif
