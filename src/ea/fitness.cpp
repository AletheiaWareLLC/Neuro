#include <iostream>

#include <Neuro/ea/fitness.h>

Fitness::Fitness(std::string path) : source(path) {}

Fitness::~Fitness() { source.close(); }

bool Fitness::next(std::string &question, std::string &answer) {
  char c;
  if (!(source >> c)) {
    // End of File
    return false;
  }
  if (c != '>') {
    std::cerr << "Fitness Function Error: Expected: '>', Got: " << c
              << std::endl;
    return false;
  }
  if (!std::getline(source, question)) {
    std::cerr << "Fitness Function Error: Failed to Read Question at Line: "
              << line << std::endl;
    return false;
  }
  line++;
  if (source >> c && c != '<') {
    std::cerr << "Fitness Function Error: Expected: '<', Got: " << c
              << std::endl;
    return false;
  }
  if (!std::getline(source, answer)) {
    std::cerr << "Fitness Function Error: Failed to Read Answer at Line: "
              << line << std::endl;
    return false;
  }
  line++;
  std::string empty;
  if (!std::getline(source, empty) || !empty.empty()) {
    std::cerr << "Fitness Function Error: Expected Empty Line, Got: " << empty
              << std::endl;
    return false;
  }
  line++;
  return true;
}
