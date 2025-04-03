#include "Cards.h"

Cards::Cards() {}

Cards::~Cards() { delete m_top; }

Card *Cards::getBottom() { return m_bottom; }

bool Cards::isEmpty() const { return (m_top == nullptr || m_bottom == nullptr); }

void Cards::insertTop(Card *t_newCard)
{
    if (isEmpty())
    {
        m_top = t_newCard;
        m_bottom = t_newCard;
        t_newCard->m_next = nullptr;
        return;
    }

    t_newCard->m_next = m_top;
    m_top = t_newCard;
}

void Cards::insertBottom(Card *t_newCard)
{
    if (isEmpty())
    {
        m_top = t_newCard;
        m_bottom = t_newCard;
        t_newCard->m_next = nullptr;
        return;
    }

    m_bottom->m_next = t_newCard;
    m_bottom = t_newCard;
    m_bottom->m_next = nullptr;
}

void Cards::splice(Cards *t_newCards)
{
    if (t_newCards->isEmpty())
    {
        return;
    }

    if (isEmpty())
    {
        m_top = t_newCards->m_top;
        m_bottom = t_newCards->m_bottom;
        return;
    }

    m_bottom->m_next = t_newCards->m_top;
    m_bottom = t_newCards->m_bottom;

    t_newCards->clear();
}

void Cards::clear()
{
    m_top = nullptr;
    m_bottom = nullptr;
}

Card *Cards::pop()
{
    if (m_top == nullptr)
        return nullptr;
    auto temp = m_top;
    m_top = m_top->m_next;
    if (m_top == nullptr)
        m_bottom = nullptr;
    return temp;
}

std::ostream &operator<<(std::ostream &t_ostream, const Cards &t_cards)
{
    Card *current = t_cards.m_top;
    while (current != nullptr)
    {
        t_ostream << current->getSymbol();
        current = current->m_next;
    }
    return t_ostream;
}
