#pragma once

#include <tuple>
#include <vector>

class Game
{
public:
    /// @brief Plays a game of Beggar My Neighbor with the given deck of cards.
    /// @param deck The deck of cards to use for the game.
    /// If empty, a shuffled deck will be generated.
    /// @return The length of the game (cards, tricks).
    static std::tuple<int, int> playGame(std::vector<int> deck = {});

private:
    static std::vector<int> generateShuffledDeck();
};
