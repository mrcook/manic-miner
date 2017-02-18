// Manic Miner C Port Copyright 2017 Michael R. Cook
// Copyright 1983 Bug-Byte Ltd (Manic Miner)

// Manic Miner disassembly
// Copyright 2010, 2012-2016 Richard Dymond
// http://skoolkit.ca/

#include "headers.h"
#include "externs.h"

#include "terminal.h"

// The game has just loaded
int main(void) {
    Game_initialize();

    while (true) {
        // Initialise the game mode indicator at DEMO.
        game.DEMO = 64;

        // Display the title screen and play the theme tune
        Game_play_intro();

        // Initialise a new game, and play!
        if (Game_play()) {
            break; // aka QUIT!
        }
    }

    Terminal_exit();
    return 0;
}
