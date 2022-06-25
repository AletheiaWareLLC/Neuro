#include <iostream>
#include <limits>
#include <random>

int main() {
  std::random_device rseed;
  std::mt19937 rng(rseed());
  std::uniform_int_distribution<int> dist(std::numeric_limits<int>::min(),
                                          std::numeric_limits<int>::max());

  for (int i = 0; i < 10; i++) {
    const auto n = dist(rng);
    const auto e = abs(n);
    std::cout << '>' << n << '\n' << '<' << e << '\n' << '\n';
  }
}
