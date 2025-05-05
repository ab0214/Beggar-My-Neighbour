#pragma once

class Card
{
private:
  int value;

public:
  Card(int v = 0);
  ~Card();

  int getValue() const;
  char getSymbol() const;

  Card *m_next = nullptr;
};