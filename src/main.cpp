#include <fstream>
#include <iostream>
#include <vector>

#include <Neuro/data.h>
#include <Neuro/dsl/lexer.h>
#include <Neuro/dsl/parser.h>
#include <Neuro/network.h>
#include <Neuro/vm/vm.h>

void usage(std::string name) {
  std::cout << name << '\n';
  std::cout << "\tgenerate [options] <destination-file>\n";
  std::cout << "\trun [options] <source-file>\n";
  std::cout << "\tevolve [options] <population-directory> <fitness-function>\n";
  std::cout << "\thelp\n";
}

int generate(std::vector<std::string> options, std::string parameter) {
  int neurons = 5;
  int states = 5;
  int receivers = 5;
  int instructions = 5;
  int connections = 5;

  // TODO Parse Options
  for (auto o : options) {
    std::cerr << "Option " << o << " not supported" << std::endl;
  }

  Network nn;
  auto result =
      nn.generate(connections, neurons, states, receivers, instructions);
  if (result != 0) {
    return result;
  }

  std::ofstream destination;
  destination.open(parameter);
  nn.emit(destination);
  destination.close();
  return 0;
}

int run(std::vector<std::string> options, std::string parameter) {
  unsigned int cycles = 100;

  // TODO Parse Options
  for (auto o : options) {
    std::cerr << "Option " << o << " not supported" << std::endl;
  }

  // Parse Network
  std::ifstream source(parameter);
  Lexer lexer(source);
  Parser parser(lexer);
  Network nn;
  auto result = parser.parseNetwork(nn);
  source.close();
  if (result != 0) {
    return result;
  }

  std::cout << "Network:\n";
  nn.emit(std::cout);

  const auto ns = nn.neurons.size();

  VM vm(cycles);

  // Communication
  std::vector<sbyte> input(ns, 0);
  std::vector<sbyte> output(ns, 0);

  std::string answer;
  while (std::getline(std::cin, answer)) {
    int i = 0;
    for (auto c : answer) {
      if (i < ns) {
        input[i] = c;
        i++;
      } else {
        break;
      }
    }
    if (vm.execute(nn, input, output)) {
      for (auto o : output) {
        if (o == '\n') {
          break;
        }
        std::cout << std::bitset<8>(o);
      }
      std::cout << std::endl;
      for (auto o : output) {
        if (o == '\n') {
          break;
        }
        std::cout << o;
      }
      std::cout << std::endl;

      std::cout << "Dump;" << std::endl;
      for (auto n : nn.neurons) {
        n.dump();
      }
    } else {
      std::cout << "Fail" << std::endl;
    }
  }
  return 0;
}

int evolve(std::vector<std::string> options, std::string parameter) {
  // TODO
  return 0;
}

int main(int argc, char **argv) {
  if (argc <= 0) {
    usage("Neuro");
    return 0;
  }
  std::string name(argv[0]);
  if (argc == 1) {
    usage(name);
    return 0;
  }

  std::string command(argv[1]);

  std::vector<std::string> options;
  std::string parameter;
  for (int i = 2; i < argc; ++i) {
    if (i + 1 < argc) {
      options.push_back(argv[i]);
    } else {
      parameter = argv[i];
    }
  }

  if (command == "generate") {
    return generate(options, parameter);
  } else if (command == "run") {
    return run(options, parameter);
  } else if (command == "evolve") {
    return evolve(options, parameter);
  } else {
    usage(name);
  }
  return 0;
}
