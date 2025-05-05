#pragma once

#include <tuple>

class GameRunner
{
public:
    GameRunner(int numThreads, int gamesPerThread);

    void runBatch();          // Runs one batch in parallel
    void runBatchBenchmark(); // Runs one batch in parallel, times and logs it

private:
    std::tuple<int, int> playGames(int games);

    int m_numThreads;
    int m_gamesPerThread;
    int m_maxCards = 0;
    int m_maxTricks = 0;
};
