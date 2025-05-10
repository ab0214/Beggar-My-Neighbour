#include <vector>
#include <gtest/gtest.h>
#include "game/Game.h"

TEST(GameTests, ShortGame)
{
    // This game ends in 51 cards played, when player 1 runs out of cards.
    // Player 2 is left with 1 card in hand, unplayed.
    // Neither player wins any tricks.
    std::vector<int> deck = {0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 4, 4, 4, 4,
                             3, 3, 3, 3, 2, 2, 2, 2,
                             1, 1, 1, 1};
    auto [cards, tricks] = Game::playGame(deck);
    EXPECT_EQ(cards, 51);
    EXPECT_EQ(tricks, 0);
}

TEST(GameTests, LongGame)
{
    // This is a long game this program has found.
    std::vector<int> deck = {0, 0, 2, 0, 0, 0, 0, 0,
                             0, 0, 0, 2, 0, 4, 1, 0,
                             0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 4, 0, 0, 0, 4,
                             0, 0, 0, 2, 4, 1, 3, 2,
                             1, 3, 3, 3, 0, 0, 0, 0,
                             0, 1, 0, 0};
    auto [cards, tricks] = Game::playGame(deck);
    EXPECT_EQ(cards, 3581);
}

TEST(GameTests, LongestGame)
{
    // This is the longest terminating game known.
    // https://people.tamu.edu/~rcn/
    // https://www.richardpmann.com/beggar-my-neighbour-records.html
    std::vector<int> deck = {0, 0, 0, 0, 0, 0, 4, 0,
                             1, 0, 0, 4, 0, 0, 2, 0,
                             0, 0, 0, 0, 0, 3, 0, 1,
                             0, 0, 0, 3, 0, 0, 0, 0,
                             0, 0, 2, 0, 4, 0, 3, 0,
                             2, 0, 1, 0, 1, 4, 0, 0,
                             2, 0, 3, 0};
    auto [cards, tricks] = Game::playGame(deck);
    EXPECT_EQ(cards, 8344);
    EXPECT_EQ(tricks, 1164);
}