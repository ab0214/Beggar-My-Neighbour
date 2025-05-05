#include "evolution/GenomeOperations.h"

#include "evolution/Genome.h"
#include <algorithm>
#include <array>
#include <random>

void GenomeOperations::mutateInPlace(Genome &genome, double rate)
{
    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<> dist(0.0, 1.0);

    for (size_t i = 0; i < genome.m_deck.size(); ++i)
    {
        if (dist(rng) < rate)
        {
            std::uniform_int_distribution<> swapDist(0, static_cast<int>(genome.m_deck.size()) - 1);
            std::swap(genome.m_deck[i], genome.m_deck[swapDist(rng)]);
        }
    }

    genome.m_fitness = -1; // Mark fitness as invalid
}

Genome GenomeOperations::mutate(const Genome &genome, double rate)
{
    Genome newGenome = genome;      // Copy the original genome
    mutateInPlace(newGenome, rate); // Reuse the in-place mutation logic
    return newGenome;
}

Genome GenomeOperations::crossover(const Genome &parent1, const Genome &parent2, int cutPoint1, int cutPoint2)
{
    cutPoint1 = std::clamp(cutPoint1, -1, 52);
    cutPoint2 = std::clamp(cutPoint2, -1, 52);

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

    if (cutPoint1 > cutPoint2)
    {
        std::swap(cutPoint1, cutPoint2);
    }

    Genome child(std::vector<int>(52));
    std::array<int, 5> cardCounts{};

    for (size_t i = 0; i < cutPoint1; ++i)
    {
        int value = parent1.m_deck[i];
        child.m_deck[i] = value;
        ++cardCounts[value];
    }

    for (size_t i = cutPoint2; i < 52; ++i)
    {
        int value = parent1.m_deck[i];
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

    child.m_fitness = -1; // Mark fitness as invalid
    return child;
}