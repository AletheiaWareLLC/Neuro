#include <fstream>
#include <iostream>
#include <vector>

#include <Neuro/data.h>
#include <Neuro/ea/ea.h>
#include <Neuro/network.h>
#include <Neuro/vm/vm.h>

void usage(std::string name) {
  std::cout << name << '\n';
  std::cout << "\tgenerate [options] <path-to-destination-file>\n";
  std::cout << "\trun [options] <path-to-source-file>\n";
  std::cout << "\tevolve [options] <path-to-fitness-function>\n";
  std::cout << "\thelp\n";
}

void usage() { usage("Neuro"); }

void help(std::string name, std::string command) {
  if (command == "generate") {
    std::cout << "Generates a new random neural network.\n\n";
    std::cout << "Options;";
    std::cout << "\n\t--neurons\n\t\tNumber of Neurons (default 5)\n";
    std::cout << "\n\t--connections\n\t\tNumber of Connections per Neuron "
                 "(default 5)\n";
    std::cout << "\n\t--states\n\t\tNumber of States per Neuron (default 5)\n";
    std::cout << "\n\t--actions\n\t\tNumber of Actions per State (default 5)\n";
    std::cout << "\n\t--instructions\n\t\tNumber of Instructions per Action "
                 "(default 5)\n";
  } else if (command == "run") {
    std::cout << "Runs a neural network.\n\n";
    std::cout << "Options;";
    std::cout << "\n\t--input\n\t\tPath to Input (default read from stdin)\n";
    std::cout << "\n\t--ouput\n\t\tPath to Ouput (default write to stdout)\n";
    std::cout << "\n\t--cycles\n\t\tCycle Limit (default 100)\n";
  } else if (command == "evolve") {
    std::cout << "Evolves a population of neural networks\n\n";
    std::cout << "Options;";
    std::cout
        << "\n\t--directory\n\t\tPopulation Directory (default ./population)\n";
    std::cout << "\n\t--population\n\t\tPopulation Limit (default 100)\n";
    std::cout << "\n\t--generation\n\t\tGeneration Limit (default 100)\n";
    std::cout << "\n\t--cycles\n\t\tCycle Limit (default 100)\n";
    std::cout
        << "\n\t--mutation\n\t\tMutation Rate (default 1 per generation)\n";
  } else {
    usage();
  }
}

std::ostream &error() {
  std::cerr << "Command Error: ";
  return std::cerr;
}

bool generate(std::vector<std::string> options, std::string parameter) {
  uint neurons = 5;
  uint states = 5;
  uint receivers = 5;
  uint instructions = 5;
  uint connections = 5;

  // TODO Parse Options
  for (const auto o : options) {
    std::cerr << "Option " << o << " not supported" << std::endl;
  }

  if (parameter.size() == 0) {
    error() << "Missing <path-to-destination-file> parameter\n";
    return -1;
  }

  Network nn;
  if (!nn.generate(neurons, connections, states, receivers, instructions)) {
    return -1;
  }

  std::ofstream destination;
  destination.open(parameter);
  auto result = nn.emit(destination);
  destination.close();
  if (!result) {
    return -1;
  }
  return 0;
}

bool run(std::vector<std::string> options, std::string parameter) {
  uint cycles = 100;

  // TODO Parse Options
  for (const auto o : options) {
    std::cerr << "Option " << o << " not supported" << std::endl;
  }

  if (parameter.size() == 0) {
    error() << "Missing <path-to-source-file> parameter\n";
    return -1;
  }

  Network nn;

  // Parse Network
  if (!nn.load(parameter)) {
    return -1;
  }

  std::cout << "Network:\n";
  nn.emit(std::cout);

  const auto ns = nn.neurons.size();

  VM vm(cycles);

  // Communication
  std::vector<sbyte> input;
  std::vector<sbyte> output;

  std::string question;
  while (std::getline(std::cin, question)) {
    int i = 0;
    for (const auto c : question) {
      input.push_back(c);
    }
    if (vm.execute(nn, input, output)) {
      for (const auto o : output) {
        if (o == 0) {
          break;
        }
        std::cout << std::bitset<8>(o);
      }
      std::cout << std::endl;
      for (const auto o : output) {
        if (o == 0) {
          break;
        }
        std::cout << o;
      }
      std::cout << std::endl;

      std::cout << "Dump;" << std::endl;
      for (const auto n : nn.neurons) {
        n.dump();
      }
    }
  }
  return 0;
}

int evolve(std::vector<std::string> options, std::string parameter) {
  std::string directory = "./population";
  uint neurons = 3;
  uint connections = 5;
  uint population = 100;
  uint generation = 10;
  uint cycles = 25;

  // TODO Parse Options
  for (const auto o : options) {
    std::cerr << "Option " << o << " not supported" << std::endl;
  }

  if (parameter.size() == 0) {
    error() << "Missing <path-to-fitness-function> parameter\n";
    return -1;
  }

  EA ea(directory, parameter, neurons, connections, population, generation,
        cycles);

  if (!ea.evolve()) {
    return -1;
  }
  return 0;
}

int main(int argc, char **argv) {
  if (argc <= 0) {
    usage();
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
  } else if (command == "help") {
    if (argc < 3) {
      usage();
    } else {
      help(name, std::string(argv[2]));
    }
  } else {
    usage(name);
  }
  return 0;
}
