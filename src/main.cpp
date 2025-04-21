#include <thread>
#include "Card.h"
#include "Cards.h"
#include "Game.h"

int main()
{
  int num_threads = std::thread::hardware_concurrency();
  int games_per_thread = 100000;
  std::cout << "Running on " << num_threads << " threads." << std::endl;

  Game game = Game(num_threads, games_per_thread);
  game.run();

  return 0;
}