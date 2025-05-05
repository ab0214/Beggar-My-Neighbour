#include "game/Game.h"

#include "game/Card.h"
#include "game/Cards.h"
#include <algorithm>
#include <random>
#include <vector>

std::tuple<int, int> Game::playGame(std::vector<int> t_deck)
{
    if (t_deck.empty())
    {
        t_deck = generateShuffledDeck();
    }

    Cards players[] = {Cards(), Cards()};
    for (int i = 0; i < t_deck.size(); i += 2)
    {
        players[0].insertBottom(new Card(t_deck[i]));
        players[1].insertBottom(new Card(t_deck[i + 1]));
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
