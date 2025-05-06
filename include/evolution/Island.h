#pragma once

#include <vector>

#include "evolution/Genome.h"

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
