#ifndef RANDOM_H
#define RANDOM_H

#include <chrono>
#include <iostream>
#include <limits>
#include <queue>
#include <random>

#include <Neuro/data.h>

class Random {
public:
  Random() {}
  virtual ~Random() = default;

  virtual sint nextSignedInt() = 0;
  virtual uint nextUnsignedInt() = 0;
};

class RealRandom : public Random {
public:
  std::mt19937 rng;
  std::uniform_int_distribution<sint> sdist;
  std::uniform_int_distribution<uint> udist;

  RealRandom()
      : rng(std::chrono::system_clock::now().time_since_epoch().count()),
        sdist(std::numeric_limits<sint>::min(),
              std::numeric_limits<sint>::max()),
        udist(0, std::numeric_limits<uint>::max()) {}
  ~RealRandom() {}

  sint nextSignedInt() override { return sdist(rng); }
  uint nextUnsignedInt() override { return udist(rng); }
};

class FakeRandom : public Random {
public:
  std::queue<sint> sints;
  std::queue<uint> uints;

  FakeRandom() {}
  ~FakeRandom() {}

  sint nextSignedInt() override {
    if (sints.empty()) {
      std::cerr << "FakeRandom ran out of sints" << std::endl;
      return 0;
    }
    const auto s = sints.front();
    sints.pop();
    return s;
  }

  uint nextUnsignedInt() override {
    if (uints.empty()) {
      std::cerr << "FakeRandom ran out of uints" << std::endl;
      return 0;
    }
    const auto u = uints.front();
    uints.pop();
    return u;
  }
};

#endif
