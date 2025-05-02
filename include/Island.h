#include "Genome.h"
#include <vector>
#include <random>

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
