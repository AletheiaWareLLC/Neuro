#ifndef LEXER_H
#define LEXER_H

#include <fstream>
#include <queue>

#include <Neuro/network.h>

#include <Neuro/dsl/token.h>

class Lexer {
public:
  std::ifstream &source;
  std::queue<Token> queue;
  // TODO keep track of line number to improve helpfulness of error messages
  Lexer(std::ifstream &fin) : source(fin) {
    std::map<std::string, Token::Category> categories = {
        {"\n", Token::Category::NewLine},
        {"{", Token::Category::Ocb},
        {"}", Token::Category::Ccb},
        {"neuron", Token::Category::Neuron},
        {"state", Token::Category::State},
        {"receive", Token::Category::Receive},
        {"connect", Token::Category::Connect},
        {"not", Token::Category::Not},
        {"and", Token::Category::And},
        {"or", Token::Category::Or},
        {"xor", Token::Category::Xor},
        {"lls", Token::Category::Lls},
        {"rls", Token::Category::Rls},
        {"add", Token::Category::Add},
        {"sub", Token::Category::Sub},
        {"mul", Token::Category::Mul},
        {"div", Token::Category::Div},
        {"mod", Token::Category::Mod},
        {"ras", Token::Category::Ras},
        {"jez", Token::Category::Jez},
        {"jnz", Token::Category::Jnz},
        {"jle", Token::Category::Jle},
        {"jlz", Token::Category::Jlz},
        {"goto", Token::Category::Goto},
        {"push", Token::Category::Push},
        {"drop", Token::Category::Drop},
        {"copy", Token::Category::Copy},
        {"swap", Token::Category::Swap},
        {"send", Token::Category::Send},
    };

    std::string s;
    while (source >> s) {
      switch (s[0]) {
      case '#':
        queue.push(Token(Token::Category::Label, s));
        continue;
      case '/':
        if (s.size() > 1 && s[1] == '/') {
          do {
            source >> s;
          } while (s != "\n");
          continue;
        }
      }

      if (auto itr = categories.find(s); itr != categories.end()) {
        queue.push(Token(itr->second, s));
        continue;
      }

      queue.push(Token(Token::Category::Literal, s));
    }
  }
  Token peek();
  Token move();
  int match(Token::Category);
};

#endif
