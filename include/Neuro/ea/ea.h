#ifndef EA_H
#define EA_H

#include <string>
#include <vector>

#include <Neuro/action.h>
#include <Neuro/network.h>
#include <Neuro/neuron.h>
#include <Neuro/state.h>

class EA {
public:
  const std::string directory;
  const std::string fitness;
  const uint maxNeurons;
  const uint maxConnections;
  const uint maxIndividuals;
  const uint maxGenerations;
  const uint maxCycles;

  EA(const std::string dir, const std::string ff, const uint ns, const uint cs,
     const uint il, const uint gl, const uint cl)
      : directory(dir), fitness(ff), maxNeurons(ns), maxConnections(cs),
        maxIndividuals(il), maxGenerations(gl), maxCycles(cl) {}

  bool evolve();
};

#endif
