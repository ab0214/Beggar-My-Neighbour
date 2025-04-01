#include <algorithm>
#include <chrono>
#include <ctime>
#include <iostream>
#include <tuple>
#include <vector>
#include <mutex>
#include <thread>
#include <future>
#include <random>

class Card
{
private:
    int value;

public:
    Card *next = nullptr;

    Card(int v = 0) : value(v)
    {
        if (v < 0 || v > 4)
        {
            throw std::invalid_argument("Card's value must be between 0 and 4");
        }
    }

    ~Card()
    {
        auto top = next;
        while (top != nullptr)
        {
            auto current = top;
            top = current->next;
            current->next = nullptr; // Prevent recursion.
            delete current;
        }
    }

    int getValue() const
    {
        return value;
    }

    char getSymbol() const
    {
        static const char symbols[] = {'-', 'J', 'Q', 'K', 'A'};
        return (value >= 1 && value <= 4) ? symbols[value] : '-';
    }
};

class Cards
{
private:
    Card *top = nullptr;
    Card *bottom = nullptr;

public:
    Cards() {}

    ~Cards()
    {
        delete top;
    }

    Card *getBottom() { return bottom; }

    bool isEmpty() const
    {
        return (top == nullptr || bottom == nullptr);
    }

    void insertTop(Card *newCard)
    {
        if (isEmpty())
        {
            top = newCard;
            bottom = newCard;
            newCard->next = nullptr;
            return;
        }

        newCard->next = top;
        top = newCard;
    }

    void insertBottom(Card *newCard)
    {
        if (isEmpty())
        {
            top = newCard;
            bottom = newCard;
            newCard->next = nullptr;
            return;
        }

        bottom->next = newCard;
        bottom = newCard;
        bottom->next = nullptr;
    }

    void splice(Cards *newCards)
    {
        if (newCards->isEmpty())
        {
            return;
        }

        if (isEmpty())
        {
            top = newCards->top;
            bottom = newCards->bottom;
            return;
        }

        bottom->next = newCards->top;
        bottom = newCards->bottom;

        newCards->clear();
    }

    void clear()
    {
        top = nullptr;
        bottom = nullptr;
    }

    Card *pop()
    {
        if (top == nullptr)
            return nullptr;
        auto temp = top;
        top = top->next;
        if (top == nullptr)
            bottom = nullptr;
        return temp;
    }

    friend std::ostream &operator<<(std::ostream &os, const Cards &cards)
    {
        Card *current = cards.top;
        while (current != nullptr)
        {
            os << current->getSymbol();
            current = current->next;
        }
        return os;
    }
};

std::tuple<int, int> play_game()
{

    std::vector<int> deck(52, 0); // 52 cards that will be dealt to the players
    for (int i = 0; i < 16; ++i)  // Add four of each picture card to the deck (replacing number cards)
        deck[i] = i / 4 + 1;

    std::random_device rd;                     // Random device to get a seed
    std::mt19937 g(rd());                      // Mersenne Twister engine initialized with the seed
    std::shuffle(deck.begin(), deck.end(), g); // Shuffle the deck

    Cards players[] = {Cards(), Cards()};
    for (int i = 0; i < deck.size(); i += 2)
    {
        players[0].insertBottom(new Card(deck[i]));
        players[1].insertBottom(new Card(deck[i + 1]));
    }

    Cards pile = Cards();

    int cards = 0;
    int tricks = 0;

    int turn = 0;
    int penalty = 0;

    while (!players[0].isEmpty() && !players[1].isEmpty())
    {
        // std::cout << "Cards    : " << cards << std::endl
        //           << "Tricks   : " << tricks << std::endl
        //           << "Player 1 : " << players[0] << std::endl
        //           << "Player 2 : " << players[1] << std::endl
        //           << "Pile     : " << pile << std::endl
        //           << std::endl;

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

std::tuple<int, int> play_games(int games)
{
    int max_cards = 0;
    int max_tricks = 0;

    for (int i = 0; i < games; ++i)
    {
        auto [cards, tricks] = play_game();
        max_cards = cards > max_cards ? cards : max_cards;
        max_tricks = tricks > max_tricks ? tricks : max_tricks;
    }

    return {max_cards, max_tricks};
}

int main()
{
    int num_threads = 6;
    int games_per_thread = 100000;
    int max_cards = 0;
    int max_tricks = 0;

    while (true)
    {
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<std::future<std::tuple<int, int>>> futures;

        for (int i = 0; i < num_threads; ++i)
        {
            futures.push_back(std::async(std::launch::async, play_games, games_per_thread));
        }

        for (auto &f : futures)
        {
            auto [cards, tricks] = f.get();
            max_cards = cards > max_cards ? cards : max_cards;
            max_tricks = tricks > max_tricks ? tricks : max_tricks;
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_s = end - start;
        std::chrono::duration<double, std::micro> elapsed_us = end - start;

        int games = num_threads * games_per_thread;
        int avg_us = elapsed_us.count() / games;
        std::cout << "Played " << games << " games in " << elapsed_s.count() << " seconds (Avg: " << avg_us << " us per game)." << std::endl
                  << "Most cards so far: " << max_cards << std::endl
                  << "Most tricks so far: " << max_tricks << std::endl;
    }

    return 0;
}