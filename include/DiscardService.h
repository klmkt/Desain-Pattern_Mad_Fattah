#pragma once
#include "RunSessionState.h"
#include <vector>
#include <string>

class DiscardService {
public:
    // Mengeksekusi aksi discard, menghapus kartu dari tangan, dan menarik kartu baru dari Deck
    void executeDiscard(RunSessionState& state, std::vector<std::string>& currentHand, const std::vector<int>& indicesToDiscard);
};