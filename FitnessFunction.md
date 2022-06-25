# Fitness Function

A fitness function is used to evaluate how well an individual in a population achieves a set of goals, and is used to guide an Evolutionary Algorithm towards an optimal solution.

In Neuro, the fitness function is decoupled from the evolutionary algorithm and exists as a file containing pairs of question/answers.

Each generation, the fitness function is read and each 'question' is posed to each individual in the population. Then the output from each individual is compared with the corresponding 'answer' to get a non-negative 'error' used to compare the fitnesses of each individual. The evolutionary algorithm will attempt to arrive at a solution that results in the smallest (closest to 0) error.

The general structure of a Fitness Function File is;

```
>question1
<answer1

>question2
<answer2

>question3
<answer3
```

For example;

```
>2+2
<4

>10-1
<9

>3*4
<12
```
