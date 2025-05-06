#pragma once

/// @brief Linked list node representing a card in the game.
/// @note Number cards (2–10) and suits are interchangeable in this game,
/// thus each card is represented by a single integer value (0-4).
class Card
{
public:
  /// @brief Constructs a card.
  /// @param value Value of the card.
  /// @throws std::invalid_argument if value is not between 0 and 4.
  Card(int value = 0);

  /// @brief Destructor.
  /// @note Also deletes the next cards in the linked list.
  ~Card();

  /// @brief Integer representation of the card.
  /// @return 0, 1, 2, 3, or 4.
  int getValue() const;

  /// @brief Character representation of the card.
  /// @return '-', 'J', 'Q', 'K', or 'A'.
  char getSymbol() const;

  /// @brief Pointer to the next card in the linked list.
  Card *m_next = nullptr;

private:
  /// @brief The value of the card (0-4).
  /// 0 = Number card, 1 = Jack, 2 = Queen, 3 = King, 4 = Ace.
  int m_value;
};