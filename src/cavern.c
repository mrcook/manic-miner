// Manic Miner C Port Copyright 2017 Michael R. Cook

#include "externs.h"

// FIXME: cavern data will need to be initialized from the game data

// Initialize the cavern
void Cavern_initialize() {
    // Initialise the current cavern number at SHEET
    cavern.SHEET = 0; // FIXME: defaults to CAVERN0

    // Load all the cavern data
    initialize_cavern0();
}

bool Cavern_isAirDepleted() {
    // For some reason 36 means no air left
    return cavern.AIR == 36;
}
