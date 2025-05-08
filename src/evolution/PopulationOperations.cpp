#include "evolution/PopulationOperations.h"

#include <random>
#include <vector>

#include "evolution/Population.h"
#include "evolution/Genome.h"

std::vector<Genome> PopulationOperations::
    truncationSelection(const Population &population, size_t selectionSize)
{
    if (selectionSize > population.genomes().size())
        throw std::invalid_argument("Selection size exceeds population size.");

    // Copy and sort genomes by ascending fitness
    auto sortedGenomes = population.sortGenomes();

    std::vector<Genome> selected(sortedGenomes.begin(),
                                 sortedGenomes.begin() + selectionSize);

    return selected;
}

std::vector<Genome> PopulationOperations::
    truncationSelection(const Population &population, double percentage)
{
    size_t numSelections =
        static_cast<int>(population.genomes().size() * percentage);
    return truncationSelection(population, numSelections);
}