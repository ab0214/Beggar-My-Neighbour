#include "Cards.h"
#include <tuple>
#include <vector>

class Game
{
public:
    static std::tuple<int, int> playGame();

private:
    static std::vector<int> generateShuffledDeck();
};
