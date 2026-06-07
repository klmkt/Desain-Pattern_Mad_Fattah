#pragma once
#include <memory>
#include <vector>
#include "PendingCommand.h"

class BlindState;

struct RunSessionState {
    int remainingPlays = 0;
    int freeRerolls = 0;
    int freePlayingCards = 0;
    int ante = 1;
    int currentScore = 0;
    std::vector<PendingCommand> pendingCommands;
    std::unique_ptr<BlindState> currentBlind;
};
