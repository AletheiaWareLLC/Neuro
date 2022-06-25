#ifndef TOKEN_H
#define TOKEN_H

#include <ostream>
#include <string>

class Token {
public:
  enum class Category {
    Literal,
    Label,
    Comment,
    // Structure
    EndOfFile,
    NewLine,
    Ocb,
    Ccb,
    Plus,
    Minus,
    Quote,
    Neuron,
    State,
    Receive,
    Connect,
    // Instructions
    Not,
    And,
    Or,
    Xor,
    Lls,
    Rls,
    Add,
    Sub,
    Mul,
    Div,
    Mod,
    Ras,
    Jez,
    Jnz,
    Jle,
    Jlz,
    Goto,
    Push,
    Drop,
    Copy,
    Swap,
    Send,
  };
  Category category;
  std::string value;

  Token(Category c, std::string v) : category(c), value(v) {}

  bool is(Category c) const;
};

std::ostream &operator<<(std::ostream &out, const Token::Category category);
std::ostream &operator<<(std::ostream &out, const Token token);

#endif
