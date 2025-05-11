#include "game/Card.h"

#include <stdexcept>

Card::Card(int value) : m_value(value)
{
    if (value < 0 || value > 4)
    {
        throw std::invalid_argument("Card's value must be between 0 and 4");
    }
}

int Card::value() const
{
    return m_value;
}

char Card::symbol() const
{
    static const char symbols[] = {'-', 'J', 'Q', 'K', 'A'};
    return (m_value >= 1 && m_value <= 4) ? symbols[m_value] : '-';
}

Card *Card::next() const
{
    return m_next;
}