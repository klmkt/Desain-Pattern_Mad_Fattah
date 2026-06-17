#include <iostream>
#include "checkers/FlushFiveChecker.h"

HandRank FlushFiveChecker::check(const Hand& hand) {
    if (hand.cards.size() == 5 && hand.isFlush() && !hand.isStraight() && !hand.isFullHouse()) {
        std::cout << "Detected FLUSH FIVE\n";
        return HandRank::FLUSH_FIVE;
    }
    if (nextChecker)
        return nextChecker->check(hand);
    return HandRank::HIGH_CARD;
}
