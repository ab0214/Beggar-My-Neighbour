#include <gtest/gtest.h>
#include "game/Card.h"

TEST(CardTests, Constructor)
{
    Card num(0);
    Card jack(1);
    Card queen(2);
    Card king(3);
    Card ace(4);
    EXPECT_EQ(num.getValue(), 0);
    EXPECT_EQ(num.getSymbol(), '-');
    EXPECT_EQ(jack.getValue(), 1);
    EXPECT_EQ(jack.getSymbol(), 'J');
    EXPECT_EQ(queen.getValue(), 2);
    EXPECT_EQ(queen.getSymbol(), 'Q');
    EXPECT_EQ(king.getValue(), 3);
    EXPECT_EQ(king.getSymbol(), 'K');
    EXPECT_EQ(ace.getValue(), 4);
    EXPECT_EQ(ace.getSymbol(), 'A');
}

TEST(CardTests, InvalidValue)
{
    EXPECT_THROW(Card(5), std::invalid_argument);
}