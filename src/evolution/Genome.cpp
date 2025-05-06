#include "evolution/Genome.h"

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include "game/Game.h"

Genome::Genome()
{
    m_deck = generateShuffledDeck();
}

Genome::Genome(const std::vector<int> t_deck)
{
    m_deck = t_deck;
}

std::vector<int> Genome::generateShuffledDeck()
{
    std::vector<int> deck(52, 0);
    for (int i = 0; i < 16; ++i)
        deck[i] = i / 4 + 1;

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
    return deck;
}

const std::vector<int> &Genome::getDeck() const
{
    return m_deck;
}

int Genome::evaluate() const
{
    if (m_fitness == -1)
    {
        auto [cards, tricks] = Game::playGame(m_deck);
        m_fitness = cards;
    }
    return m_fitness;
}

bool Genome::operator==(const Genome &other) const
{
    if (m_fitness != other.m_fitness &&
        m_fitness != -1 &&
        other.m_fitness != -1)
        return false;

    return m_deck == other.m_deck;
}

bool Genome::operator!=(const Genome &other) const
{
    return !(*this == other);
}

std::ostream &operator<<(std::ostream &os, const Genome &genome)
{
    static const char symbols[] = {'-', 'J', 'Q', 'K', 'A'};

    for (int val : genome.m_deck)
    {
        char symbol = (val >= 1 && val <= 4) ? symbols[val] : '-';
        os << symbol;
    }
    return os;
}
