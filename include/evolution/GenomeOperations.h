#pragma once

class Genome; // Forward declaration

class GenomeOperations
{
public:
    static void mutateInPlace(Genome &genome, double rate);
    static Genome mutate(const Genome &genome, double rate);
    static Genome crossover(const Genome &parent1, const Genome &parent2, int cutPoint1 = -1, int cutPoint2 = -1);
};