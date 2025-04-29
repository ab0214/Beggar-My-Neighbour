#include "Genome.h"
#include <algorithm>
#include <random>

// Genome::Genome() : m_deck(52) {}

Genome::Genome(const std::vector<int> &t_deck) : m_deck(t_deck)
{
    if (t_deck.empty())
    {
        m_deck = generateShuffledDeck();
    }
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
}

void Genome::crossover(const Genome &parent2, Genome &child1, Genome &child2, int cutPoint1, int cutPoint2) const
{
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

    // Create two children based on the crossover points
    child1.m_deck.clear();
    child2.m_deck.clear();

    // Child 1 gets the beginning of parent 1 (this)
    child1.m_deck.insert(
        child1.m_deck.begin(),
        m_deck.begin(),
        m_deck.begin() + cutPoint1);
    // Child 1 gets the middle of parent 2 (other)
    child1.m_deck.insert(
        child1.m_deck.begin() + cutPoint1,
        parent2.m_deck.begin() + cutPoint1,
        parent2.m_deck.begin() + cutPoint2);
    // Child 1 gets the end of parent 1 (this)
    child1.m_deck.insert(
        child1.m_deck.begin() + cutPoint2,
        m_deck.begin() + cutPoint2,
        m_deck.end());

    // Child 2 gets the beginning of parent 2 (other)
    child2.m_deck.insert(
        child2.m_deck.begin(),
        parent2.m_deck.begin(),
        parent2.m_deck.begin() + cutPoint1);
    // Child 2 gets the middle of parent 1 (this)
    child2.m_deck.insert(
        child2.m_deck.begin() + cutPoint1,
        m_deck.begin() + cutPoint1,
        m_deck.begin() + cutPoint2);
    // Child 2 gets the end of parent 2 (other)
    child2.m_deck.insert(
        child2.m_deck.begin() + cutPoint2,
        parent2.m_deck.begin() + cutPoint2,
        parent2.m_deck.end());
}

const std::vector<int> &Genome::getDeck() const
{
    return m_deck;
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
