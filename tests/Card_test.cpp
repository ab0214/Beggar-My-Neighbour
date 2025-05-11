#include <gtest/gtest.h>
#include "game/Card.h"

TEST(CardTests, Constructor)
{
    Card num(0);
    Card jack(1);
    Card queen(2);
    Card king(3);
    Card ace(4);
    EXPECT_EQ(num.value(), 0);
    EXPECT_EQ(num.symbol(), '-');
    EXPECT_EQ(jack.value(), 1);
    EXPECT_EQ(jack.symbol(), 'J');
    EXPECT_EQ(queen.value(), 2);
    EXPECT_EQ(queen.symbol(), 'Q');
    EXPECT_EQ(king.value(), 3);
    EXPECT_EQ(king.symbol(), 'K');
    EXPECT_EQ(ace.value(), 4);
    EXPECT_EQ(ace.symbol(), 'A');
}

TEST(CardTests, InvalidValue)
{
    EXPECT_THROW(Card(5), std::invalid_argument);
}