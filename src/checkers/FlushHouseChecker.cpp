#include <iostream>
#include "checkers/FlushHouseChecker.h"

HandRank FlushHouseChecker::check(const Hand& hand) {
    if (hand.cards.size() == 5 && hand.isFlush() && hand.isFullHouse()) {
        std::cout << "Detected FLUSH HOUSE\n";
        return HandRank::FLUSH_HOUSE;
    }
    if (nextChecker)
        return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
