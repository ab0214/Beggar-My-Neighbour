#pragma once

#include <vector>

#include "evolution/Genome.h"

class Population
{
public:
    /// @brief Constructs a population with a give number of genomes.
    /// @param populationSize The number of genomes in the population.
    Population(size_t populationSize);

    /// @brief Constructs a population with a given set of genomes.
    /// @param genomes The genomes to include in the population.
    Population(std::vector<Genome> genomes);

    /// @brief Gets a read-only reference to the genomes in the population.
    /// @return A read-only reference to the genomes in the population.
    const std::vector<Genome> &genomes() const;

    /// @brief Gets a read-only reference to the best genome in the population.
    /// @return A read-only reference to the best genome in the population.
    const Genome &bestGenome() const;

    /// @brief Computes the average fitness of the population.
    /// @return The average fitness of the population.
    /// @note Returns 0.0 if the population is empty.
    const double averageFitness() const;

    /// @brief Computes the total fitness of the population.
    /// @return The sum of the fitness scores of all genomes in the population.
    /// @note Returns 0.0 if the population is empty.
    const double totalFitness() const;

    /// @brief Sorts the genomes in the population
    /// by fitness in descending order.
    /// @return A copy of the genomes sorted by fitness.
    /// @note The original population remains unchanged.
    std::vector<Genome> Population::sortGenomes() const;

private:
    std::vector<Genome> m_genomes;

    // Grant PopulationOperations access to private members of Population.
    friend class PopulationOperations;
};