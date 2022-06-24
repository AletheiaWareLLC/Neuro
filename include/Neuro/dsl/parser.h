#ifndef PARSER_H
#define PARSER_H

#include <Neuro/dsl/lexer.h>
#include <Neuro/network.h>

class Parser {
public:
  Lexer &lexer;
  Parser(Lexer &l) : lexer(l) {}
  int parseLiteral(int &l);
  int parseNetwork(Network &nn);
  int parseNeuron(Neuron &n);
  int parseState(State &s);
  int parseLabel(std::string &l);
  int parseLabel(Action &a, int pc);
  int parseInstruction(Action &a);
};

#endif
