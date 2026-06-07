#include "JokerManager.h"

void JokerManager::addJoker(std::unique_ptr<Joker> joker) {
    activeJokers.push_back(std::move(joker));
}

void JokerManager::applyAllEffects(ScoreContext& context) {
    for (auto& joker : activeJokers) {
        joker->applyEffect(context);
    }
}