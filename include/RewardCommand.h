#pragma once
#include <string>

class RunSessionState;

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

class FreePlayingCard : public RewardCommand {
public:
    std::string getName() const override;
    std::string getDescription() const override;
    void execute(RunSessionState& state) override;
};

class ExtraRerollCommand : public RewardCommand {
public:
    std::string getName() const override;
    std::string getDescription() const override;
    void execute(RunSessionState& state) override;
};

class BonusMoneyCommand : public RewardCommand {
public:
    std::string getName() const override;
    std::string getDescription() const override;
    void execute(RunSessionState& state) override;
};
