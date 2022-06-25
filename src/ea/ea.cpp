#include <iostream>
#include <sstream>

#include <Neuro/ea/ea.h>
#include <Neuro/ea/fitness.h>
#include <Neuro/network.h>
#include <Neuro/random.h>
#include <Neuro/vm/vm.h>

bool EA::evolve() {
  const auto path = std::filesystem::path{directory};
  if (!std::filesystem::is_directory(path) || !std::filesystem::exists(path)) {
    if (!std::filesystem::create_directory(path)) {
      std::cerr << "EA Error: Population Directory Creation Failed"
                << std::endl;
      return false;
    }
  }

  const VM vm(maxCycles);

  std::map<std::string, Network> population;

  std::string best;
  std::string worst;

  // Read Population From Directory
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    const auto path = entry.path();
    const auto name = path.filename().string();
    if (entry.is_regular_file()) {
      Network nn;
      if (!nn.load(path)) {
        std::cerr << "EA Error: Network Load Failed" << std::endl;
        return false;
      }
      population[name] = nn;
      std::cout << "Loaded: " << name << std::endl;
    }
  }

  for (int g = 0; g < maxGenerations; g++) {
    std::cout << "Generation: " << g << std::endl;

    //  While population size < population limit, generate random network
    uint randoms = 0;
    while (population.size() < maxIndividuals) {
      Network nn;
      if (!nn.generate(maxNeurons, maxConnections)) {
        std::cerr << "EA Error: Network Generation Failed" << std::endl;
        return false;
      }
      const auto id = nn.id() + ".neu";
      population[id] = nn;
      // std::cout << "Random: " << id << std::endl;
      randoms++;
    }
    std::cout << "Randoms: " << randoms << std::endl;

    // Create fitness function
    Fitness ff(fitness);
    std::map<std::string, uint> errors;

    std::set<std::string> failed;

    // Read question/answer from fitness function
    std::string question, answer;
    while (ff.next(question, answer)) {
      // std::cout << "Question: " << question << std::endl;

      std::vector<sbyte> input(question.size(), 0);
      for (int i = 0; i < question.size(); i++) {
        input[i] = question[i];
      }

      for (auto [name, network] : population) {
        if (failed.find(name) != failed.end()) {
          // Skip
          continue;
        }
        std::vector<sbyte> output;
        if (vm.execute(network, input, output)) {
          // std::cout << "Answer: ";
          // for (auto c : output) {
          // std::cout << c;
          //}
          // std::cout << std::endl;

          // Compare answer to fitness function
          auto error = 0;
          int i = 0;
          for (; i < output.size(); i++) {
            if (i < answer.size()) {
              error += abs(output[i] - question[i]);
            } else {
              // Extra output is penalized
              error += output[i];
            }
          }
          if (errors.find(name) == errors.end()) {
            errors[name] = error;
          } else {
            errors[name] += error;
          }
        } else {
          failed.insert(name);
        }
      }
    }

    for (const auto [k, v] : errors) {
      std::cout << k << ": " << v << std::endl;
    }

    uint min = std::numeric_limits<uint>::max();
    uint max = std::numeric_limits<uint>::min();
    uint sum = 0;
    float average = 0;

    for (const auto [name, error] : errors) {
      if (error < min) {
        min = error;
        best = name;
      }
      if (error > max) {
        max = error;
        worst = name;
      }
      sum += error;
    }

    if (const auto count = errors.size(); count > 0) {
      std::cout << "Worst: " << max << std::endl;
      average = (float)sum / (float)count;
      std::cout << "Average: " << average << std::endl;
      std::cout << "Best: " << min << std::endl;
    }

    std::vector<std::string> parents;

    for (const auto [name, nn] : population) {
      if (errors.find(name) == errors.end()) {
        failed.insert(name);
      } else {
        parents.push_back(name);
      }
    }

    for (const auto name : failed) {
      population.erase(name);
      errors.erase(name);
      /* TODO delete from directory
      const auto file = path / name;
      if (std::filesystem::exists(file)) {
        if (!std::filesystem::remove(file)) {
          std::cerr << "EA Error: Network File Deletion Failed" << std::endl;
          return false;
        }
      }
      */
      // std::cout << "Killed: " << name << std::endl;
    }
    std::cout << "Deaths: " << failed.size() << std::endl;

    if (g + 1 < maxGenerations) {
      // std::cout << "Reproducing" << std::endl;
      // Mate Best For Next Generation
      uint births = 0;
      if (const auto count = parents.size(); count > 0) {
        while (population.size() < maxIndividuals) {
          std::string a = parents[udist(rng) % count];
          std::string b = parents[udist(rng) % count];
          if (a == b) {
            break;
          }
          const Network &na = population[a];
          const Network &nb = population[b];
          Network nc;
          if (!nc.mate(na, nb)) {
            std::cerr << "EA Error: Network Reproduction Failed" << std::endl;
            return false;
          }
          const auto id = nc.id() + ".neu";
          population[id] = nc;
          // std::cout << "Child: " << a << " + " << b << " = " << id <<
          // std::endl;
          births++;
        }
      }
      std::cout << "Births: " << births << std::endl;

      if (!worst.empty() && best != worst) {
        // Randomly Mutate Worst Scorer
        auto nn = population[worst];
        std::cout << "Mutating: " << worst << std::endl;
        if (nn.mutate()) {
          // Network ID has Changed
          population.erase(worst);
          errors.erase(worst);
          const auto id = nn.id() + ".neu";
          population[id] = nn;
        }
      }
    }
  }

  if (best.size() > 0) {
    // Save Best Scorer
    std::cout << "Best: " << best << std::endl;
    const auto nn = population[best];
    std::ofstream destination;
    destination.open(path / best);
    auto result = nn.emit(destination);
    destination.close();
    if (!result) {
      std::cerr << "EA Error: Network Emission Failed" << std::endl;
      return false;
    }
  } else {
    std::cout << "Solution Not Found" << std::endl;
  }

  return true;
}