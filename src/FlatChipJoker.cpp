#include "FlatChipJoker.h"
#include <iostream>

FlatChipJoker::FlatChipJoker(int chips) : bonusChips(chips) {}

void FlatChipJoker::applyEffect(ScoreContext& context) {
    context.baseChips += bonusChips;
    std::cout << "    [Joker Effect] FlatChipJoker triggered! Added +" << bonusChips << " Chips.\n";
}