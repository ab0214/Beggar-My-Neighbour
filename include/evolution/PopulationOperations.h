#pragma once

#include "evolution/Population.h"
#include "evolution/Genome.h"

class PopulationOperations
{
public:
    /// @brief Performs truncation selection on a population.
    /// @param population The population to select from.
    /// @param selectionSize The number of genomes to select.
    /// @return A vector of copies of the selected genomes.
    static std::vector<Genome> truncationSelection(
        const Population &population, size_t selectionSize);

    /// @brief Performs truncation selection on a population.
    /// @param population The population to select from.
    /// @param selectionRate The fraction of the population to select (0-1).
    /// @return A vector of copies of the selected genomes.
    static std::vector<Genome> truncationSelection(
        const Population &population, double selectionRate);
};