#include "RunSessionService.h"
#include <iostream>

void RunSessionService::skipBlind(RunSessionState& state) {
    if (!state.currentBlind) {
        return;
    }

    PendingCommand command = state.currentBlind->createSkipRewardCommand();
    std::cout << "  [Reward Command Created] " << command.command->getName() << ": "
              << command.command->getDescription() << "\n";
    state.pendingCommands.push_back(std::move(command));
    state.currentBlind = state.currentBlind->nextState(state.ante);
    resetBlindResources(state);
}

void RunSessionService::executePendingCommands(RunSessionState& state, CommandTiming timing) {
    for (auto& pending : state.pendingCommands) {
        if (!pending.executed && pending.timing == timing && pending.command) {
            std::string timingStr;
            if (timing == CommandTiming::Start) timingStr = "START";
            else if (timing == CommandTiming::NextBlind) timingStr = "NEXT_BLIND";
            else if (timing == CommandTiming::NextAnte) timingStr = "NEXT_ANTE";
            
            std::cout << "  [Executing Command] " << pending.command->getName() 
                      << " (timing: " << timingStr << ")\n";
            pending.command->execute(state);
            pending.executed = true;
            
            std::cout << "    Game state updated: remainingPlays=" << state.remainingPlays
                      << ", freeRerolls=" << state.freeRerolls
                      << ", freePlayingCards=" << state.freePlayingCards << "\n";
        }
    }
}

void RunSessionService::resetBlindResources(RunSessionState& state) {
    (void)state;
}
