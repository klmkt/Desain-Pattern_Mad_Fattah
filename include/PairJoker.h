#pragma once
#include "Joker.h"

class PairJoker : public Joker {
private:
    int multiplierBonus;

public:
    PairJoker(int mult);
    void applyEffect(ScoreContext& context) override;
};