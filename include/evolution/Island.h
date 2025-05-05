#pragma once

#include <vector>

class Genome; // Forward declaration

class Island
{
private:
    std::vector<Genome> m_population;

public:
    Island();
    void evolve();
    Genome getBestGenome();
    double getAverageFitness();
};
