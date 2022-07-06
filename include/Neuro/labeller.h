#ifndef LABELLER_H
#define LABELLER_H

#include <map>

#include <Neuro/data.h>

class Labeller {
public:
  std::map<std::string, uint> labels;
  uint labelIndex = 0;
};

#endif
