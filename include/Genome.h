#pragma once

#include <vector>
#include <iostream>

class Genome
{
private:
    std::vector<int> m_deck;
    std::vector<int> generateShuffledDeck();

public:
    // Genome();
    explicit Genome(const std::vector<int> &t_deck = {});

    void mutate(double rate);
    void crossover(const Genome &parent2, Genome &child1, Genome &child2, int cutPoint1 = -1, int cutPoint2 = -1) const;
    const std::vector<int> &getDeck() const;

    friend std::ostream &operator<<(std::ostream &os, const Genome &genome);
};
