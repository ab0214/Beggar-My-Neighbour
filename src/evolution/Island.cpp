#include "evolution/Island.h"

#include "evolution/Genome.h"
#include "evolution/GenomeOperations.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

const int POPULATION_SIZE = 200;

Island::Island()
{
    // Fill initial population with random genomes
    m_population.reserve(POPULATION_SIZE);
    for (int i = 0; i < POPULATION_SIZE; ++i)
    {
        m_population.push_back(Genome());
    }
}

void Island::evolve()
{
    // Evaluate fitness of each genome
    for (auto &genome : m_population)
    {
        genome.evaluate();
    }

    // Sort by fitness descending
    std::sort(m_population.begin(), m_population.end(), [](const Genome &a, const Genome &b)
              {
                  return a.evaluate() > b.evaluate(); // higher fitness first
              });

    // Keep top 10% as elites
    int elite_count = POPULATION_SIZE / 10;
    std::vector<Genome> newPop;
    for (int i = 0; i < elite_count; ++i)
        newPop.push_back(m_population[i]);

    // Also add mutated versions of elites
    for (int i = 0; i < elite_count; ++i)
    {
        Genome original = m_population[i];
        Genome mutated = GenomeOperations::mutate(original, 0.05);
        newPop.push_back(mutated);
    }

    // Fill rest with children of elites
    std::mt19937 rng(std::random_device{}());
    std::vector<double> weights;
    for (int i = 0; i < elite_count; ++i)
        weights.push_back(pow(elite_count - i, 1.5)); // Higher rank = higher weight
    std::discrete_distribution<> dist(weights.begin(), weights.end());
    while (newPop.size() < POPULATION_SIZE)
    {
        int i1 = dist(rng);
        int i2;
        do
            i2 = dist(rng);
        while (i2 == i1);

        Genome parent1 = newPop[i1];
        Genome parent2 = newPop[i2];

        Genome child1 = GenomeOperations::crossover(parent1, parent2);
        Genome child2 = GenomeOperations::crossover(parent2, parent1);
        GenomeOperations::mutateInPlace(child1, 0.05);
        GenomeOperations::mutateInPlace(child2, 0.05);

        newPop.push_back(child1);
        if (newPop.size() < POPULATION_SIZE)
            newPop.push_back(child2);
    }

    m_population = std::move(newPop);
}

Genome Island::getBestGenome()
{
    return *std::max_element(m_population.begin(), m_population.end(),
                             [](Genome &a, Genome &b)
                             {
                                 return a.evaluate() < b.evaluate();
                             });
}

double Island::getAverageFitness()
{
    double totalFitness = 0.0;
    for (const Genome &g : m_population)
        totalFitness += g.evaluate();
    return totalFitness / m_population.size();
}