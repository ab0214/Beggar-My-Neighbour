#pragma once

#include "evolution/Genome.h"

/// @brief Class for genetic operators, such as mutation and crossover.
class GenomeOperations
{
public:
    /// @brief Mutates a genome in place, modifying its state.
    /// @param genome The genome to mutate.
    /// @param rate The mutation rate (0.0 to 1.0).
    static void mutateInPlace(Genome &genome, double rate);

    /// @brief Mutates a genome, returning a new mutated genome.
    /// @param genome The genome to mutate.
    /// @param rate The mutation rate (0.0 to 1.0).
    static Genome mutate(const Genome &genome, double rate);

    /// @brief Performs crossover between two genomes, returning a new genome.
    /// @param parent1 The first parent genome.
    /// @param parent2 The second parent genome.
    /// @param cutPoint1 The first cut point (default -1 for random).
    /// @param cutPoint2 The second cut point (default -1 for random).
    /// @return A new genome resulting from the crossover.
    /// @note For one-point crossover, set one of the cut points to 0.
    /// @note If you want two children, swap the parents and call again.
    static Genome crossover(const Genome &parent1, const Genome &parent2,
                            int cutPoint1 = -1, int cutPoint2 = -1);
};