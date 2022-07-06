#ifndef LEXER_H
#define LEXER_H

#include <istream>
#include <ostream>
#include <vector>

#include <Neuro/network.h>

#include <Neuro/dsl/token.h>

class Lexer {
public:
  std::vector<Token> tokens;
  uint index = 0;
  uint line = 1;

  bool tokenize(std::istream &in);

  void add(Token &&t);

  Token peek();

  Token move();

  bool match(Token::Category);

  std::ostream &error();
};

#endif
