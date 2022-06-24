# Fitness Function

A fitness function is used to evaluate how well an individual in a population achieves a set of goals, and is used to guide an Evolutionary Algorithm towards an optimal solution.

In Neuro, the fitness function is decoupled from the evolutionary algorithm and runs as its own program, communicating with the evolutionary algorithm via stdin/stdout.

Each generation, the fitness function program is executed and must print a single line 'question' to stdout which is used as the input for all individuals in the population. Then it must repeadedly read a single line 'answer' representing the output from an individual from stdin and write a single line, non-negative 'error' to stdout, until there is no more input.

The general structure of a Fitness Function is;

```
std::cout << question << std::endl;

std::string answer;
while (std::getline(std::cin, answer)) {
    auto error = fitness(answer);
    std::cout << error << std::endl;
}
```

The evolutionary algorithm will attempt to arrive at a solution that results in the smallest (closest to 0) error.
