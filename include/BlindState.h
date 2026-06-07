#pragma once
#include <memory>
#include <string>
#include "PendingCommand.h"

class BlindState {
public:
    virtual ~BlindState() = default;
    virtual std::string getName() const = 0;
    virtual int getTargetScore(int ante) const = 0;
    virtual int getRewardMoney() const = 0;
    virtual PendingCommand createSkipRewardCommand() const = 0;
    virtual std::unique_ptr<BlindState> nextState(int& ante) const = 0;
    virtual CommandTiming getCommandTiming() const = 0;
};

class SmallBlindState : public BlindState {
public:
    std::string getName() const override;
    int getTargetScore(int ante) const override;
    int getRewardMoney() const override;
    PendingCommand createSkipRewardCommand() const override;
    std::unique_ptr<BlindState> nextState(int& ante) const override;
    CommandTiming getCommandTiming() const override;
};

class BigBlindState : public BlindState {
public:
    std::string getName() const override;
    int getTargetScore(int ante) const override;
    int getRewardMoney() const override;
    PendingCommand createSkipRewardCommand() const override;
    std::unique_ptr<BlindState> nextState(int& ante) const override;
    CommandTiming getCommandTiming() const override;
};

class BossBlindState : public BlindState {
public:
    std::string getName() const override;
    int getTargetScore(int ante) const override;
    int getRewardMoney() const override;
    PendingCommand createSkipRewardCommand() const override;
    std::unique_ptr<BlindState> nextState(int& ante) const override;
    CommandTiming getCommandTiming() const override;
};
