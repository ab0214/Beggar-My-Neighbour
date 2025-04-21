#include <algorithm>
#include <chrono>
#include <ctime>
#include <future>
#include <iostream>
#include <random>
#include <tuple>
#include <vector>
#include "Card.h"
#include "Cards.h"

std::tuple<int, int> playGame()
{
  std::vector<int> deck(52, 0); // 52 cards that will be dealt to the players
  for (int i = 0; i < 16; ++i)  // Add four of each picture card to the deck
                                // (replacing number cards)
    deck[i] = i / 4 + 1;

  std::random_device rd;                     // Random device to get a seed
  std::mt19937 g(rd());                      // Mersenne Twister engine initialized with the seed
  std::shuffle(deck.begin(), deck.end(), g); // Shuffle the deck

  Cards players[] = {Cards(), Cards()};
  for (int i = 0; i < deck.size(); i += 2)
  {
    players[0].insertBottom(new Card(deck[i]));
    players[1].insertBottom(new Card(deck[i + 1]));
  }

  Cards pile = Cards();

  int cards = 0;
  int tricks = 0;

  int turn = 0;
  int penalty = 0;

  while (!players[0].isEmpty() && !players[1].isEmpty())
  {
    ++cards;

    if (penalty > 0)
    {
      pile.insertBottom(players[turn].pop());

      if (pile.getBottom()->getValue() > 0)
      {
        turn = !turn;
        penalty = pile.getBottom()->getValue();
        tricks++;
        continue;
      }

      else if (--penalty == 0)
      {
        turn = !turn;
        players[turn].splice(&pile);
      }

      continue;
    }

    pile.insertBottom(players[turn].pop());
    penalty = pile.getBottom()->getValue();
    turn = !turn;
  }

  return {cards, tricks};
}

std::tuple<int, int> playGames(int t_games)
{
  int max_cards = 0;
  int max_tricks = 0;

  for (int i = 0; i < t_games; ++i)
  {
    auto [cards, tricks] = playGame();
    max_cards = cards > max_cards ? cards : max_cards;
    max_tricks = tricks > max_tricks ? tricks : max_tricks;
  }

  return {max_cards, max_tricks};
}

int main()
{
  int num_threads = std::thread::hardware_concurrency();
  int games_per_thread = 100000;
  int max_cards = 0;
  int max_tricks = 0;

  std::cout << "Running on " << num_threads << " threads." << std::endl;

  while (true)
  {
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::future<std::tuple<int, int>>> futures;

    for (int i = 0; i < num_threads; ++i)
    {
      futures.push_back(
          std::async(std::launch::async, playGames, games_per_thread));
    }

    for (auto &f : futures)
    {
      auto [cards, tricks] = f.get();
      max_cards = cards > max_cards ? cards : max_cards;
      max_tricks = tricks > max_tricks ? tricks : max_tricks;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_s = end - start;
    std::chrono::duration<double, std::micro> elapsed_us = end - start;

    int games = num_threads * games_per_thread;
    int avg_us = elapsed_us.count() / games;
    std::cout << "Played " << games << " games in " << elapsed_s.count()
              << " seconds (Avg: " << avg_us << " us per game)." << std::endl
              << "Most cards so far: " << max_cards << std::endl
              << "Most tricks so far: " << max_tricks << std::endl;
  }

  return 0;
}