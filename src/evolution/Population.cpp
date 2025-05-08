#include "evolution/Population.h"

#include <algorithm>
#include <stdexcept>
#include <vector>

#include "evolution/Genome.h"

Population::Population(size_t populationSize) : m_genomes(populationSize) {}

Population::Population(std::vector<Genome> genomes) : m_genomes(std::move(genomes)) {}

const std::vector<Genome> &Population::genomes() const
{
    return m_genomes;
}

const Genome &Population::bestGenome() const
{
    if (m_genomes.empty())
        throw std::runtime_error("Population is empty");

    return *std::max_element(m_genomes.begin(), m_genomes.end(),
                             [](const Genome &a, const Genome &b)
                             {
                                 return a.evaluate() < b.evaluate();
                             });
}

const double Population::averageFitness() const
{
    return m_genomes.empty() ? 0.0 : // Avoid division by zero
               totalFitness() / m_genomes.size();
}

const double Population::totalFitness() const
{
    double totalFitness = 0.0;
    for (const Genome &g : m_genomes)
        totalFitness += g.evaluate();
    return totalFitness;
}

std::vector<Genome> Population::sortGenomes() const
{
    std::vector<Genome> sortedGenomes;
    for (const auto &genome : m_genomes)
        sortedGenomes.emplace_back(genome);

    std::sort(sortedGenomes.begin(), sortedGenomes.end(), [](const Genome a, const Genome b)
              {
                  return a.evaluate() > b.evaluate(); // Higher fitness first
              });

    return sortedGenomes;
}
