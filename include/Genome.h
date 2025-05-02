#pragma once

#include <vector>
#include <iostream>

class Genome
{
private:
    std::vector<int> m_deck;
    std::vector<int> generateShuffledDeck();
    mutable int m_fitness = -1;

public:
    Genome();
    explicit Genome(const std::vector<int> t_deck);

    void mutate(double rate);
    Genome crossover(const Genome &parent2, int cutPoint1 = -1, int cutPoint2 = -1) const;
    const std::vector<int> &getDeck() const;
    int evaluate() const;

    friend std::ostream &operator<<(std::ostream &os, const Genome &genome);
};
