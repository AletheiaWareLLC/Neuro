#ifndef TOKEN_H
#define TOKEN_H

#include <map>
#include <string>

class Token {
public:
  enum class Category {
    Literal,
    Label,
    Comment,
    // Structure
    Eof,
    NewLine,
    Ocb,
    Ccb,
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
};

std::ostream &operator<<(std::ostream &out, const Token::Category category);

#endif
