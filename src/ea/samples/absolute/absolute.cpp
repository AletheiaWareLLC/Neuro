#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <stdlib.h>

int main() {
  std::random_device rseed;
  std::mt19937 rng(rseed());
  std::uniform_int_distribution<int> dist(std::numeric_limits<int>::min(),
                                          std::numeric_limits<int>::max());

  auto n = dist(rng);

  std::cout << n << std::endl;

  auto expected = abs(n);

  int answer;
  while (std::cin >> answer) {
    auto error = abs(answer - expected);
    std::cout << error << std::endl;
  }
}
