#pragma once

#include <algorithm>
#include <map>
#include <vector>

struct Card {
    int rank = 0;
    char suit = ' ';
};

inline bool compareCard(const Card& a, const Card& b)
{
    if (a.rank != b.rank)
        return a.rank < b.rank;
    return a.suit < b.suit;
}

struct Hand
{
    int value = 0;
    std::vector<Card> cards;

    void sortByRank() {
        std::sort(cards.begin(), cards.end(), compareCard);
    }

    std::map<int, int> rankCounts() const {
        std::map<int, int> counts;
        for (const auto& card : cards) {
            counts[card.rank]++;
        }
        return counts;
    }

    std::map<char, int> suitCounts() const {
        std::map<char, int> counts;
        for (const auto& card : cards) {
            counts[card.suit]++;
        }
        return counts;
    }

    bool isFlush() const {
        if (cards.size() < 5)
            return false;
        return suitCounts().size() == 1;
    }

    bool isStraight() const {
        if (cards.size() < 5)
            return false;

        std::vector<int> ranks;
        ranks.reserve(cards.size());
        for (const auto& card : cards) {
            ranks.push_back(card.rank);
        }
        std::sort(ranks.begin(), ranks.end());
        ranks.erase(std::unique(ranks.begin(), ranks.end()), ranks.end());

        if (ranks.size() != cards.size())
            return false;

        bool regularStraight = true;
        for (size_t i = 1; i < ranks.size(); ++i) {
            if (ranks[i] != ranks[i - 1] + 1) {
                regularStraight = false;
                break;
            }
        }
        if (regularStraight)
            return true;

        // Ace-low straight: A, 2, 3, 4, 5
        if (ranks.size() == 5 && ranks[0] == 2 && ranks[1] == 3 && ranks[2] == 4 && ranks[3] == 5 && ranks[4] == 14)
            return true;

        return false;
    }

    bool isRoyal() const {
        if (!isStraight() || !isFlush())
            return false;

        int minRank = 100;
        int maxRank = 0;
        for (const auto& card : cards) {
            minRank = std::min(minRank, card.rank);
            maxRank = std::max(maxRank, card.rank);
        }
        return minRank == 10 && maxRank == 14;
    }

    bool isFullHouse() const {
        const auto counts = rankCounts();
        bool hasThree = false;
        bool hasTwo = false;
        if (counts.size() != 2)
            return false;

        for (const auto& kv : counts) {
            if (kv.second == 3)
                hasThree = true;
            else if (kv.second == 2)
                hasTwo = true;
        }
        return hasThree && hasTwo;
    }

    int highestRank() const {
        if (cards.empty())
            return 0;
        int high = cards.front().rank;
        for (const auto& card : cards) {
            if (card.rank > high)
                high = card.rank;
        }
        return high;
    }
};
