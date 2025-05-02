#include <thread>
#include <atomic>
#include <iostream>
#include <iomanip>
#include "GameRunner.h"
#include "Genome.h"
#include "Island.h"

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
    Genome child1 = parent1.crossover(parent2, cut1, cut2);
    Genome child2 = parent2.crossover(parent1, cut1, cut2);
    std::cout << "Crossover (" << cut1 << ", " << cut2 << ")" << std::endl
              << "Child 1  : " << child1 << std::endl
              << "Child 2  : " << child2 << std::endl
              << std::endl;
  }

  std::cout << std::fixed << std::setprecision(2);
  for (double rate : {0.0, 0.01, 0.1, 0.25, 0.5, 0.75, 0.99, 1.0})
  {
    Genome mutated = parent1; // Copy original
    mutated.mutate(rate);
    std::cout << "Mutation rate " << rate << " : " << mutated << std::endl;
  }

  // std::cin.get();
}

void runGA()
{
  Island island = Island();
  while (true)
  {
    for (int i = 0; i < 500; i++)
    {
      island.evolve();
    }
    Genome best = island.getBestGenome();
    std::cout << best << std::endl
              << "Best: " << best.evaluate() << std::endl
              << "Average: " << island.getAverageFitness() << std::endl;
  }
}

int main()
{
  system("cls");
  runTests();
  runGA();
  return 0;
}