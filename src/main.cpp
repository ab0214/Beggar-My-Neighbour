#include <thread>
#include <atomic>
#include <iostream>
#include "GameRunner.h"

int main()
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

  return 0;
}