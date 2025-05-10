# Beggar-My-Neighbour

This C++ program implements a genetic algorithm to find long instances of the card game [**Beggar-my-neighbour**](https://en.wikipedia.org/wiki/Beggar_my_neighbour).

## Features
- Beggar-my-neighbour simulation.
- Genetic algorithm to optimize deck configuration for the longest game.
- Supports mutation, crossover, and truncation selection.

## Usage
Run the program with the following optional command line arguments:
```cmd
Beggar-My-Neighbour [logInterval] [populationSize] [mutationRate] [selectionRate]
```
- **logInterval**: Print progress every N generations (default: 100)
- **populationSize**: Number of individuals in each generation (default: 100)
- **mutationRate**: Fraction of cards to reshuffle in each individual deck (0.0–1.0, default: 0.1)
- **selectionRate**: Fraction of top individuals selected for reproduction (0.0–1.0, default: 0.25)

## Build requirements
- **CMake** ≥ 3.14
- **C++17** compatible compiler (e.g., GCC 7+, Clang 5+, MSVC 2017+)
- **Git** (for downloading GoogleTest via FetchContent)
