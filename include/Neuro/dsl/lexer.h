#ifndef LEXER_H
#define LEXER_H

#include <fstream>
#include <sstream>
#include <vector>

#include <Neuro/network.h>

#include <Neuro/dsl/token.h>

class Lexer {
public:
  std::ifstream &source;
  std::vector<Token> tokens;
  int index = 0;
  int line = 1;
  Lexer(std::ifstream &fin) : source(fin) { tokenize(); }
  void tokenize();
  void add(Token &&t);
  Token peek();
  Token move();
  bool match(Token::Category);
  std::ostream &error();
};

#endif
