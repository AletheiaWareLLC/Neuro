#include <iostream>
#include <limits>
#include <random>

int main() {
  std::random_device rseed;
  std::mt19937 rng(rseed());
  std::uniform_int_distribution<int> dist(std::numeric_limits<int>::min(),
                                          std::numeric_limits<int>::max());

  for (int i = 0; i < 1000; i++) {
    auto a = dist(rng);
    std::cout << '>' << a << '\n' << '<' << ~a << '\n' << '\n';
  }
}
