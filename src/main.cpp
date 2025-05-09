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

    std::cout << "Gen.: " << generations
              << " | Avg. fit.: " << averageFitness
              << " | Best fit.: " << population.bestGenome().evaluate()
              << " | Best genome: " << population.bestGenome() << std::endl;

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