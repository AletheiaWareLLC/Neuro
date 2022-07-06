#include <bitset>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <Neuro/alphabet.h>
#include <Neuro/data.h>
#include <Neuro/ea/ea.h>
#include <Neuro/ea/fitness.h>
#include <Neuro/network.h>
#include <Neuro/random.h>
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
    std::cout << "\n\t--links\n\t\tNumber of Links per Neuron (default 5)\n";
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
  uint actions = 5;
  uint instructions = 5;
  uint links = 5;

  // TODO Parse Options
  for (const auto o : options) {
    std::cerr << "Option " << o << " not supported" << std::endl;
  }

  if (parameter.size() == 0) {
    error() << "Missing <path-to-destination-file> parameter\n";
    return -1;
  }

  RealRandom rng;
  std::set<sint> alphabet{0, '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
  Network nn;
  if (!nn.generate(rng, alphabet, neurons, states, actions, instructions,
                   links)) {
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

  if (!nn.load(parameter)) {
    return -1;
  }

  std::cout << "Network:\n";
  std::stringstream ss;
  if (!nn.emit(ss)) {
    error() << "Network Emission Failed" << std::endl;
    return -1;
  }
  const auto s = ss.str();
  std::cout << s << std::endl;
  std::cout << "Size: " << s.size() << std::endl;

  const auto ns = nn.neurons.size();

  VM vm(cycles);

  std::string question;
  while (std::getline(std::cin, question)) {
    // Communication
    std::vector<sint> input;
    std::vector<sint> output;

    for (const auto c : question) {
      input.push_back(c);
    }
    input.push_back(0);
    uint c = 0;
    if (vm.execute(nn, input, output, c)) {
      std::cout << "Cycles: " << c << std::endl;
      auto first = true;
      for (const auto o : output) {
        if (o == 0) {
          break;
        }
        if (first) {
          first = false;
        } else {
          std::cout << ',';
        }
        std::cout << std::bitset<8>(o);
      }
      std::cout << std::endl;
      first = true;
      for (const auto o : output) {
        if (o == 0) {
          break;
        }
        if (first) {
          first = false;
        } else {
          std::cout << ',';
        }
        std::cout << o;
      }
      std::cout << std::endl;

      std::cout << "Dump;" << std::endl;
      for (const auto &n : nn.neurons) {
        n->dump(std::cout);
      }
    }
  }
  return 0;
}

int evaluate(std::vector<std::string> options, std::string parameter) {
  // TODO load all networks in directory
  // Run all tests in fitness function
  // Print scores
  return 0;
}

int evolve(std::vector<std::string> options, std::string parameter) {
  std::set<sint> alphabet;
  alphabet.insert(0);
  std::string directory = "./population";

  uint population = 1000;
  uint generation = 500;
  uint lifespan = 5;
  uint cycles = 10000;
  uint neurons = 25;
  uint states = 5;
  uint actions = 5;
  uint instructions = 5;
  uint links = (float)neurons * 2.5f;

  // TODO Parse Options
  for (const auto o : options) {
    std::cerr << "Option " << o << " not supported" << std::endl;
  }

  std::cout << "Population: " << population << std::endl;
  std::cout << "Generation: " << generation << std::endl;
  std::cout << "Lifespan: " << lifespan << std::endl;
  std::cout << "Cycles: " << cycles << std::endl;
  std::cout << "Neurons: " << neurons << std::endl;
  std::cout << "States: " << states << std::endl;
  std::cout << "Actions: " << actions << std::endl;
  std::cout << "Instructions: " << instructions << std::endl;
  std::cout << "Links: " << links << std::endl;

  if (parameter.size() == 0) {
    error() << "Missing <path-to-fitness-function> parameter\n";
    return -1;
  }

  // Fitness Function
  Fitness ff(parameter);
  // Read question/answer from fitness function
  std::string question, answer;
  while (ff.next(question, answer)) {
    alphabet.insert(question.cbegin(), question.cend());
    alphabet.insert(answer.cbegin(), answer.cend());
  }

  EA ea(alphabet, parameter, cycles, neurons, states, actions, instructions,
        links);

  if (!ea.load(directory)) {
    return -1;
  }

  RealRandom rng;

  std::map<std::string, uint> ages;
  std::string best;
  std::string worst;
  uint goat = std::numeric_limits<uint>::max();

  for (int g = 0; g < generation; g++) {
    std::cout << "================" << std::endl;
    std::cout << "Generation: " << g << std::endl;
    std::cout << "Population: " << ea.population.size() << std::endl;

    uint randoms = 0;
    uint duplicates = 0;
    uint defects = 0;

    if (!ea.repopulate(rng, population, randoms, duplicates, defects)) {
      return -1;
    }
    std::cout << "Randoms: " << randoms << std::endl;

    uint min = std::numeric_limits<uint>::max();
    uint max = std::numeric_limits<uint>::min();
    float average = 0;

    if (!ea.evaluate(rng, best, worst, min, max, average)) {
      return -1;
    }

    std::cout << "Worst: " << max << " (" << worst << ")" << std::endl;
    std::cout << "Average: " << average << std::endl;
    std::cout << "Best: " << min << " (" << best << ")" << std::endl;

    if (best == worst) {
      std::cerr << "No Progress" << std::endl;
      break;
    }

    if (!best.empty()) {
      if (const auto e = ea.errors[best]; e < goat) {
        goat = e;
        if (!ea.save(directory, best, ea.population[best])) {
          return -1;
        }
      }
    }

    const auto target = average + (max - average) / 2;
    for (const auto [name, error] : ea.errors) {
      if (error > target) {
        ea.failed.insert(name);
      }
    }

    uint deaths = 0;
    if (!ea.kill(directory, ea.failed, deaths)) {
      return -1;
    }

    if (g + 1 < generation) {
      std::set<std::string> parents;
      uint births = 0;
      if (!ea.reproduce(rng, population, parents, births, duplicates,
                        defects)) {
        return -1;
      }
      std::cout << "Parents: " << parents.size() << std::endl;
      std::cout << "Births: " << births << std::endl;

      uint mutations = 0;
      for (int i = 0; i < population / 100; i++) {
        // Mutate 1% of population
        if (!ea.mutate(rng, mutations, duplicates, defects)) {
          return -1;
        }
      }
      std::cout << "Mutations: " << mutations << std::endl;
    }

    std::set<std::string> senicides;
    float ns = 0;
    float ss = 0;
    float as = 0;
    float is = 0;
    float ls = 0;
    for (const auto &[name, network] : ea.population) {
      ages[name]++;
      if (ages[name] >= lifespan) {
        senicides.insert(name);
      } else {
        ns += network->neurons.size();
        for (const auto &n : network->neurons) {
          ss += n->states.size();
          for (const auto &s : n->states) {
            as += s->actions.size();
            for (const auto &[p, a] : s->actions) {
              is += a->instructions.size();
            }
            if (s->wildcard) {
              is += s->wildcard->get()->instructions.size();
            }
          }
        }
        ls += network->links.size();
      }
    }
    ea.kill(senicides, deaths);

    std::cout << "Deaths: " << deaths << std::endl;
    std::cout << "Duplicates: " << duplicates << std::endl;
    std::cout << "Defects: " << defects << std::endl;

    if (const auto ps = ea.population.size(); ps > 0) {
      std::cout << "Neurons per Network: " << (ns / ps) << std::endl;
      std::cout << "States per Neuron: " << (ss / ns) << std::endl;
      std::cout << "Actions per State: " << (as / ss) << std::endl;
      std::cout << "Instructions per Action: " << (is / as) << std::endl;
      std::cout << "Links per Network: " << (ls / ps) << std::endl;
    }
  }

  if (best.empty() || best == worst) {
    std::cerr << "Solution Not Found" << std::endl;
    return -1;
  }
  // Save Best Scorer
  std::cout << "Best: " << best << std::endl;
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
  } else if (command == "evaluate") {
    return evaluate(options, parameter);
  } else if (command == "evolve") {
    return evolve(options, parameter);
  } else if (command == "random") {
    RealRandom rng;
    std::cout << rng.nextSignedInt() << std::endl;
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
