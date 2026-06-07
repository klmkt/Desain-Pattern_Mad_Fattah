#include "PairJoker.h"
#include <iostream>

PairJoker::PairJoker(int mult) : multiplierBonus(mult) {}

void PairJoker::applyEffect(ScoreContext& context) {
    // Mengecek apakah tipe tangan yang dikirim dari GameManager adalah "Pair"
    if (context.handType == "Pair") {
        context.multiplier *= multiplierBonus;
        std::cout << "    [Joker Effect] PairJoker triggered! Multiplier is multiplied by x" << multiplierBonus << ".\n";
    }
}