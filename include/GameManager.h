#pragma once
#include <memory>
#include "Hand.h"
#include "HandPlayer.h"
#include "ScoringRule.h"
#include "RunSessionService.h"
#include "DiscardService.h"
#include "RunSessionState.h"
#include "JokerManager.h" // Tambahkan header JokerManager

class GameManager
{
public:
    GameManager(
        std::unique_ptr<HandPlayer> player,
        std::unique_ptr<ScoringRule> rule,
        std::unique_ptr<RunSessionService> service,
        std::unique_ptr<DiscardService> discardSvc
    );
    GameManager();
    
    void runSession();

private:
    void setupJokers(); // Deklarasi fungsi inisialisasi Joker

    std::unique_ptr<HandPlayer> handPlayer;
    std::unique_ptr<ScoringRule> scoringRule;
    std::unique_ptr<RunSessionService> runSessionService;
    std::unique_ptr<DiscardService> discardService;
    
    RunSessionState sessionState;
    JokerManager jokerManager; // Objek pengelola Joker
};