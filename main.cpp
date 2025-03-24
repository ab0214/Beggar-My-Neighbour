#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <array>
#include <deque>
#include <stack>
#include <algorithm> // For std::shuffle
#include <random>    // For std::random_device, std::mt19937
#include <ctime>
#include <tuple>

const char intToCard[] = {'-', 'J', 'Q', 'K', 'A'};

int cards_played = 0;
int rounds_played = 0;

std::vector<int> pile;
std::deque<int> alice;
std::deque<int> bob;

template <typename Container>
void print_cards(const Container &cards, const char name[])
{
    std::cout << name << ": ";
    for (int card : cards)
    {
        std::cout << intToCard[card];
    }
    std::cout << std::endl;
}

void print_stacks()
{
    // std::cout << name << std::endl;
    print_cards(alice, "Alice ");
    print_cards(bob, "Bob   ");
    print_cards(pile, "Pile  ");
    std::cout << std::endl;
}

// Moves the player's top card to the pile.
// Returns the value of the played card.
int play_card(std::deque<int> &player)
{
    int played_card = player.back();
    player.pop_back();
    pile.push_back(played_card);
    ++cards_played;
    return played_card;
}

int play_penalty(std::deque<int> &player, int penalty)
{
    int size = player.size();
    penalty = std::min(size, penalty);
    for (int played = 0; played < penalty; ++played)
    {
        int played_card = play_card(player);
        // std::cout << intToCard[played_card];
        if (played_card > 0)
        {
            return played_card;
        }
    }
    return 0;
}

void win_round(std::deque<int> &player)
{
    player.insert(player.begin(), pile.rbegin(), pile.rend());
    pile.clear();
    ++rounds_played;
}

std::tuple<int, int> play_game()
{
    cards_played = 0;
    rounds_played = 0;

    std::vector<int> deck(52, 0); // 52 cards that will be dealt to the players
    for (int i = 0; i < 16; ++i)  // Add four of each picture card to the deck (replacing number cards)
        deck[i] = i / 4 + 1;

    // std::random_device rd;                  // Random device to get a seed
    // std::mt19937 g(rd());                    // Mersenne Twister engine initialized with the seed
    srand(time(0));
    std::random_shuffle(deck.begin(), deck.end()); // Shuffle the deck

    // Deal the deck to two players (split at the middle)
    alice.clear();
    alice.assign(deck.begin(), deck.begin() + deck.size() / 2);
    bob.clear();
    bob.assign(deck.begin() + deck.size() / 2, deck.end());

    // std::cout << std::endl
    //           << std::endl
    //           << "================" << std::endl
    //           << std::endl;
    // std::cout << "Starting stacks:" << std::endl;
    // print_stacks();

    std::deque<int> *players[] = {&alice, &bob};
    const char *names[] = {"Alice", "Bob"};
    int turn = 0;

    int last_card;
    do
    {
        // std::cout << names[turn] << " plays ";
        last_card = play_card(*players[turn]);
        // std::cout << intToCard[last_card] << "." << std::endl;
        // print_stacks();

        while (last_card > 0)
        {
            // std::cout << names[!turn] << " must pay " << last_card << " penalty." << std::endl;
            // std::cout << names[!turn] << " pays ";
            last_card = play_penalty(*players[!turn], last_card);
            // std::cout << " as penalty. " << std::endl;
            // print_stacks();

            if (last_card == 0)
            {
                // std::cout << names[turn] << " wins the round!" << std::endl
                //           << std::endl
                //           << "Next round: " << std::endl;
                win_round(*players[turn]);
                // print_stacks();
            }
            turn = !turn;
        }
        turn = !turn;
    } while (last_card == 0 && (*players[turn]).size() > 0);

    // std::cout << names[!turn] << " won!" << std::endl
    //           << "Rounds: " << rounds_played << std::endl
    //           << "Cards: " << cards_played << std::endl;

    return std::make_tuple(rounds_played, cards_played);
}

int main()
{
    const int games = 10000;
    int max_rounds = 0;
    int max_cards = 0;
    for (int i = 0; i < games; ++i)
    {
        auto result = play_game();
        int rounds = std::get<0>(result);
        int cards = std::get<1>(result);
        max_rounds = std::max(max_rounds, rounds);
        max_cards = std::max(max_cards, cards);
    }
    std::cout << "Played " << games << " games." << std::endl
              << "Most rounds played: " << max_rounds << std::endl
              << "Most cards played: " << max_cards << std::endl;

    return 0;
}