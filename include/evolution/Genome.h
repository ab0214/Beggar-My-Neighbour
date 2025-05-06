#pragma once

#include <vector>
#include <ostream>

/// @brief Genome used in evolutionary algorithms.
/// @details Represents the initial deck order before dealing cards to players.
/// A genome can be evaluated by simulating a game,
/// with fitness based on total cards played.
class Genome
{
public:
    /// @brief Constructs an instance with a randomly shuffled deck of cards.
    Genome();

    /// @brief Constructs an instance with a given deck of cards.
    /// @param deck The deck of cards to use.
    Genome(const std::vector<int> deck);

    /// @brief Gets a read-only reference to the deck of cards.
    /// @return A read-only reference to the deck of cards.
    const std::vector<int> &getDeck() const;

    /// @brief Evaluates the fitness of the genome by playing a game.
    /// @return Number of cards played, the fitness score of the genome.
    /// @note Returns a cached value if already evaluated and unmodified.
    int evaluate() const;

    /// @brief Compares two genomes for equality.
    /// @param other The genome to compare with.
    bool operator==(const Genome &other) const;

    /// @brief Compares two genomes for inequality.
    /// @param other The genome to compare with.
    bool operator!=(const Genome &other) const;

    /// @brief Insertion operator.
    /// Used for printing the genome to an output stream.
    friend std::ostream &operator<<(std::ostream &os, const Genome &genome);

    // Grants GenomeOperations access to private members of Genome.
    friend class GenomeOperations;

private:
    /// @brief The deck of cards represented as a vector of integers.
    std::vector<int> m_deck;

    /// @brief Generates a shuffled deck of cards.
    /// @return A vector of integers representing a shuffled deck of cards.
    std::vector<int> generateShuffledDeck();

    /// @brief The fitness score of the genome.
    /// @note Initialized to -1, indicating that it has not been evaluated yet.
    /// @note If the genome is modified, this must be reset to -1.
    mutable int m_fitness = -1;
};
