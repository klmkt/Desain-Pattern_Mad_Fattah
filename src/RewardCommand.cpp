#include "RewardCommand.h"
#include "RunSessionState.h"
#include <iostream>

// ==========================================
// Implementasi BonusHandCommand
// ==========================================
std::string BonusHandCommand::getName() const {
    return "Bonus Hand";
}

std::string BonusHandCommand::getDescription() const {
    return "Gain +1 hand play for the next blind";
}

void BonusHandCommand::execute(RunSessionState& state) {
    state.remainingPlays += 1;
    std::cout << "  [Command Effect] Gained +1 hand play for the next blind! Total plays: " 
              << state.remainingPlays << "\n";
}

// ==========================================
// Implementasi FreePlayingCardCommand
// ==========================================
std::string FreePlayingCardCommand::getName() const {
    return "Free Playing Card";
}

std::string FreePlayingCardCommand::getDescription() const {
    return "Gain a free playing card, added permanently to the deck";
}

void FreePlayingCardCommand::execute(RunSessionState& state) {
    state.deck.addCard("Ace of Spades (Bonus)"); 
    state.freePlayingCards++;
    
    std::cout << "  [Command Effect] Added a free playing card to the deck! Current deck size: " 
              << state.deck.getRemainingSize() << "\n";
}