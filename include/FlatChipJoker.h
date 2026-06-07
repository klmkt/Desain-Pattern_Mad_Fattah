#pragma once
#include "Joker.h"

class FlatChipJoker : public Joker {
private:
    int bonusChips;

public:
    FlatChipJoker(int chips);
    void applyEffect(ScoreContext& context) override;
};