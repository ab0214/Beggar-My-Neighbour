#pragma once

#include <ostream>
#include <vector>

#include <game/Card.h>

/// @brief Linked list of cards.
/// Used where fast iteration, and insertion are needed.
class Cards
{
public:
    /// @brief Constructs an empty linked list of cards.
    Cards();

    /// @brief Destructor.
    /// @note Also deletes the cards in the linked list.
    ~Cards();

    /// @brief Returns the bottom card in the linked list.
    /// @return Pointer to the bottom card.
    Card *bottom() const;

    /// @brief Checks if the linked list is empty.
    /// @return True if the linked list is empty, false otherwise.
    bool empty() const;

    /// @brief Inserts a card at the top of the linked list.
    /// @param card Pointer to the card to be inserted.
    void addToTop(Card *card);

    /// @brief Inserts a card at the bottom of the linked list.
    /// @param card Pointer to the card to be inserted.
    void addToBottom(Card *card);

    /// @brief Moves all cards from another linked to the end of this one.
    /// @param other Pointer to the linked list of cards to be moved.
    void addToBottom(Cards *other);

    /// @brief Removes and returns the top card from the linked list.
    /// @return Pointer to the removed card.
    /// @note Can be null if the list is empty.
    Card *takeFromTop();

    /// @brief Insertion operator.
    /// Used for printing the cards to an output stream.
    friend std::ostream &operator<<(std::ostream &ostream, const Cards &cards);

private:
    /// @brief Pointer to the top card in the linked list.
    Card *m_top = nullptr;

    /// @brief Pointer to the bottom card in the linked list.
    Card *m_bottom = nullptr;

    /// @brief Clears the linked list.
    /// @note Does not delete the cards, only removes them from the list.
    void clear();
};
