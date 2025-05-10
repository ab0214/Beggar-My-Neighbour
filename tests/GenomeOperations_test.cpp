#include <gtest/gtest.h>
#include "evolution/Genome.h"
#include "evolution/GenomeOperations.h"

TEST(GenomeOperationsTests, Crossover)
{
    Genome parent1 = Genome();
    Genome parent2 = Genome();

    // Test crossover with no cut points
    Genome child1 = GenomeOperations::crossover(parent1, parent2, 0, 0);
    EXPECT_EQ(child1, parent1);

    // Test crossover with full cut points
    child1 = GenomeOperations::crossover(parent1, parent2, 0, 52);
    EXPECT_EQ(child1, parent2);

    // Test crossover with one point
    child1 = GenomeOperations::crossover(parent1, parent2, 0, 26);
    EXPECT_NE(child1, parent1);
    EXPECT_NE(child1, parent2);

    // Test crossover with two points
    child1 = GenomeOperations::crossover(parent1, parent2, 17, 32);
    EXPECT_NE(child1, parent1);
    EXPECT_NE(child1, parent2);

    // Test crossover with random cut points
    bool different = false;
    for (int i = 0; i < 100; ++i)
    {
        child1 = GenomeOperations::crossover(parent1, parent2);
        if (child1 != parent1 && child1 != parent2)
        {
            different = true;
            break;
        }
    }
    EXPECT_TRUE(different);
}

TEST(GenomeOperationsTests, Mutation)
{
    Genome original = Genome();
    Genome copy = original;

    // Test copy mutation
    Genome mutated = GenomeOperations::mutate(original, 0.5);
    EXPECT_NE(original, mutated);
    EXPECT_EQ(original, copy);

    // Test in-place mutation
    GenomeOperations::mutateInPlace(copy, 0.5);
    EXPECT_NE(original, copy);
}