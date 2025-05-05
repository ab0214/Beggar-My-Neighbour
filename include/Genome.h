#pragma once

#include <vector>
#include <iostream>

class GenomeOperations; // Forward declaration

class Genome
{
private:
    std::vector<int> m_deck;
    std::vector<int> generateShuffledDeck();
    mutable int m_fitness = -1;

public:
    Genome();
    explicit Genome(const std::vector<int> t_deck);

    const std::vector<int> &getDeck() const;
    int evaluate() const;

    bool operator==(const Genome &other) const;
    bool operator!=(const Genome &other) const;

    friend std::ostream &operator<<(std::ostream &os, const Genome &genome);

    friend class GenomeOperations;
};
