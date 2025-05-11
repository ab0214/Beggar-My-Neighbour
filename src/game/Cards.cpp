#include "game/Cards.h"

#include <ostream>

#include "game/Card.h"

Cards::Cards() {}

Cards::~Cards()
{
    while (m_top != nullptr)
    {
        Card *temp = m_top->m_next;
        m_top->m_next = nullptr; // Prevent recursion if Card has a destructor.
        delete m_top;
        m_top = temp;
    }
}

Card *Cards::bottom() const { return m_bottom; }

bool Cards::empty() const
{
    return (m_top == nullptr || m_bottom == nullptr);
}

void Cards::addToTop(Card *card)
{
    if (empty())
    {
        m_top = card;
        m_bottom = card;
        card->m_next = nullptr;
        return;
    }

    card->m_next = m_top;
    m_top = card;
}

void Cards::addToBottom(Card *card)
{
    if (empty())
    {
        m_top = card;
        m_bottom = card;
        card->m_next = nullptr;
        return;
    }

    m_bottom->m_next = card;
    m_bottom = card;
    m_bottom->m_next = nullptr;
}

void Cards::addToBottom(Cards *other)
{
    if (other->empty())
    {
        return;
    }

    if (empty())
    {
        m_top = other->m_top;
        m_bottom = other->m_bottom;
        return;
    }

    m_bottom->m_next = other->m_top;
    m_bottom = other->m_bottom;

    other->clear();
}

Card *Cards::takeFromTop()
{
    if (m_top == nullptr)
        return nullptr;
    auto temp = m_top;
    m_top = m_top->m_next;
    if (m_top == nullptr)
        m_bottom = nullptr;
    return temp;
}

std::ostream &operator<<(std::ostream &ostream, const Cards &cards)
{
    Card *current = cards.m_top;
    while (current != nullptr)
    {
        ostream << current->symbol();
        current = current->next();
    }
    return ostream;
}

void Cards::clear()
{
    m_top = nullptr;
    m_bottom = nullptr;
}