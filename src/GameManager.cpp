#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include "GameManager.h"

// Sertakan file Joker dan Context (Pastikan file ini sudah kamu buat sebelumnya)
#include "FlatChipJoker.h"
#include "PairJoker.h"
#include "ScoreContext.h"
#include "HandRank.h"

namespace {

enum class PlayerAction {
    Play,
    Skip
};

int parseRank(const std::string& rankStr) {
    if (rankStr == "Jack") return 11;
    if (rankStr == "Queen") return 12;
    if (rankStr == "King") return 13;
    if (rankStr == "Ace") return 14;
    try {
        return std::stoi(rankStr);
    } catch (...) {
        return 0;
    }
}

char parseSuit(const std::string& suitStr) {
    if (suitStr == "Hearts") return 'H';
    if (suitStr == "Diamonds") return 'D';
    if (suitStr == "Clubs") return 'C';
    if (suitStr == "Spades") return 'S';
    return ' ';
}

Card parseCardString(const std::string& cardStr) {
    std::istringstream stream(cardStr);
    std::string rankPart;
    std::string ofWord;
    std::string suitPart;
    stream >> rankPart >> ofWord >> suitPart;
    return Card{parseRank(rankPart), parseSuit(suitPart)};
}

std::string handRankToString(HandRank rank) {
    switch (rank) {
        case HandRank::HIGH_CARD: return "HighCard";
        case HandRank::PAIR: return "Pair";
        case HandRank::TWO_PAIR: return "TwoPair";
        case HandRank::THREE_OF_A_KIND: return "ThreeOfAKind";
        case HandRank::STRAIGHT: return "Straight";
        case HandRank::FLUSH: return "Flush";
        case HandRank::FULL_HOUSE: return "FullHouse";
        case HandRank::FOUR_OF_A_KIND: return "FourOfAKind";
        case HandRank::STRAIGHT_FLUSH: return "StraightFlush";
        case HandRank::ROYAL_FLUSH: return "RoyalFlush";
        case HandRank::FIVE_OF_A_KIND: return "FiveOfAKind";
        case HandRank::FLUSH_HOUSE: return "FlushHouse";
        case HandRank::FLUSH_FIVE: return "FlushFive";
        default: return "Unknown";
    }
}

PlayerAction promptPlayerAction(const BlindState& blind, bool allowSkip) {
    while (true) {
        std::cout << "Current blind: " << blind.getName() << "\n";
        std::cout << "Choose action [P]LAY";
        if (allowSkip) {
            std::cout << " or [S]KIP";
        }
        std::cout << ": ";

        std::string input;
        if (!(std::cin >> input)) {
            return PlayerAction::Play;
        }

        if (input == "P" || input == "p" || input == "PLAY" || input == "play") {
            return PlayerAction::Play;
        }

        if (allowSkip && (input == "S" || input == "s" || input == "SKIP" || input == "skip")) {
            return PlayerAction::Skip;
        }

        if (!allowSkip && (input == "S" || input == "s" || input == "SKIP" || input == "skip")) {
            std::cout << "Boss Blind tidak bisa di-skip. Anda harus PLAY.\n";
            continue;
        }

        std::cout << "Invalid choice. Please enter PLAY";
        if (allowSkip) {
            std::cout << " or SKIP";
        }
        std::cout << ".\n";
    }
}

} // akhir namespace anonim

GameManager::GameManager(
    std::unique_ptr<HandPlayer> player,
    std::unique_ptr<ScoringRule> rule,
    std::unique_ptr<RunSessionService> service,
    std::unique_ptr<DiscardService> discardSvc
)
    : handPlayer(std::move(player)),
      scoringRule(std::move(rule)),
      runSessionService(std::move(service)),
      discardService(std::move(discardSvc)) {}

GameManager::GameManager()
    : handPlayer(std::make_unique<HandPlayer>()),
      scoringRule(std::make_unique<ScoringRule>()),
      runSessionService(std::make_unique<RunSessionService>()),
      discardService(std::make_unique<DiscardService>()) {}

// ==========================================
// Inisialisasi Joker Aktif
// ==========================================
void GameManager::setupJokers() {
    // Mendaftarkan joker-joker yang aktif untuk run ini
    jokerManager.addJoker(std::make_unique<FlatChipJoker>(50));
    jokerManager.addJoker(std::make_unique<PairJoker>(2));
    
    std::cout << "  [System] Jokers initialized and registered.\n";
}

void GameManager::runSession() {
    std::cout << "=== Run Started ===\n";

    // 1. Setup Awal
    if (!sessionState.currentBlind) {
        sessionState.currentBlind = std::make_unique<SmallBlindState>();
    }
    
    sessionState.deck.initializeStandardDeck();
    sessionState.deck.shuffleDeck();
    
    // Panggil registrasi Joker
    setupJokers();

    // 2. Loop Progresi Blind
    for (int blindIndex = 0; blindIndex < 3; ++blindIndex) {
        sessionState.remainingPlays = 3;  
        sessionState.remainingDiscards = 3;
        sessionState.currentScore = 0; 

        runSessionService->executePendingCommands(
            sessionState,
            sessionState.currentBlind->getCommandTiming()
        );

        const PlayerAction action = promptPlayerAction(
            *sessionState.currentBlind,
            sessionState.currentBlind->canSkip()
        );

        if (action == PlayerAction::Skip) {
            runSessionService->skipBlind(sessionState);
            runSessionService->executePendingCommands(
                sessionState,
                sessionState.currentBlind->getCommandTiming()
            );
            std::cout << "Blind skipped.\n\n";
            continue;
        }

        std::vector<std::string> currentHandStr = sessionState.deck.drawCards(8);
        int targetScore = sessionState.currentBlind->getTargetScore(sessionState.ante);
        bool blindCleared = false;

        // 3. Interactive Gameplay Loop (Play / Discard)
        while (sessionState.remainingPlays > 0) {
            std::cout << "\n====================================\n";
            std::cout << "Target Score: " << targetScore << " | Current Score: " << sessionState.currentScore << "\n";
            std::cout << "Plays left: " << sessionState.remainingPlays 
                      << " | Discards left: " << sessionState.remainingDiscards << "\n";
            std::cout << "--- Your Hand ---\n";
            for (size_t i = 0; i < currentHandStr.size(); ++i) {
                std::cout << "[" << i + 1 << "] " << currentHandStr[i] << "\n";
            }
            
            std::cout << "\nChoose action:\n [P]lay selected cards\n [D]iscard selected cards\nChoice: ";
            std::string subAction;
            std::cin >> subAction;

            if (subAction == "D" || subAction == "d") {
                if (sessionState.remainingDiscards <= 0) {
                    std::cout << "Not enough discards remaining!\n";
                    continue;
                }
                std::cout << "Enter card numbers to DISCARD (separated by space, end with 0, e.g., '1 3 4 0'): ";
                std::vector<int> indices;
                int idx;
                while (std::cin >> idx && idx != 0) {
                    indices.push_back(idx - 1); 
                }
                discardService->executeDiscard(sessionState, currentHandStr, indices);
            } 
            else if (subAction == "P" || subAction == "p") {
                std::cout << "Enter card numbers to PLAY (at least 1 card, separated by space, end with 0, e.g., '1 2 3 0'): ";
                std::vector<int> indices;
                int idx;
                while (std::cin >> idx && idx != 0) {
                    indices.push_back(idx - 1);
                }
                if (indices.empty()) {
                    std::cout << "Please select at least one card to play.\n";
                    continue;
                }
                
                std::vector<std::string> chosenCardsStr;
                std::vector<int> sortedIndices = indices;
                std::sort(sortedIndices.rbegin(), sortedIndices.rend());
                
                for (int index : sortedIndices) {
                    if (index >= 0 && index < currentHandStr.size()) {
                        chosenCardsStr.push_back(currentHandStr[index]);
                        currentHandStr.erase(currentHandStr.begin() + index);
                    }
                }

                Hand playedHand;
                for (const auto& cardStr : chosenCardsStr) {
                    playedHand.cards.push_back(parseCardString(cardStr));
                }
                playedHand.sortByRank();

                // ==========================================
                // Evaluasi Skor & Efek Joker
                // ==========================================
                ScoreContext context;
                
                HandRank rank = scoringRule->evaluateHand(playedHand);
                context.handType = handRankToString(rank);
                context.baseChips = scoringRule->scoreHand(playedHand); // Hasil evaluasi awal tangan
                context.multiplier = 1; // Default multiplier awal
                
                std::cout << "    [Hand Type] " << context.handType << "\n";
                
                // Memicu Joker agar mengubah 'context' (menambah chip/multiplier)
                jokerManager.applyAllEffects(context);
                
                // Kalkulasi final ala Balatro (Chips x Multiplier)
                int scoreGained = context.baseChips * context.multiplier;
                
                sessionState.currentScore += scoreGained;
                sessionState.remainingPlays--;

                std::cout << "\n  [Played Cards Scored: " << scoreGained 
                          << " (Chips: " << context.baseChips << " x Mult: " << context.multiplier << ")]\n";

                // Cek target kemenangan
                if (sessionState.currentScore >= targetScore) {
                    blindCleared = true;
                    break;
                }
                
                // Tarik kartu baru ke tangan (Refill)
                int cardsNeeded = 8 - currentHandStr.size();
                std::vector<std::string> newCards = sessionState.deck.drawCards(cardsNeeded);
                currentHandStr.insert(currentHandStr.end(), newCards.begin(), newCards.end());
            }
        }

        // Resolusi akhir permainan
        if (blindCleared) {
            int reward = sessionState.currentBlind->getRewardMoney();
            std::cout << "\n>>> Target Reached! Money gained: " << reward << " <<<\n\n";
            sessionState.currentBlind = sessionState.currentBlind->nextState(sessionState.ante);
        } else {
            std::cout << "\n>>> Run failed to reach target score. Game Over. <<<\n";
            break; 
        }
    }

    // Eksekusi sisa command
    runSessionService->executePendingCommands(sessionState, CommandTiming::Start);
    std::cout << "=== Run Ended ===\n";
}