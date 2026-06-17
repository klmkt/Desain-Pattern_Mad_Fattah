#include <iostream>
#include "checkers/FlushChecker.h"

HandRank FlushChecker::check(const Hand& hand) {
    if (hand.cards.size() == 5 && hand.isFlush() && !hand.isStraight() && !hand.isFullHouse()) {
        std::cout << "Detected FLUSH\n";
        return HandRank::FLUSH;
    }
    if (nextChecker)
        return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
