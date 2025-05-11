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

  /// @brief Integer representation of the card.
  /// @return 0, 1, 2, 3, or 4.
  int value() const;

  /// @brief Character representation of the card.
  /// @return '-', 'J', 'Q', 'K', or 'A'.
  char symbol() const;

  /// @brief Next card in the linked list.
  /// @return Pointer to the next card in the linked list.
  /// @note Can be null if this is the last card in the list.
  Card *next() const;

  // Grants Cards access to private members of Card.
  friend class Cards;

private:
  /// @brief The value of the card (0-4).
  /// 0 = Number card, 1 = Jack, 2 = Queen, 3 = King, 4 = Ace.
  int m_value = 0;

  /// @brief Pointer to the next card in the linked list.
  Card *m_next = nullptr;
};