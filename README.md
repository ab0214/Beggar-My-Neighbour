# Beggar-My-Neighbour

This project uses a genetic algorithm to find long-running games of [**Beggar-my-neighbour**](https://en.wikipedia.org/wiki/Beggar_my_neighbour).

Created to explore C++ and share as part of a personal portfolio.

## Features
- Beggar-my-neighbour simulation.
- Genetic algorithm to optimize starting configuration for the longest game.
- Supports mutation, crossover, and truncation selection.
- Unit tests using [GoogleTest](https://github.com/google/googletest).
- Performance benchmarks using [Google Benchmark](https://github.com/google/benchmark).

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
- **Git** (for downloading GoogleTest and Google Benchmark via FetchContent)
