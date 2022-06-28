#include <iostream>

#include <Neuro/dsl/parser.h>

bool Parser::parseLiteral(int &l) {
  const Token t = lexer.peek();
  if (t.is(Token::Category::Literal)) {
    l = std::stoi(t.value);
  }
  return lexer.match(Token::Category::Literal);
}

bool Parser::parseNetwork(Network &nn) {
  while (lexer.peek().is(Token::Category::Neuron)) {
    auto n = std::make_unique<Neuron>();

    if (!parseNeuron(*n)) {
      return false;
    }
    if (n->id != nn.neurons.size()) {
      lexer.error() << "Neuron ID Out-of-Order" << std::endl;
      return false;
    }
    nn.neurons.push_back(std::move(n));
  }

  while (lexer.peek().is(Token::Category::Link)) {
    lexer.move();
    int source = 0;
    if (!parseLiteral(source)) {
      return false;
    }
    if (source < 0) {
      lexer.error() << "Negative Link Source" << std::endl;
      return false;
    }
    if (source >= nn.neurons.size()) {
      lexer.error() << "Link Source Out-of-Bounds" << std::endl;
      return false;
    }
    if (!lexer.match(Token::Category::Ocb)) {
      return false;
    }
    while (!lexer.peek().is(Token::Category::Ccb)) {
      int destination = 0;
      if (!parseLiteral(destination)) {
        return false;
      }
      if (destination < 0) {
        lexer.error() << "Negative Link Destination" << std::endl;
        return false;
      }
      if (destination >= nn.neurons.size()) {
        lexer.error() << "Link Destination Out-of-Bounds" << std::endl;
        return false;
      }
      nn.links[source].insert(destination);
    }
    if (!lexer.match(Token::Category::Ccb)) {
      return false;
    }
  }
  return true;
}

bool Parser::parseNeuron(Neuron &n) {
  if (!lexer.match(Token::Category::Neuron)) {
    return false;
  }

  int nid;
  if (!parseLiteral(nid)) {
    return false;
  }
  if (nid < 0) {
    lexer.error() << "Negative Neuron ID" << std::endl;
    return false;
  }
  n.id = nid;

  if (!lexer.match(Token::Category::Ocb)) {
    return false;
  }

  while (!lexer.peek().is(Token::Category::Ccb)) {
    auto s = std::make_unique<State>();
    if (!parseState(*s)) {
      return false;
    }
    if (s->id != n.states.size()) {
      lexer.error() << "State ID Out-of-Order" << std::endl;
      return false;
    }
    n.states.push_back(std::move(s));
  }
  return lexer.match(Token::Category::Ccb);
}

bool Parser::parseState(State &s) {
  if (!lexer.match(Token::Category::State)) {
    return false;
  }

  int sid;
  if (!parseLiteral(sid)) {
    return false;
  }
  if (sid < 0) {
    lexer.error() << "Negative State ID" << std::endl;
    return false;
  }
  s.id = sid;

  if (!lexer.match(Token::Category::Ocb)) {
    return false;
  }

  // Parse Actions
  while (!lexer.peek().is(Token::Category::Ccb)) {
    auto a = std::make_unique<Action>();

    if (!lexer.match(Token::Category::Action)) {
      return false;
    }

    bool wildcard = false;
    int pattern;
    if (lexer.peek().is(Token::Category::Ocb)) {
      wildcard = true;
    } else {
      if (!parseLiteral(pattern)) {
        return false;
      }
    }

    if (!lexer.match(Token::Category::Ocb)) {
      return false;
    }

    // Parse instructions
    int pc = 0;
    while (!lexer.peek().is(Token::Category::Ccb)) {
      if (lexer.peek().is(Token::Category::Label)) {
        if (!parseLabel(*a, pc)) {
          return false;
        }
      } else {
        if (!parseInstruction(*a)) {
          return false;
        }
        pc++;
      }
    }
    if (!lexer.match(Token::Category::Ccb)) {
      return false;
    }

    if (wildcard) {
      s.wildcard = std::move(a);
    } else {
      s.actions[pattern] = std::move(a);
    }
  }
  return lexer.match(Token::Category::Ccb);
}

bool Parser::parseLabel(std::string &l) {
  const Token t = lexer.move();
  l = t.value;
  return true;
}

bool Parser::parseLabel(Action &a, int pc) {
  const Token t = lexer.move();
  a.labels[t.value] = pc;
  return true;
}

bool Parser::parseInstruction(Action &a) {
  const Token t = lexer.move();
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
    if (!parseLabel(label)) {
      return false;
    }
    i = std::make_shared<Jump>(Jump::ConditionCode::EZ, label);
    break;
  }
  case Token::Category::Jnz: {
    std::string label;
    if (!parseLabel(label)) {
      return false;
    }
    i = std::make_shared<Jump>(Jump::ConditionCode::NZ, label);
    break;
  }
  case Token::Category::Jle: {
    std::string label;
    if (!parseLabel(label)) {
      return false;
    }
    i = std::make_shared<Jump>(Jump::ConditionCode::LE, label);
    break;
  }
  case Token::Category::Jlz: {
    std::string label;
    if (!parseLabel(label)) {
      return false;
    }
    i = std::make_shared<Jump>(Jump::ConditionCode::LZ, label);
    break;
  }
  case Token::Category::Goto: {
    int state;
    if (!parseLiteral(state)) {
      return false;
    }
    if (state < 0) {
      lexer.error() << "Negative Next State" << std::endl;
      return false;
    }
    i = std::make_shared<Goto>(state);
    break;
  }
  case Token::Category::Push: {
    int constant = 0;
    if (!parseLiteral(constant)) {
      return false;
    }
    // Ensure constant in within sbyte range
    if (constant < -128 || constant > 127) {
      lexer.error() << "Constant Out-of-Range: " << constant << std::endl;
      return false;
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
    lexer.error() << "Unrecognized Instruction: " << t << std::endl;
    return false;
  }
  a.instructions.push_back(i);
  return true;
}
