#include "DiscardService.h"
#include <iostream>
#include <algorithm>

void DiscardService::executeDiscard(RunSessionState& state, std::vector<std::string>& currentHand, const std::vector<int>& indicesToDiscard) {
    if (state.remainingDiscards <= 0) {
        std::cout << "No discards remaining!\n";
        return;
    }

    // Urutkan index dari yang terbesar ke terkecil agar saat dihapus, index di depannya tidak bergeser
    std::vector<int> sortedIndices = indicesToDiscard;
    std::sort(sortedIndices.rbegin(), sortedIndices.rend());

    for (int idx : sortedIndices) {
        if (idx >= 0 && idx < currentHand.size()) {
            currentHand.erase(currentHand.begin() + idx);
        }
    }

    // Refill (Tarik kartu baru dari deck untuk melengkapi tangan kembali menjadi 8 kartu)
    int cardsNeeded = 8 - currentHand.size();
    std::vector<std::string> newCards = state.deck.drawCards(cardsNeeded);
    currentHand.insert(currentHand.end(), newCards.begin(), newCards.end());

    state.remainingDiscards--; // Kurangi kuota discard
    std::cout << "  [Discard Service] Successfully discarded and redrew cards.\n";
}