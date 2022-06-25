#include <iostream>
#include <limits>
#include <random>

int main() {
  std::random_device rseed;
  std::mt19937 rng(rseed());
  std::uniform_int_distribution<int> dist(
      std::numeric_limits<int>::min() / 1000,
      std::numeric_limits<int>::max() / 1000);

  for (int i = 0; i < 100; i++) {
    auto a = dist(rng);
    auto b = dist(rng);
    int c;
    char op;
    switch ((uint)dist(rng) % 5) {
    case 0:
      op = '+';
      c = a + b;
      break;
    case 1:
      op = '-';
      c = a - b;
      break;
    case 2:
      op = '*';
      c = a * b;
      break;
    case 3:
      while (b == 0) {
        b = dist(rng);
      }
      op = '/';
      c = a / b;
      break;
    case 4:
      while (b == 0) {
        b = dist(rng);
      }
      op = '%';
      c = a % b;
      break;
    }
    std::cout << '>' << a << op << b << '\n' << '<' << c << '\n' << '\n';
  }
}
