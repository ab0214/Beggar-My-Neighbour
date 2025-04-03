#include "Card.h"
#include <stdexcept>

Card::Card(int v) : value(v)
{
    if (v < 0 || v > 4)
    {
        throw std::invalid_argument("Card's value must be between 0 and 4");
    }
}

Card::~Card()
{
    auto top = m_next;
    while (top != nullptr)
    {
        auto current = top;
        top = current->m_next;
        current->m_next = nullptr; // Prevent recursion.
        delete current;
    }
}

int Card::getValue() const
{
    return value;
}

char Card::getSymbol() const
{
    static const char symbols[] = {'-', 'J', 'Q', 'K', 'A'};
    return (value >= 1 && value <= 4) ? symbols[value] : '-';
}
