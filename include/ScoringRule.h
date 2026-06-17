#pragma once
#include "Hand.h"
#include "HandRank.h"
#include "checkers/PokerHandChecker.h"
#include "checkers/RoyalFlushChecker.h"
#include "checkers/StraightFlushChecker.h"
#include "checkers/FiveOfAKindChecker.h"
#include "checkers/FourOfAKindChecker.h"
#include "checkers/FlushHouseChecker.h"
#include "checkers/FullHouseChecker.h"
#include "checkers/FlushFiveChecker.h"
#include "checkers/FlushChecker.h"
#include "checkers/StraightChecker.h"
#include "checkers/ThreeOfAKindChecker.h"
#include "checkers/TwoPairChecker.h"
#include "checkers/PairChecker.h"
#include "checkers/HighCardChecker.h"

class ScoringRule
{
public:
    ScoringRule();
    int scoreHand(const Hand& hand);
    HandRank evaluateHand(const Hand& hand);

private:
    RoyalFlushChecker royalFlushChecker;
    StraightFlushChecker straightFlushChecker;
    FiveOfAKindChecker fiveOfAKindChecker;
    FourOfAKindChecker fourOfAKindChecker;
    FlushHouseChecker flushHouseChecker;
    FullHouseChecker fullHouseChecker;
    FlushFiveChecker flushFiveChecker;
    FlushChecker flushChecker;
    StraightChecker straightChecker;
    ThreeOfAKindChecker threeOfAKindChecker;
    TwoPairChecker twoPairChecker;
    PairChecker pairChecker;
    HighCardChecker highCardChecker;

    int convertRankToScore(HandRank rank);
};