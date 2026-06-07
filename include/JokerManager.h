#pragma once
#include <vector>
#include <memory>
#include "Joker.h"
#include "ScoreContext.h"

class JokerManager {
private:
    std::vector<std::unique_ptr<Joker>> activeJokers;

public:
    void addJoker(std::unique_ptr<Joker> joker);
    void applyAllEffects(ScoreContext& context);
};