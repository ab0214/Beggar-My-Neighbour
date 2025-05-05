#include "game/GameRunner.h"

#include "game/Game.h"
#include <algorithm>
#include <chrono>
#include <future>
#include <iostream>
#include <tuple>

GameRunner::GameRunner(int numThreads, int gamesPerThread)
    : m_numThreads(numThreads), m_gamesPerThread(gamesPerThread) {}

std::tuple<int, int> GameRunner::playGames(int games)
{
    int maxCards = 0, maxTricks = 0;
    for (int i = 0; i < games; ++i)
    {
        auto [cards, tricks] = Game::playGame();
        maxCards = std::max(maxCards, cards);
        maxTricks = std::max(maxTricks, tricks);
    }
    return {maxCards, maxTricks};
}

void GameRunner::runBatch()
{
    std::vector<std::future<std::tuple<int, int>>> futures;

    for (int i = 0; i < m_numThreads; ++i)
        futures.push_back(std::async(std::launch::async, &GameRunner::playGames, this, m_gamesPerThread));

    for (auto &f : futures)
    {
        auto [cards, tricks] = f.get();
        m_maxCards = std::max(m_maxCards, cards);
        m_maxTricks = std::max(m_maxTricks, tricks);
    }
}

void GameRunner::runBatchBenchmark()
{
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::future<std::tuple<int, int>>> futures;

    runBatch();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_s = end - start;
    std::chrono::duration<double, std::micro> elapsed_us = end - start;

    int games = m_numThreads * m_gamesPerThread;
    double avg_us = elapsed_us.count() / games;

    std::cout << "Played " << games << " games in " << elapsed_s.count() << " seconds (Avg: " << avg_us
              << " us per game).\n"
              << "Most cards so far: " << m_maxCards << "\n"
              << "Most tricks so far: " << m_maxTricks << "\n";
}
