#include <iostream>
#include "checkers/RoyalFlushChecker.h"

HandRank RoyalFlushChecker::check(const Hand& hand) {
    if (hand.cards.size() == 5 && hand.isRoyal()) {
        std::cout << "Detected ROYAL FLUSH\n";
        return HandRank::ROYAL_FLUSH;
    }
    if (nextChecker)
        return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
