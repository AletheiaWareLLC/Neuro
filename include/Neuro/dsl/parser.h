#ifndef PARSER_H
#define PARSER_H

#include <string>

#include <Neuro/dsl/lexer.h>
#include <Neuro/network.h>

class Parser {
public:
  Lexer &lexer;

  Parser(Lexer &l) : lexer(l) {}

  bool parseLiteral(int &l);
  bool parseNetwork(Network &nn);
  bool parseNeuron(Neuron &n);
  bool parseState(State &s);
  bool parseLabel(std::string &l);
  bool parseLabel(Action &a, int pc);
  bool parseInstruction(Action &a);
};

#endif
