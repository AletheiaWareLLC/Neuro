#include <iostream>
#include <map>
#include <sstream>

#include <Neuro/dsl/lexer.h>

const std::map<std::string, Token::Category> keywords = {
    {"neuron", Token::Category::Neuron}, {"state", Token::Category::State},
    {"action", Token::Category::Action}, {"link", Token::Category::Link},
    {"not", Token::Category::Not},       {"and", Token::Category::And},
    {"or", Token::Category::Or},         {"xor", Token::Category::Xor},
    {"lls", Token::Category::Lls},       {"rls", Token::Category::Rls},
    {"add", Token::Category::Add},       {"sub", Token::Category::Sub},
    {"mul", Token::Category::Mul},       {"div", Token::Category::Div},
    {"mod", Token::Category::Mod},       {"ras", Token::Category::Ras},
    {"jez", Token::Category::Jez},       {"jnz", Token::Category::Jnz},
    {"jle", Token::Category::Jle},       {"jlz", Token::Category::Jlz},
    {"goto", Token::Category::Goto},     {"push", Token::Category::Push},
    {"drop", Token::Category::Drop},     {"copy", Token::Category::Copy},
    {"swap", Token::Category::Swap},     {"send", Token::Category::Send},
};

bool Lexer::tokenize(std::istream &in) {
  char c = 0;
  in >> std::noskipws;
  while (in >> c) {
    switch (c) {
    case ' ':
    case '\t':
    case '\n':
    case '{':
    case '}':
      // Ignored for now
      break;
    case '+':
    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': {
      std::stringstream number;
      number << c;
      while (in >> c && c != ' ' && c != '\t' && c != '\n' && c != '{' &&
             c != '}') {
        number << c;
      }
      add(Token(Token::Category::Literal, number.str()));
      break;
    }
    case '#': {
      std::stringstream comment;
      while (in >> c && c != '\n') {
        comment << c;
      }
      add(Token(Token::Category::Comment, comment.str()));
      break;
    }
    case '$': {
      std::stringstream label;
      while (in >> c && c != ' ' && c != '\t' && c != '\n' && c != '{' &&
             c != '}') {
        label << c;
      }
      add(Token(Token::Category::Label, label.str()));
      break;
    }
    default: {
      std::stringstream word;
      word << c;
      while (in >> c && c != ' ' && c != '\t' && c != '\n' && c != '{' &&
             c != '}') {
        word << c;
      }
      auto w = word.str();
      if (auto itr = keywords.find(w); itr != keywords.end()) {
        add(Token(itr->second, w));
      } else {
        error() << "Unrecognized keyword: " << w << std::endl;
        return false;
      }
      break;
    }
    }
    switch (c) {
    case '\n':
      add(Token(Token::Category::NewLine, "\n"));
      break;
    case '{':
      add(Token(Token::Category::Ocb, "{"));
      break;
    case '}':
      add(Token(Token::Category::Ccb, "}"));
      break;
    }
  }
  return true;
}

void Lexer::add(Token &&t) { tokens.push_back(t); }

Token Lexer::peek() {
  int i = index;
  for (; i < tokens.size() && (tokens[i].is(Token::Category::NewLine) ||
                               tokens[i].is(Token::Category::Comment));
       i++) {
  }
  if (i >= tokens.size()) {
    return Token(Token::Category::EndOfFile, "");
  }
  return tokens[i];
}

Token Lexer::move() {
  const Token t = tokens[index];
  index++;
  if (t.is(Token::Category::NewLine)) {
    line++;
    return move();
  }
  if (t.is(Token::Category::Comment)) {
    return move();
  }
  return t;
}

bool Lexer::match(Token::Category category) {
  const Token t = move();
  if (!t.is(category)) {
    error() << "Expected: " << category << ", Got: " << t << std::endl;
    return false;
  }
  return true;
}

std::ostream &Lexer::error() {
  std::cerr << "Syntax Error: Line " << line << ": ";
  return std::cerr;
}
