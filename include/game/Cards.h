#pragma once

#include <ostream>
#include <vector>

class Card; // Forward declaration

class Cards
{
private:
    Card *m_top = nullptr;
    Card *m_bottom = nullptr;

public:
    Cards();
    ~Cards();

    Card *getBottom();
    bool isEmpty() const;

    void insertTop(Card *t_newCard);
    void insertBottom(Card *t_newCard);
    void splice(Cards *t_newCards);
    void clear();
    Card *pop();

    friend std::ostream &operator<<(std::ostream &t_ostream, const Cards &t_cards);
};
