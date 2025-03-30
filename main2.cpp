#include <algorithm>
#include <ctime>
#include <iostream>
#include <vector>

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
        while (top != nullptr) {
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

    bool isEmpty() const
    {
        return (top == nullptr || bottom == nullptr);
    }

    void addToTop(Card *newCard)
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

    void addToBottom(Card *newCard)
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

    void addToBottom(Cards *newCards)
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
    }

    friend std::ostream &operator<<(std::ostream &os, const Cards &cards)
    {
        Card *current = cards.top;
        while (current != nullptr && current != cards.bottom)
        {
            os << current->getSymbol();
            current = current->next;
        }
        return os;
    }
};

int main()
{
    std::vector<int> deck(52, 0); // 52 cards that will be dealt to the players
    for (int i = 0; i < 16; ++i)  // Add four of each picture card to the deck (replacing number cards)
        deck[i] = i / 4 + 1;

    srand(time(0));
    std::random_shuffle(deck.begin(), deck.end()); // Shuffle the deck

    Cards player1 = Cards();
    Cards player2 = Cards();

    for (int i = 0; i < deck.size(); i += 2)
    {
        player1.addToBottom(new Card(deck[i]));
        player2.addToBottom(new Card(deck[i + 1]));
    }

    std::cout << player1 << std::endl;
    std::cout << player2 << std::endl;

    return 0;
}