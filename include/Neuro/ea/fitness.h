#ifndef FITNESS_H
#define FITNESS_H

#include <fstream>
#include <string>

class Fitness {
public:
  std::ifstream source;
  uint line = 1;

  Fitness(std::string path);
  ~Fitness();

  bool next(std::string &question, std::string &answer);
};

#endif
