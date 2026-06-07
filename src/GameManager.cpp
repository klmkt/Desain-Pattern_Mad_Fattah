#include <iostream>
#include <memory>
#include <string>
#include "GameManager.h"

namespace {

enum class PlayerAction {
    Play,
    Skip
};

PlayerAction promptPlayerAction(const BlindState& blind) {
    while (true) {
        std::cout << "Current blind: " << blind.getName() << "\n";
        std::cout << "Choose action [P]LAY or [S]KIP: ";

        std::string input;
        if (!(std::cin >> input)) {
            return PlayerAction::Play;
        }

        if (input == "P" || input == "p" || input == "PLAY" || input == "play") {
            return PlayerAction::Play;
        }

        if (input == "S" || input == "s" || input == "SKIP" || input == "skip") {
            return PlayerAction::Skip;
        }

        std::cout << "Invalid choice. Please enter PLAY or SKIP.\n";
    }
}

}

GameManager::GameManager(
    std::unique_ptr<HandGenerator> generator,
    std::unique_ptr<HandPlayer> player,
    std::unique_ptr<ScoringRule> rule,
    std::unique_ptr<RunSessionService> service
)
    : handGenerator(std::move(generator)),
      handPlayer(std::move(player)),
      scoringRule(std::move(rule)),
      runSessionService(std::move(service)) {}

GameManager::GameManager()
    : handGenerator(std::make_unique<HandGenerator>()),
      handPlayer(std::make_unique<HandPlayer>()),
      scoringRule(std::make_unique<ScoringRule>()),
      runSessionService(std::make_unique<RunSessionService>()) {}

void GameManager::runSession() {
    std::cout << "=== Run Started ===\n";

    if (!sessionState.currentBlind) {
        sessionState.currentBlind = std::make_unique<SmallBlindState>();
    }

    for (int blindIndex = 0; blindIndex < 3; ++blindIndex) {
        runSessionService->executePendingCommands(
            sessionState,
            sessionState.currentBlind->getCommandTiming()
        );

        const PlayerAction action = promptPlayerAction(*sessionState.currentBlind);

        if (action == PlayerAction::Skip) {
            runSessionService->skipBlind(sessionState);
            runSessionService->executePendingCommands(
                sessionState,
                sessionState.currentBlind->getCommandTiming()
            );
            std::cout << "Blind skipped.\n";
            continue;
        }

        Hand hand = handGenerator->generateHand();
        ChosenHand chosenHand = handPlayer->playHand(hand);
        const Hand& scoredHand = chosenHand.toHand();
        int score = scoringRule->scoreHand(scoredHand);
        sessionState.currentScore = score;

        int targetScore = sessionState.currentBlind->getTargetScore(sessionState.ante);
        bool win = score >= targetScore;

        if (win) {
            int reward = sessionState.currentBlind->getRewardMoney();
            std::cout << "Money gained: " << reward << "\n";
        } else {
            std::cout << "Run failed to reach target score.\n";
        }

        sessionState.currentBlind = sessionState.currentBlind->nextState(sessionState.ante);
        runSessionService->executePendingCommands(
            sessionState,
            sessionState.currentBlind->getCommandTiming()
        );
    }

    std::cout << "=== Run Ended ===\n";
}
