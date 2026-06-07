#pragma once
#include <memory>
#include <vector>
#include "BlindState.h"
#include "PendingCommand.h"
#include "Deck.h" // Sediakan include ke Deck baru

struct RunSessionState {
    int freeRerolls = 0;
    int freePlayingCards = 0;
    int ante = 1;
    int currentScore = 0;
    
    // Tambahkan variabel pembatas giliran permainan sesuai brief Part 3
    int remainingPlays = 0;      
    int remainingDiscards = 0;   

    std::vector<PendingCommand> pendingCommands;
    std::unique_ptr<BlindState> currentBlind;
    
    // Tambahkan Deck persisten di sini
    Deck deck; 
};