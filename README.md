# Neuro

Neuro is an implementation of a Neural Network in which each Neuron is implemented as an 8-bit Pushdown Automaton.

Neural Networks can either be written manually in Domain-Specific Language, or an Evolutionary Algorithm can be used to generate the optimal Neural Network for a given Fitness Function.

## Build

```cmake
cmake -S . -B build
cmake --build build
```

## Test

```cmake
cmake -S test -B build/test
cmake --build build/test/
(cd build/test/ && ctest)
```


# Usage

```
./build/Neuro
```

## Generate

Generates a new random neural network.

```
./build/Neuro generate [options] <destination-file>
```

### Parameter

1. Path to DSL Destination File

### Options

- Number of Neurons (default 5)
- Number of Connections per Neuron (default 5)
- Number of States per Neuron (default 5)
- Number of Actions per State (default 5)
- Number of Instructions per Action (default 5)

## Run

Runs a neural network.

```
./build/Neuro run [options] <source-file>
```

### Parameter

1. Path to DSL Source File

### Options

- Path to Input (default read from stdin)
- Path to Ouput (default write to stdout)
- Cycle Limit (default 100)

## Evolve

Evolves a population of neural networks

```
./build/Neuro evolve [options] <population-directory> <fitness-function>
```

### Parameters

1. Path to Population
2. Path to Fitness Function

### Options

- Population Limit (default 100)
- Generation Limit (default 100)
- Cycle Limit (default 100)
- Mutation Rate (default 1 per generation)
