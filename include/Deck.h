#pragma once
#include <vector>
#include <algorithm>
#include <random>
#include "Hand.h" // Asumsi Hand berisi kumpulan Card atau representasi kartu Anda

class Deck {
private:
    // Ganti std::string dengan class Card Anda jika sudah ada
    std::vector<std::string> cards; 

public:
    Deck();
    void initializeStandardDeck();
    void shuffleDeck();
    void addCard(const std::string& card);
    
    // Method untuk menarik sejumlah kartu ke tangan pemain
    std::vector<std::string> drawCards(int amount);
    int getRemainingSize() const;
};