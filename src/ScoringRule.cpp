#include <iostream>
#include "../include/ScoringRule.h" // Gunakan relative path jika diperlukan compiler

ScoringRule::ScoringRule() {
    // Build the chain dari susunan (rank) BALATRO TERTINGGI hingga TERENDAH
    flushFiveChecker.setNext(&flushHouseChecker);
    flushHouseChecker.setNext(&fiveOfAKindChecker);
    fiveOfAKindChecker.setNext(&royalFlushChecker);
    royalFlushChecker.setNext(&straightFlushChecker);
    straightFlushChecker.setNext(&fourOfAKindChecker);
    fourOfAKindChecker.setNext(&fullHouseChecker);
    fullHouseChecker.setNext(&flushChecker);
    flushChecker.setNext(&straightChecker);
    straightChecker.setNext(&threeOfAKindChecker);
    threeOfAKindChecker.setNext(&twoPairChecker);
    twoPairChecker.setNext(&pairChecker);
    pairChecker.setNext(&highCardChecker);
}

int ScoringRule::scoreHand(const Hand& hand) {
    std::cout << "Calculating hand score...\n";
    HandRank rank = flushFiveChecker.check(hand); 
    int score = convertRankToScore(rank);
    std::cout << "Final score = " << score << "\n";
    return score;
}

HandRank ScoringRule::evaluateHand(const Hand& hand) {
    return flushFiveChecker.check(hand);
}

int ScoringRule::convertRankToScore(HandRank rank) {
    switch (rank) {
    case HandRank::FLUSH_FIVE:
        return 160; 
    case HandRank::FLUSH_HOUSE:
        return 140;
    case HandRank::FIVE_OF_A_KIND:
        return 120;
    case HandRank::ROYAL_FLUSH:
        return 100;
    case HandRank::STRAIGHT_FLUSH:
        return 90;
    case HandRank::FOUR_OF_A_KIND:
        return 60;
    case HandRank::FULL_HOUSE:
        return 40;
    case HandRank::FLUSH:
        return 35;
    case HandRank::STRAIGHT:
        return 30;
    case HandRank::THREE_OF_A_KIND:
        return 20;
    case HandRank::TWO_PAIR:
        return 15;
    case HandRank::PAIR:
        return 10;
    case HandRank::HIGH_CARD:
    default:
        return 5;
    }
}