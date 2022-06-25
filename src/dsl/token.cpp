#include <Neuro/dsl/token.h>
#include <iostream>

bool Token::is(Category c) const { return category == c; }

std::ostream &operator<<(std::ostream &out, const Token::Category category) {
  std::map<Token::Category, std::string> categories = {
      {Token::Category::EndOfFile, "endoffile"},
      {Token::Category::NewLine, "newline"},
      {Token::Category::Ocb, "opencurlybrace"},
      {Token::Category::Ccb, "closecurlybrace"},
      {Token::Category::Plus, "plus"},
      {Token::Category::Minus, "minus"},
      {Token::Category::Quote, "quote"},
      {Token::Category::Literal, "literal"},
      {Token::Category::Label, "label"},
      {Token::Category::Comment, "comment"},
      {Token::Category::Neuron, "neuron"},
      {Token::Category::State, "state"},
      {Token::Category::Receive, "receive"},
      {Token::Category::Connect, "connect"},
      {Token::Category::Not, "not"},
      {Token::Category::And, "and"},
      {Token::Category::Or, "or"},
      {Token::Category::Xor, "xor"},
      {Token::Category::Lls, "lls"},
      {Token::Category::Rls, "rls"},
      {Token::Category::Add, "add"},
      {Token::Category::Sub, "sub"},
      {Token::Category::Mul, "mul"},
      {Token::Category::Div, "div"},
      {Token::Category::Mod, "mod"},
      {Token::Category::Ras, "ras"},
      {Token::Category::Jez, "jez"},
      {Token::Category::Jnz, "jnz"},
      {Token::Category::Jle, "jle"},
      {Token::Category::Jlz, "jlz"},
      {Token::Category::Goto, "goto"},
      {Token::Category::Push, "push"},
      {Token::Category::Drop, "drop"},
      {Token::Category::Copy, "copy"},
      {Token::Category::Swap, "swap"},
      {Token::Category::Send, "send"},
  };
  return out << categories[category];
}

std::ostream &operator<<(std::ostream &out, const Token token) {
  return out << token.category << ":" << token.value;
}
