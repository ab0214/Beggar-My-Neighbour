#include "game/Game.h"

#include <algorithm>
#include <random>
#include <vector>

#include "game/Card.h"
#include "game/Cards.h"

std::tuple<int, int> Game::playGame(std::vector<int> deck)
{
    Cards players[] = {Cards(), Cards()};
    for (int i = 0; i < deck.size(); i += 2)
    {
        players[0].addToBottom(new Card(deck[i]));
        players[1].addToBottom(new Card(deck[i + 1]));
    }

    Cards pile;      // Pile into which cards are played
                     // (and from which they are taken if a player wins a trick)
    int cards = 0,   // Number of cards played
        tricks = 0,  // Number of tricks won
        turn = 0,    // Who's turn it is (0 or 1)
        penalty = 0; // Penalty left to pay (0 if no penalty)

    while (!players[0].empty() && !players[1].empty())
    {
        Card *playedCard = players[turn].takeFromTop(); // Take card from hand.
        pile.addToBottom(playedCard);                   // Place it in the pile.
        ++cards;                                        // Increment statistics.

        // If the player is NOT paying penalty:
        if (penalty == 0)
        {
            // Value of played card determines penalty for next player, if any.
            penalty = playedCard->value();
            turn = !turn; // Switch turns.
            continue;     // No other actions needed, next player continues.
        }

        // If the player IS playing penalty:
        if (penalty > 0) // (This check is redundant, but kept for clarity.)
        {
            --penalty; // A card was played, decrement remaining penalty.

            // If the played card was a penalty card:
            if (playedCard->value() > 0)
            {
                // Store its value, other player has to pay penalty next turn.
                penalty = playedCard->value();
                turn = !turn; // This player's penalty ceases, switch turns.
                continue;     // No other actions needed, next player continues.
            }

            // If the played card was a not a penalty card:
            if (playedCard->value() == 0) // (Redundant check, for clarity.)
            {
                // If remaining penalty is zero,
                // (current player has not played a penalty card)
                // the other player wins the trick, and takes the pile:
                if (penalty == 0)
                {
                    turn = !turn;                     // Switch turns.
                    players[turn].addToBottom(&pile); // Take pile.
                    ++tricks;                         // Increment statistics.
                    continue;                         // Next player continues.
                }

                // If there is still penalty left to pay:
                if (penalty > 0) // (Redundant check, for clarity.)
                {
                    // Turns are not switched,
                    // this player continues to pay penalty.
                    continue; // (Redundant, kept for clarity and consistency.)
                }
            }
        }
    }

    return {cards, tricks};
}