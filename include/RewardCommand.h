#pragma once
#include <string>

struct RunSessionState; // Forward declaration

class RewardCommand {
public:
    virtual ~RewardCommand() = default;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual void execute(RunSessionState& state) = 0;
};

class BonusHandCommand : public RewardCommand {
public:
    std::string getName() const override;
    std::string getDescription() const override;
    void execute(RunSessionState& state) override;
};

class FreePlayingCardCommand : public RewardCommand {
public:
    std::string getName() const override;
    std::string getDescription() const override;
    void execute(RunSessionState& state) override;
};