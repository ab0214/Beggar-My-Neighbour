#include <algorithm>
#include <chrono>
#include <ctime>
#include <future>
#include <iostream>
#include <random>
#include <tuple>
#include <vector>

class Card {
private:
  int m_value;

public:
  Card *m_next = nullptr;

  Card(int v = 0) : m_value(v) {
    if (v < 0 || v > 4) {
      throw std::invalid_argument("Card's value must be between 0 and 4");
    }
  }

  ~Card() {
    auto top = m_next;
    while (top != nullptr) {
      auto current = top;
      top = current->m_next;
      current->m_next = nullptr; // Prevent recursion.
      delete current;
    }
  }

  int getValue() const { return m_value; }

  char getSymbol() const {
    static const char symbols[] = {'-', 'J', 'Q', 'K', 'A'};
    return (m_value >= 1 && m_value <= 4) ? symbols[m_value] : '-';
  }
};

class Cards {
private:
  Card *m_top = nullptr;
  Card *m_bottom = nullptr;

public:
  Cards() {}

  ~Cards() { delete m_top; }

  Card *getBottom() { return m_bottom; }

  bool isEmpty() const { return (m_top == nullptr || m_bottom == nullptr); }

  void insertTop(Card *t_newCard) {
    if (isEmpty()) {
      m_top = t_newCard;
      m_bottom = t_newCard;
      t_newCard->m_next = nullptr;
      return;
    }

    t_newCard->m_next = m_top;
    m_top = t_newCard;
  }

  void insertBottom(Card *t_newCard) {
    if (isEmpty()) {
      m_top = t_newCard;
      m_bottom = t_newCard;
      t_newCard->m_next = nullptr;
      return;
    }

    m_bottom->m_next = t_newCard;
    m_bottom = t_newCard;
    m_bottom->m_next = nullptr;
  }

  void splice(Cards *t_newCards) {
    if (t_newCards->isEmpty()) {
      return;
    }

    if (isEmpty()) {
      m_top = t_newCards->m_top;
      m_bottom = t_newCards->m_bottom;
      return;
    }

    m_bottom->m_next = t_newCards->m_top;
    m_bottom = t_newCards->m_bottom;

    t_newCards->clear();
  }

  void clear() {
    m_top = nullptr;
    m_bottom = nullptr;
  }

  Card *pop() {
    if (m_top == nullptr)
      return nullptr;
    auto temp = m_top;
    m_top = m_top->m_next;
    if (m_top == nullptr)
      m_bottom = nullptr;
    return temp;
  }

  friend std::ostream &operator<<(std::ostream &t_ostream,
                                  const Cards &t_cards) {
    Card *current = t_cards.m_top;
    while (current != nullptr) {
      t_ostream << current->getSymbol();
      current = current->m_next;
    }
    return t_ostream;
  }
};

std::tuple<int, int> playGame() {
  std::vector<int> deck(52, 0); // 52 cards that will be dealt to the players
  for (int i = 0; i < 16; ++i)  // Add four of each picture card to the deck
                                // (replacing number cards)
    deck[i] = i / 4 + 1;

  std::random_device rd; // Random device to get a seed
  std::mt19937 g(rd());  // Mersenne Twister engine initialized with the seed
  std::shuffle(deck.begin(), deck.end(), g); // Shuffle the deck

  Cards players[] = {Cards(), Cards()};
  for (int i = 0; i < deck.size(); i += 2) {
    players[0].insertBottom(new Card(deck[i]));
    players[1].insertBottom(new Card(deck[i + 1]));
  }

  Cards pile = Cards();

  int cards = 0;
  int tricks = 0;

  int turn = 0;
  int penalty = 0;

  while (!players[0].isEmpty() && !players[1].isEmpty()) {
    ++cards;

    if (penalty > 0) {
      pile.insertBottom(players[turn].pop());

      if (pile.getBottom()->getValue() > 0) {
        turn = !turn;
        penalty = pile.getBottom()->getValue();
        tricks++;
        continue;
      }

      else if (--penalty == 0) {
        turn = !turn;
        players[turn].splice(&pile);
      }

      continue;
    }

    pile.insertBottom(players[turn].pop());
    penalty = pile.getBottom()->getValue();
    turn = !turn;
  }

  return {cards, tricks};
}

std::tuple<int, int> playGames(int t_games) {
  int max_cards = 0;
  int max_tricks = 0;

  for (int i = 0; i < t_games; ++i) {
    auto [cards, tricks] = playGame();
    max_cards = cards > max_cards ? cards : max_cards;
    max_tricks = tricks > max_tricks ? tricks : max_tricks;
  }

  return {max_cards, max_tricks};
}

int main() {
  int num_threads = 6;
  int games_per_thread = 100000;
  int max_cards = 0;
  int max_tricks = 0;

  while (true) {
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::future<std::tuple<int, int>>> futures;

    for (int i = 0; i < num_threads; ++i) {
      futures.push_back(
          std::async(std::launch::async, playGames, games_per_thread));
    }

    for (auto &f : futures) {
      auto [cards, tricks] = f.get();
      max_cards = cards > max_cards ? cards : max_cards;
      max_tricks = tricks > max_tricks ? tricks : max_tricks;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_s = end - start;
    std::chrono::duration<double, std::micro> elapsed_us = end - start;

    int games = num_threads * games_per_thread;
    int avg_us = elapsed_us.count() / games;
    std::cout << "Played " << games << " games in " << elapsed_s.count()
              << " seconds (Avg: " << avg_us << " us per game)." << std::endl
              << "Most cards so far: " << max_cards << std::endl
              << "Most tricks so far: " << max_tricks << std::endl;
  }

  return 0;
}