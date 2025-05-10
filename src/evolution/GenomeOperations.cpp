#include "evolution/GenomeOperations.h"

#include <algorithm>
#include <array>
#include <random>

#include "evolution/Genome.h"

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

Genome GenomeOperations::crossover(const Genome &parent1, const Genome &parent2,
                                   int cutPoint1, int cutPoint2)
{
    // Ensure cut points are within valid range
    cutPoint1 = std::clamp(cutPoint1, -1, 52);
    cutPoint2 = std::clamp(cutPoint2, -1, 52);

    // If cut points have not been set, generate random cut points
    // Does not allow random cut points to be 0 or 52,
    // or the same as the other cut point.
    if (cutPoint1 < 0 || cutPoint2 < 0)
    {
        std::random_device rd;
        std::mt19937 g(rd());
        if (cutPoint1 < 0)
            do
                cutPoint1 = std::uniform_int_distribution<>(1, 51)(g);
            while (cutPoint1 == cutPoint2);
        if (cutPoint2 < 0)
            do
                cutPoint2 = std::uniform_int_distribution<>(1, 51)(g);
            while (cutPoint2 == cutPoint1);
    }

    // Ensure cut points are in the correct order
    if (cutPoint1 > cutPoint2)
        std::swap(cutPoint1, cutPoint2);

    // Initialize a child genome with an empty vector
    Genome child(std::vector<int>(52));

    // Create an array to track the number of each type of card in the child.
    // This is to ensure the result is a valid deck of cards.
    // (36 number cards, 4 of each penalty card).
    std::array<int, 5> cardCounts{};

    // Fill from start to cutPoint1 with cards from parent1.
    for (size_t i = 0; i < cutPoint1; ++i)
    {
        int value = parent1.m_deck[i];
        child.m_deck[i] = value;
        ++cardCounts[value];
    }
    // Fill from cutPoint2 to end with cards from parent2.
    for (size_t i = cutPoint2; i < 52; ++i)
    {
        int value = parent1.m_deck[i];
        child.m_deck[i] = value;
        ++cardCounts[value];
    }

    // Fill the rest with cards from parent2, using order crossover.
    // (Ensures that the resulting genome is a valid deck of cards.)
    for (size_t i = cutPoint1, j = 0; i < cutPoint2; ++i)
    {
        int value, maxAllowedCount;

        // Find the next card from parent2, that is valid to add to the child.
        // (i.e., not exceeding the max allowed count for that card type)
        do
        {
            value = parent2.m_deck[j++];
            // A valid deck has 36 number cards (value 0),
            // and 4 of each penalty card. (values 1-4).
            maxAllowedCount = value == 0 ? 36 : 4;
        } while (cardCounts[value] >= maxAllowedCount);

        child.m_deck[i] = value; // Add the valid card to the child genome.
        ++cardCounts[value];     // Increment the count for that card type.
    }

    child.m_fitness = -1; // Mark fitness as invalid (to be recalculated).
    return child;
}