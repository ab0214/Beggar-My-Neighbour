#pragma once
#include "Cards.h"
#include <tuple>
#include <vector>

class Game
{
public:
    Game(int t_numThreads, int t_gamesPerThread);
    void runBatch();
    void runBatchBenchmark();

private:
    std::tuple<int, int> playGame();
    std::tuple<int, int> playGames(int t_games);
    std::vector<int> generateShuffledDeck();

    int m_numThreads;
    int m_gamesPerThread;
    int m_maxCards = 0;
    int m_maxTricks = 0;
};
