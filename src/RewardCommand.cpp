#include "RewardCommand.h"
#include "RunSessionState.h"

std::string BonusHandCommand::getName() const {
    return "Bonus Hand";
}

std::string BonusHandCommand::getDescription() const {
    return "Gain +1 hand next blind.";
}

void BonusHandCommand::execute(RunSessionState& state) {
    state.remainingPlays += 1;
}

std::string FreePlayingCard::getName() const {
    return "Free Playing Card";
}

std::string FreePlayingCard::getDescription() const {
    return "Gain 1 free playing card added to the deck.";
}

void FreePlayingCard::execute(RunSessionState& state) {
    state.freePlayingCards += 1;
}

std::string ExtraRerollCommand::getName() const {
    return "Extra Reroll";
}

std::string ExtraRerollCommand::getDescription() const {
    return "Gain +1 reroll for the next blind.";
}

void ExtraRerollCommand::execute(RunSessionState& state) {
    state.freeRerolls += 1;
}

std::string BonusMoneyCommand::getName() const {
    return "Bonus Money";
}

std::string BonusMoneyCommand::getDescription() const {
    return "Gain +2 money multiplier.";
}

void BonusMoneyCommand::execute(RunSessionState& state) {
    // This could be extended to add a money multiplier field
    // For now, we'll use it as a placeholder for future enhancement
    (void)state;
}
