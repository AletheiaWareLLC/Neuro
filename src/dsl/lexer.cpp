#include <map>

#include <Neuro/dsl/lexer.h>

const std::map<std::string, Token::Category> keywords = {
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

void Lexer::tokenize() {
  char c = 0;
  source >> std::noskipws;
  while (source >> c) {
    switch (c) {
    case ' ':
    case '\t':
    case '\n':
      // Ignored
      break;
    case '{':
      add(Token(Token::Category::Ocb, "{"));
      break;
    case '}':
      add(Token(Token::Category::Ccb, "}"));
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
      while (source >> c && c != '\n' && c != '\t' && c != ' ') {
        number << c;
      }
      add(Token(Token::Category::Literal, number.str()));
      break;
    }
    case '\'': {
      std::stringstream comment;
      while (source >> c && c != '\n') {
        comment << c;
      }
      add(Token(Token::Category::Comment, comment.str()));
      break;
    }
    case '#': {
      std::stringstream label;
      while (source >> c && c != '\n' && c != '\t' && c != ' ') {
        label << c;
      }
      add(Token(Token::Category::Label, label.str()));
      break;
    }
    default: {
      std::stringstream word;
      word << c;
      while (source >> c && c != '\n' && c != '\t' && c != ' ') {
        word << c;
      }
      auto w = word.str();
      if (auto itr = keywords.find(w); itr != keywords.end()) {
        add(Token(itr->second, w));
      } else {
        // TODO
      }
      break;
    }
    }
    if (c == '\n') {
      add(Token(Token::Category::NewLine, "\n"));
    }
  }
}

void Lexer::add(Token &&t) { tokens.push_back(t); }

Token Lexer::peek() {
  int i = index;
  for (; i < tokens.size() && tokens[i].is(Token::Category::NewLine); i++) {
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
