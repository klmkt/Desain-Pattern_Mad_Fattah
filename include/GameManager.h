#pragma once
#include <memory>
#include "Hand.h"
#include "HandGenerator.h"
#include "HandPlayer.h"
#include "ScoringRule.h"
#include "RunSessionService.h"

class GameManager
{
public:
    GameManager(
        std::unique_ptr<HandGenerator> generator,
        std::unique_ptr<HandPlayer> player,
        std::unique_ptr<ScoringRule> rule,
        std::unique_ptr<RunSessionService> service
    );
    GameManager();
    void runSession();

private:
    std::unique_ptr<HandGenerator> handGenerator;
    std::unique_ptr<HandPlayer> handPlayer;
    std::unique_ptr<ScoringRule> scoringRule;
    std::unique_ptr<RunSessionService> runSessionService;
    RunSessionState sessionState;
};