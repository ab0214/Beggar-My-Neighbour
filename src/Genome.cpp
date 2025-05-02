#include "Genome.h"
#include "Game.h"
#include <algorithm>
#include <array>
#include <random>

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

void Genome::mutate(double rate)
{
    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<> dist(0.0, 1.0);

    for (size_t i = 0; i < m_deck.size(); ++i)
    {
        if (dist(rng) < rate)
        {
            std::uniform_int_distribution<> swapDist(0, static_cast<int>(m_deck.size()) - 1);
            std::swap(m_deck[i], m_deck[swapDist(rng)]);
        }
    }

    evaluate();
}

Genome Genome::crossover(const Genome &parent2, int cutPoint1, int cutPoint2) const
{
    cutPoint1 = std::clamp(cutPoint1, -1, 52);
    cutPoint2 = std::clamp(cutPoint2, -1, 52);

    // If the cut points are -1, randomize them
    if (cutPoint1 == -1)
    {
        std::random_device rd;
        std::mt19937 g(rd());
        cutPoint1 = std::uniform_int_distribution<>(0, 51)(g);
    }
    if (cutPoint2 == -1)
    {
        std::random_device rd;
        std::mt19937 g(rd());
        cutPoint2 = std::uniform_int_distribution<>(0, 51)(g);
    }

    // Make sure cutPoint1 is less than cutPoint2
    if (cutPoint1 > cutPoint2)
    {
        std::swap(cutPoint1, cutPoint2);
    }

    Genome child = Genome(std::vector<int>(52));

    std::array<int, 5> cardCounts{};

    for (size_t i = 0; i < cutPoint1; ++i)
    {
        int value = m_deck[i];
        child.m_deck[i] = value;
        ++cardCounts[value];
    }

    for (size_t i = cutPoint2; i < 52; ++i)
    {
        int value = m_deck[i];
        child.m_deck[i] = value;
        ++cardCounts[value];
    }

    for (size_t i = cutPoint1, j = 0; i < cutPoint2; ++i)
    {
        int value, maxAllowedCount;

        do
        {
            value = parent2.m_deck[j++];
            maxAllowedCount = value == 0 ? 36 : 4;
        } while (cardCounts[value] >= maxAllowedCount);

        child.m_deck[i] = value;
        ++cardCounts[value];
    }

    return child;
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
