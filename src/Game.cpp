#include "Game.h"
#include <algorithm>
#include <chrono>
#include <future>
#include <iostream>
#include <random>
#include <vector>

Game::Game(int numThreads, int gamesPerThread)
    : m_numThreads(numThreads), m_gamesPerThread(gamesPerThread) {}

std::tuple<int, int> Game::playGame()
{
    auto deck = generateShuffledDeck();
    Cards players[] = {Cards(), Cards()};
    for (int i = 0; i < deck.size(); i += 2)
    {
        players[0].insertBottom(new Card(deck[i]));
        players[1].insertBottom(new Card(deck[i + 1]));
    }

    Cards pile;
    int cards = 0, tricks = 0, turn = 0, penalty = 0;

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

std::tuple<int, int> Game::playGames(int games)
{
    int maxCards = 0, maxTricks = 0;
    for (int i = 0; i < games; ++i)
    {
        auto [cards, tricks] = playGame();
        maxCards = std::max(maxCards, cards);
        maxTricks = std::max(maxTricks, tricks);
    }
    return {maxCards, maxTricks};
}

std::vector<int> Game::generateShuffledDeck()
{
    std::vector<int> deck(52, 0);
    for (int i = 0; i < 16; ++i)
        deck[i] = i / 4 + 1;

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
    return deck;
}

void Game::runBatch()
{
    std::vector<std::future<std::tuple<int, int>>> futures;

    for (int i = 0; i < m_numThreads; ++i)
        futures.push_back(std::async(std::launch::async, &Game::playGames, this, m_gamesPerThread));

    for (auto &f : futures)
    {
        auto [cards, tricks] = f.get();
        m_maxCards = std::max(m_maxCards, cards);
        m_maxTricks = std::max(m_maxTricks, tricks);
    }
}

void Game::runBatchBenchmark()
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
