#include "evolution/PopulationOperations.h"

#include <random>
#include <vector>
#include <queue>

#include "evolution/Population.h"
#include "evolution/Genome.h"

std::vector<Genome> PopulationOperations::
    truncationSelection(const Population &population, size_t selectionSize)
{
    if (selectionSize > population.genomes().size())
        throw std::invalid_argument("Selection size exceeds population size.");

    // Compare function for the priority queue.
    auto compare = [](const Genome &a, const Genome &b)
    {
        return a.evaluate() > b.evaluate();
    };

    // Create a priority queue to keep track of the best genomes.
    std::priority_queue<Genome, std::vector<Genome>, decltype(compare)>
        queue(compare);

    // Iterate through the genomes in the population.
    for (const auto &g : population.genomes())
    {
        // If the queue is not full, simply add the genome to the queue.
        if (queue.size() < selectionSize)
        {
            queue.emplace(g);
        }
        // If the queue is full, only add the genome if it is better than
        // the worst one in the queue, and remove the worst one.
        else if (g.evaluate() > queue.top().evaluate())
        {
            queue.pop();
            queue.push(g);
        }
    }

    // Convert the priority queue to a vector and return it.
    std::vector<Genome> selected;
    selected.reserve(selectionSize);
    while (!queue.empty())
    {
        selected.push_back(queue.top());
        queue.pop();
    }
    return selected;
}

std::vector<Genome> PopulationOperations::
    truncationSelection(const Population &population, double percentage)
{
    size_t numSelections =
        static_cast<int>(population.genomes().size() * percentage);
    return truncationSelection(population, numSelections);
}