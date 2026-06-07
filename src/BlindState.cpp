#include "BlindState.h"
#include "RewardCommand.h"
#include "PendingCommand.h"

// =====================================
// SmallBlindState
// =====================================
int SmallBlindState::getTargetScore(int ante) const {
    return 300 * ante;
}

int SmallBlindState::getRewardMoney() const {
    return 3;
}

PendingCommand SmallBlindState::createSkipRewardCommand() const {
    // Urutan yang benar: Timing, Executed (bool), Command (Pointer)
    return PendingCommand{
        CommandTiming::NextAnte,
        false,
        std::make_unique<FreePlayingCardCommand>()
    };
}

std::unique_ptr<BlindState> SmallBlindState::nextState(int& ante) const {
    return std::make_unique<BigBlindState>();
}

CommandTiming SmallBlindState::getCommandTiming() const {
    return CommandTiming::NextAnte;
}

std::string SmallBlindState::getName() const {
    return "Small Blind";
}

// =====================================
// BigBlindState
// =====================================
int BigBlindState::getTargetScore(int ante) const {
    return 450 * ante;
}

int BigBlindState::getRewardMoney() const {
    return 4;
}

PendingCommand BigBlindState::createSkipRewardCommand() const {
    return PendingCommand{
        CommandTiming::Start,
        false,
        std::make_unique<BonusHandCommand>()
    };
}

std::unique_ptr<BlindState> BigBlindState::nextState(int& ante) const {
    return std::make_unique<BossBlindState>();
}

CommandTiming BigBlindState::getCommandTiming() const {
    return CommandTiming::Start;
}

std::string BigBlindState::getName() const {
    return "Big Blind";
}

// =====================================
// BossBlindState
// =====================================
int BossBlindState::getTargetScore(int ante) const {
    return 600 * ante;
}

int BossBlindState::getRewardMoney() const {
    return 5;
}

PendingCommand BossBlindState::createSkipRewardCommand() const {
    return PendingCommand{
        CommandTiming::NextBlind,
        false,
        std::make_unique<FreePlayingCardCommand>()
    };
}

std::unique_ptr<BlindState> BossBlindState::nextState(int& ante) const {
    ante += 1;
    return std::make_unique<SmallBlindState>();
}

CommandTiming BossBlindState::getCommandTiming() const {
    return CommandTiming::NextBlind;
}

std::string BossBlindState::getName() const {
    return "Boss Blind";
}