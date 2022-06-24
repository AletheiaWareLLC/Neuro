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
./build/Neuro generate [options] <path-to-destination-file>
```

## Run

Runs a neural network.

```
./build/Neuro run [options] <path-to-source-file>
```

## Evolve

Evolves a population of neural networks

```
./build/Neuro evolve [options] <path-to-fitness-function-program>
```

## Help

```
./build/Neuro help
./build/Neuro help <command>
```
