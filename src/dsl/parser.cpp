#include <Neuro/dsl/parser.h>

int Parser::parseLiteral(int &l) {
  Token t = lexer.peek();
  if (t.category == Token::Category::Literal) {
    l = std::stoi(t.value);
  }
  return lexer.match(Token::Category::Literal);
}

int Parser::parseNetwork(Network &nn) {
  while (lexer.peek().category == Token::Category::Neuron) {
    Neuron n;
    auto result = parseNeuron(n);
    if (result != 0) {
      return result;
    }
    if (n.id != nn.neurons.size()) {
      std::cerr << "Error: Neuron ID Out-of-Order" << std::endl;
      return -1;
    }
    nn.neurons.push_back(n);
  }

  while (lexer.peek().category == Token::Category::Connect) {
    lexer.move();
    int source = 0;
    int destination = 0;
    auto result = parseLiteral(source);
    if (result != 0) {
      return result;
    }
    if (source < 0) {
      std::cerr << "Error: Negative Connection Source" << std::endl;
      return -1;
    }
    result = parseLiteral(destination);
    if (result != 0) {
      return result;
    }
    if (destination < 0) {
      std::cerr << "Error: Negative Connection Destination" << std::endl;
      return -1;
    }
    nn.connections[source].push_back(destination);
  }
  return 0;
}

int Parser::parseNeuron(Neuron &n) {
  auto result = lexer.match(Token::Category::Neuron);
  if (result != 0) {
    return result;
  }

  int nid;
  result = parseLiteral(nid);
  if (result != 0) {
    return result;
  }
  if (nid < 0) {
    std::cerr << "Error: Negative Neuron ID" << std::endl;
    return -1;
  }
  n.id = nid;

  result = lexer.match(Token::Category::Ocb);
  if (result != 0) {
    return result;
  }

  while (lexer.peek().category != Token::Category::Ccb) {
    State s;
    result = parseState(s);
    if (result != 0) {
      return result;
    }
    if (s.id != n.states.size()) {
      std::cerr << "Error: State ID Out-of-Order" << std::endl;
      return -1;
    }
    n.states.push_back(s);
  }
  return lexer.match(Token::Category::Ccb);
}

int Parser::parseState(State &s) {
  auto result = lexer.match(Token::Category::State);
  if (result != 0) {
    return result;
  }

  int sid;
  result = parseLiteral(sid);
  if (result != 0) {
    return result;
  }
  if (sid < 0) {
    std::cerr << "Error: Negative State ID" << std::endl;
    return -1;
  }
  s.id = sid;

  result = lexer.match(Token::Category::Ocb);
  if (result != 0) {
    return result;
  }

  // Parse Actions
  while (lexer.peek().category != Token::Category::Ccb) {
    Action a;

    result = lexer.match(Token::Category::Receive);
    if (result != 0) {
      return result;
    }

    bool wildcard = false;
    int pattern;
    if (lexer.peek().category == Token::Category::Ocb) {
      wildcard = true;
    } else {
      result = parseLiteral(pattern);
      if (result != 0) {
        return result;
      }
    }

    result = lexer.match(Token::Category::Ocb);
    if (result != 0) {
      return result;
    }

    // Parse instructions
    int pc = 0;
    while (lexer.peek().category != Token::Category::Ccb) {
      if (lexer.peek().category == Token::Category::Label) {
        result = parseLabel(a, pc);
      } else {
        result = parseInstruction(a);
        pc++;
      }
      if (result != 0) {
        return result;
      }
    }
    result = lexer.match(Token::Category::Ccb);
    if (result != 0) {
      return result;
    }

    if (wildcard) {
      s.wildcard = std::make_optional(a);
    } else {
      s.actions[pattern] = a;
    }
  }
  return lexer.match(Token::Category::Ccb);
}

int Parser::parseLabel(std::string &l) {
  Token t = lexer.move();
  l = t.value;
  return 0;
}

int Parser::parseLabel(Action &a, int pc) {
  Token t = lexer.move();
  a.labels[t.value] = pc;
  return 0;
}

int Parser::parseInstruction(Action &a) {
  Token t = lexer.move();
  std::shared_ptr<Instruction> i;
  switch (t.category) {
  case Token::Category::Not:
    i = std::make_shared<Not>();
    break;
  case Token::Category::And:
    i = std::make_shared<And>();
    break;
  case Token::Category::Or:
    i = std::make_shared<Or>();
    break;
  case Token::Category::Xor:
    i = std::make_shared<Xor>();
    break;
  case Token::Category::Lls:
    i = std::make_shared<Lls>();
    break;
  case Token::Category::Rls:
    i = std::make_shared<Rls>();
    break;
  case Token::Category::Add:
    i = std::make_shared<Add>();
    break;
  case Token::Category::Sub:
    i = std::make_shared<Sub>();
    break;
  case Token::Category::Mul:
    i = std::make_shared<Mul>();
    break;
  case Token::Category::Div:
    i = std::make_shared<Div>();
    break;
  case Token::Category::Mod:
    i = std::make_shared<Mod>();
    break;
  case Token::Category::Ras:
    i = std::make_shared<Ras>();
    break;
  case Token::Category::Jez: {
    std::string label;
    auto result = parseLabel(label);
    if (result != 0) {
      return result;
    }
    i = std::make_shared<Jump>(Jump::ConditionCode::EZ, label);
    break;
  }
  case Token::Category::Jnz: {
    std::string label;
    auto result = parseLabel(label);
    if (result != 0) {
      return result;
    }
    i = std::make_shared<Jump>(Jump::ConditionCode::NZ, label);
    break;
  }
  case Token::Category::Jle: {
    std::string label;
    auto result = parseLabel(label);
    if (result != 0) {
      return result;
    }
    i = std::make_shared<Jump>(Jump::ConditionCode::LE, label);
    break;
  }
  case Token::Category::Jlz: {
    std::string label;
    auto result = parseLabel(label);
    if (result != 0) {
      return result;
    }
    i = std::make_shared<Jump>(Jump::ConditionCode::LZ, label);
    break;
  }
  case Token::Category::Goto: {
    int state;
    auto result = parseLiteral(state);
    if (result != 0) {
      return result;
    }
    if (state < 0) {
      std::cerr << "Error: Negative Next State" << std::endl;
      return -1;
    }
    i = std::make_shared<Goto>(state);
    break;
  }
  case Token::Category::Push: {
    int constant = 0;
    auto result = parseLiteral(constant);
    if (result != 0) {
      return result;
    }
    i = std::make_shared<Push>(constant);
    break;
  }
  case Token::Category::Drop:
    i = std::make_shared<Drop>();
    break;
  case Token::Category::Copy:
    i = std::make_shared<Copy>();
    break;
  case Token::Category::Swap:
    i = std::make_shared<Swap>();
    break;
  case Token::Category::Send:
    i = std::make_shared<Send>();
    break;
  default:
    std::cerr << "Error: Unrecognized Instruction: " << t.value << std::endl;
    return -1;
  }
  a.instructions.push_back(i);
  return 0;
}
