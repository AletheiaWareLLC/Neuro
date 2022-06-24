#include <map>

#include <Neuro/dsl/lexer.h>

Token Lexer::peek() {
  if (queue.empty()) {
    return Token(Token::Category::Eof, "");
  }
  return queue.front();
}

Token Lexer::move() {
  Token t = peek();
  if (!queue.empty()) {
    queue.pop();
  }
  return t;
}

int Lexer::match(Token::Category category) {
  Token t = move();
  if (t.category != category) {
    std::cerr << "Error: Expected: " << category << ", Got: " << t.category
              << std::endl;
    return -1;
  }
  return 0;
}