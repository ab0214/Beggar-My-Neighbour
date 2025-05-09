#include <vector>
#include <gtest/gtest.h>
#include "game/Game.h"

TEST(GameTests, ShortGame)
{
    std::vector<int> deck = {0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 4, 4, 4, 4,
                             3, 3, 3, 3, 2, 2, 2, 2,
                             1, 1, 1, 1};
    auto [cards, tricks] = Game::playGame(deck);
    EXPECT_EQ(cards, 52);
    EXPECT_EQ(tricks, 1);
}

TEST(GameTests, LongGame)
{
    // This is the longest game I have found so far.
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
    // This is the longest terminating game found.
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