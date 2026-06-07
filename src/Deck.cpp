#include "Deck.h"
#include <chrono>

Deck::Deck() {
    initializeStandardDeck();
}

void Deck::initializeStandardDeck() {
    cards.clear();
    std::vector<std::string> suits = {"Hearts", "Diamonds", "Clubs", "Spades"};
    std::vector<std::string> ranks = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};
    
    for (const auto& suit : suits) {
        for (const auto& rank : ranks) {
            cards.push_back(rank + " of " + suit);
        }
    }
}

void Deck::shuffleDeck() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cards.begin(), cards.end(), std::default_random_engine(seed));
}

void Deck::addCard(const std::string& card) {
    cards.push_back(card);
}

std::vector<std::string> Deck::drawCards(int amount) {
    std::vector<std::string> drawn;
    for (int i = 0; i < amount && !cards.empty(); ++i) {
        drawn.push_back(cards.back());
        cards.pop_back();
    }
    return drawn;
}

int Deck::getRemainingSize() const {
    return cards.size();
}