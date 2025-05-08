#include <atomic>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <random>
#include <thread>

#include "evolution/Genome.h"
#include "evolution/GenomeOperations.h"
#include "evolution/Population.h"
#include "evolution/PopulationOperations.h"
#include "game/GameRunner.h"

void runRandomGames()
{
  int num_threads = std::thread::hardware_concurrency();
  int games_per_thread = 100000;
  GameRunner gameRunner = GameRunner(num_threads, games_per_thread);

  std::cout << "Using " << num_threads << " threads." << std::endl;

  std::atomic<bool> running{true};
  while (running)
  {
    std::thread worker([&]()
                       { while (running) gameRunner.runBatchBenchmark(); });

    std::cout << "Running. Press enter to stop..." << std::endl;
    std::cin.get();
    running = false;
    worker.join();

    std::cout << "Paused. Press enter to continue..." << std::endl;
    std::cin.get();
    running = true;
  }
}

void runTests()
{
  Genome parent1 = Genome();
  Genome parent2 = Genome();
  std::cout << "Parents" << std::endl
            << "Parent 1 : " << parent1 << std::endl
            << "Parent 2 : " << parent2 << std::endl
            << std::endl;

  Genome child1;
  Genome child2;

  std::vector<std::pair<int, int>> cutPoints = {
      {0, 0},   // Do nothing
      {0, 52},  // Swap
      {0, 26},  // One point
      {17, 32}, // Two point
      {-1, -1}  // Random
  };

  for (const auto &[cut1, cut2] : cutPoints)
  {
    Genome child1 = GenomeOperations::crossover(parent1, parent2, cut1, cut2);
    Genome child2 = GenomeOperations::crossover(parent2, parent1, cut1, cut2);
    std::cout << "Crossover (" << cut1 << ", " << cut2 << ")" << std::endl
              << "Child 1  : " << child1 << std::endl
              << "Child 2  : " << child2 << std::endl
              << std::endl;
  }

  std::cout << std::fixed << std::setprecision(2);
  for (double rate : {0.0, 0.01, 0.1, 0.25, 0.5, 0.75, 0.99, 1.0})
  {
    Genome mutated = GenomeOperations::mutate(parent1, rate); // Copy original
    std::cout << "Original           : " << parent1 << std::endl;
    std::cout << "Mutation rate " << rate << " : " << mutated << std::endl;
  }

  // std::cin.get();
}

void runGA(int printEveryXGenerations,
           int populationSize,
           double mutationRate,
           double selectionRate)
{
  int generations = 0;
  Population population = Population(populationSize);

  std::vector<Genome> nextGeneration;
  nextGeneration.reserve(populationSize);

  std::cout << std::fixed << std::setprecision(2); // Set precision for output
  std::mt19937 rng(std::random_device{}());        // Random number generator

  while (true)
  {
    double totalFitness = population.totalFitness();
    double averageFitness = population.averageFitness();

    std::cout << "Gen: " << generations;
    std::cout << " | Avg. fit.: " << averageFitness;
    std::cout << " | Best fit.: " << population.bestGenome().evaluate();
    std::cout << " | Best genome.: " << population.bestGenome() << std::endl;

    for (int i = 0; i < printEveryXGenerations; ++i, ++generations)
    {
      // Clear next generation
      nextGeneration.clear();

      // Select elites
      auto elites =
          PopulationOperations::truncationSelection(population, selectionRate);

      // Add elites to next generation
      nextGeneration.insert(nextGeneration.end(), elites.begin(), elites.end());

      // Add mutated copies of elites to next generation
      for (const auto &genome : elites)
      {
        Genome mutated = GenomeOperations::mutate(genome, mutationRate);
        nextGeneration.emplace_back(mutated);
      }

      // Random index distribution for selecting parents
      std::uniform_int_distribution<size_t> eliteIndexDist(0, elites.size() - 1);

      // Fill the rest of the population with children of elites
      while (nextGeneration.size() < populationSize)
      {
        // Select two parents randomly
        size_t i1 = eliteIndexDist(rng);
        size_t i2 = eliteIndexDist(rng);

        // Ensure parents are different
        while (i2 == i1)
          i2 = eliteIndexDist(rng);

        Genome parent1 = elites[i1];
        Genome parent2 = elites[i2];

        // Perform crossover and mutation
        Genome child1 = GenomeOperations::crossover(parent1, parent2);
        Genome child2 = GenomeOperations::crossover(parent2, parent1);
        GenomeOperations::mutateInPlace(child1, mutationRate);
        GenomeOperations::mutateInPlace(child2, mutationRate);

        // Add children to next generation
        nextGeneration.push_back(child1);
        if (nextGeneration.size() < populationSize)
          nextGeneration.push_back(child2);
      }

      population = Population(nextGeneration);
    }
  }
}

int main(int argc, char *argv[])
{
  // Default values
  int printEveryXGenerations = 100;
  int populationSize = 100;
  double mutationRate = 0.1;
  double selectionRate = 0.25;

  // Parse command-line arguments
  if (argc > 1)
    printEveryXGenerations = std::atoi(argv[1]); // First argument
  if (argc > 2)
    populationSize = std::atoi(argv[2]); // Second argument
  if (argc > 3)
    mutationRate = std::atof(argv[3]); // Third argument
  if (argc > 4)
    selectionRate = std::atof(argv[4]); // Fourth argument

  runGA(printEveryXGenerations, populationSize, mutationRate, selectionRate);
  return 0;
}