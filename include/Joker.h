#pragma once
#include "ScoreContext.h"

class Joker {
public:
    virtual ~Joker() = default;
    
    // Fungsi murni virtual yang harus diisi oleh Joker spesifik
    virtual void applyEffect(ScoreContext& context) = 0; 
};