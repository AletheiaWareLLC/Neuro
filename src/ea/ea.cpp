#include <iostream>
#include <sstream>

#include <Neuro/ea/ea.h>
#include <Neuro/ea/fitness.h>
#include <Neuro/network.h>
#include <Neuro/random.h>
#include <Neuro/vm/vm.h>

bool EA::load(const std::string directory) {
  // Read Population From Directory
  const auto path = std::filesystem::path{directory};
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    const auto path = entry.path();
    if (entry.is_regular_file()) {
      auto network = std::make_unique<Network>();
      if (!network->load(path)) {
        std::cerr << "EA Error: Network Load Failed: " << path << std::endl;
        return false;
      }
      if (network->neurons.empty()) {
        std::cerr << "EA Error: Loaded Network has no Neurons: " << path
                  << std::endl;
        return false;
      }
      std::stringstream ss;
      if (network->emit(ss)) {
        const auto s = ss.str();
        const std::hash<std::string> hasher;
        const auto hash = hasher(s);
        const auto id = std::to_string(hash) + ".neu";
        if (population.find(id) == population.cend()) {
          population[id] = std::move(network);
          sizes[id] = s.size();
          std::cout << "Loaded: " << id << std::endl;
        } else {
          std::cout << "Duplicate: " << id << std::endl;
        }
      } else {
        std::cout << "Defect: " << path << std::endl;
      }
    }
  }
  return true;
}

bool EA::repopulate(Random &rng, const uint limit, uint &randoms,
                    uint &duplicates, uint &defects) {
  while (population.size() < limit) {
    const auto ns = rng.nextUnsignedInt() % neurons + 1;
    const auto ss = rng.nextUnsignedInt() % states + 1;
    const auto as = rng.nextUnsignedInt() % actions + 1;
    const auto is = rng.nextUnsignedInt() % instructions + 1;
    const auto ls = rng.nextUnsignedInt() % links + 1;
    auto network = std::make_unique<Network>();
    if (network->generate(rng, alphabet, ns, ss, as, is, ls)) {
      if (network->neurons.empty()) {
        std::cerr << "EA Error: Random Network has no Neurons: " << ns << " "
                  << ss << " " << as << " " << is << " " << ls << " "
                  << std::endl;
        return false;
      }
      std::stringstream ss;
      if (network->emit(ss)) {
        const auto s = ss.str();
        const std::hash<std::string> hasher;
        const auto hash = hasher(s);
        const auto id = std::to_string(hash) + ".neu";
        if (population.find(id) == population.cend()) {
          population[id] = std::move(network);
          sizes[id] = s.size();
          // std::cout << "Random: " << id << std::endl;
          randoms++;
        } else {
          duplicates++;
        }
      } else {
        defects++;
      }
    } else {
      defects++;
    }
  }
  return true;
}

bool EA::evaluate(Random &rng, std::string &best, std::string &worst, uint &min,
                  uint &max, float &average) {
  // Virtual Machine
  const VM vm(cycles);
  // Fitness Function
  Fitness ff(fitness);

  errors.clear();
  failed.clear();

  for (const auto &[name, network] : population) {
    // Start at character count to incentivize space efficiency
    errors[name] = sizes[name];
  }

  // Read question/answer from fitness function
  std::string question, answer;

  while (ff.next(question, answer)) {

    std::vector<sint> input(question.size() + 1, 0);
    uint i = 0;
    for (; i < question.size(); i++) {
      input[i] = question[i];
    }
    input[i] = 0;

    for (const auto &[name, network] : population) {
      // std::cout << "Evaluating: " << name << std::endl;
      if (failed.find(name) != failed.end()) {
        // Skip
        continue;
      }

      std::vector<sint> output;
      uint c = 0;

      if (vm.execute(*network.get(), input, output, c)) {
        // Compare answer to fitness function
        // Start at cycle count to incentivize time efficiency
        auto error = c;
        uint i = 0;
        for (; i < output.size() && i < answer.size(); i++) {
          error += abs(output[i] - answer[i]);
        }
        for (; i < answer.size(); i++) {
          // Incomplete answer is penalized
          error += abs(answer[i]);
        }
        for (; i < output.size(); i++) {
          // Extra output is penalized
          error += abs(output[i]);
        }
        if (error > std::numeric_limits<uint>::max() - errors[name]) {
          // Avoid overflow
          errors[name] = std::numeric_limits<uint>::max();
        } else {
          errors[name] += error;
        }
      } else {
        failed.insert(name);
        errors.erase(name);
      }
    }
  }

  for (const auto &[k, v] : errors) {
    // std::cout << k << ": " << v << std::endl;
  }

  uint sum = 0;

  if (const auto count = errors.size(); count > 0) {
    for (const auto &[name, error] : errors) {
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
    average = (float)sum / (float)count;
  }

  if (!worst.empty()) {
    // Kill worst
    failed.insert(worst);
    errors.erase(worst);
  }
  return true;
}

bool EA::kill(const std::set<std::string> &victims, uint &deaths) {
  for (const auto &name : victims) {
    population.erase(name);
    sizes.erase(name);
    errors.erase(name);
    deaths++;
    // std::cout << "Death: " << name << std::endl;
  }
  return true;
}

bool EA::kill(const std::string directory, const std::set<std::string> &victims,
              uint &deaths) {
  const auto path = std::filesystem::path{directory};
  for (const auto &name : victims) {
    // Delete from directory
    const auto file = path / name;
    if (std::filesystem::exists(file)) {
      if (!std::filesystem::remove(file)) {
        std::cerr << "EA Error: File Deletion Failed: " << file << std::endl;
        return false;
      }
    }
  }
  return kill(victims, deaths);
}

bool EA::reproduce(Random &rng, const uint limit,
                   std::set<std::string> &parents, uint &births,
                   uint &duplicates, uint &defects) {
  if (population.size() >= limit) {
    // Nothing to do
    return true;
  }

  // Get eligible parents
  std::vector<std::string> ps;
  ps.reserve(errors.size());
  for (const auto &[name, error] : errors) {
    ps.push_back(name);
  }

  if (ps.size() < 2) {
    // Need at least two parents to reproduce
    return true;
  }

  const auto children = limit - population.size();

  for (auto c = 0; c < children; c++) {
    // Mate Best For Next Generation
    std::string a = ps[rng.nextUnsignedInt() % ps.size()];
    std::string b = ps[rng.nextUnsignedInt() % ps.size()];
    if (a == b) {
      continue;
    }
    parents.insert(a);
    parents.insert(b);
    const Network &na = *population[a].get();
    const Network &nb = *population[b].get();
    auto nc = std::make_unique<Network>();
    if (nc->mate(rng, na, nb)) {
      if (nc->neurons.empty()) {
        std::cerr << "EA Error: Child Network has no Neurons: " << a << " " << b
                  << std::endl;
        return false;
      }
      std::stringstream ss;
      if (nc->emit(ss)) {
        const auto s = ss.str();
        const std::hash<std::string> hasher;
        const auto hash = hasher(s);
        const auto id = std::to_string(hash) + ".neu";
        if (population.find(id) == population.cend()) {
          population[id] = std::move(nc);
          sizes[id] = s.size();
          // std::cout << "Child: " << a << " + " << b << " = " << id <<
          // std::endl;
          births++;
        } else {
          duplicates++;
        }
      } else {
        defects++;
      }
    } else {
      defects++;
    }
  }
  return true;
}

bool EA::mutate(Random &rng, uint &mutations, uint &duplicates, uint &defects) {
  if (population.size() == 0) {
    // Nothing to mutate
    return true;
  }
  auto itr = population.begin();
  // Randomly Mutate an Individual
  std::advance(itr, rng.nextUnsignedInt() % population.size());
  const std::string mutant = itr->first;
  // std::cout << "Mutating: " << mutant << std::endl;
  auto &network = itr->second;
  const auto ss = rng.nextUnsignedInt() % states + 1;
  const auto as = rng.nextUnsignedInt() % actions + 1;
  const auto is = rng.nextUnsignedInt() % instructions + 1;
  if (network->mutate(rng, alphabet, ss, as, is)) {
    if (network->neurons.empty()) {
      std::cerr << "EA Error: Mutated Network has no Neurons" << std::endl;
      return false;
    }
    // Network ID has Changed
    std::stringstream ss;
    if (network->emit(ss)) {
      const auto s = ss.str();
      const std::hash<std::string> hasher;
      const auto hash = hasher(s);
      const auto id = std::to_string(hash) + ".neu";
      if (population.find(id) == population.cend()) {
        population[id] = std::move(network);
        sizes[id] = s.size();
        population.erase(mutant);
        sizes.erase(mutant);
        errors.erase(mutant);
        mutations++;
      } else {
        duplicates++;
      }
    } else {
      defects++;
    }
  } else {
    defects++;
  }
  return true;
}

bool EA::save(const std::string directory, const std::string name,
              const std::unique_ptr<Network> &network) {
  const auto path = std::filesystem::path{directory};
  if (!std::filesystem::is_directory(path) || !std::filesystem::exists(path)) {
    if (!std::filesystem::create_directory(path)) {
      std::cerr << "EA Error: Directory Creation Failed" << std::endl;
      return false;
    }
  }
  std::ofstream destination;
  destination.open(path / name);
  auto result = network->emit(destination);
  destination.close();
  if (!result) {
    std::cerr << "EA Error: Network Emission Failed" << std::endl;
    return false;
  }
  return true;
}