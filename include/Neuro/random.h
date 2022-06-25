#ifndef RANDOM_H
#define RANDOM_H

#include <limits>
#include <random>

#include <Neuro/data.h>

static std::random_device rseed;
static std::mt19937 rng(rseed());
static std::uniform_int_distribution<int>
    sdist(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
static std::uniform_int_distribution<uint>
    udist(0, std::numeric_limits<uint>::max());

#endif
