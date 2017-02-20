// Manic Miner C Port Copyright 2017 Michael R. Cook
// Copyright 1983 Bug-Byte Ltd (Manic Miner)

// Manic Miner disassembly
// Copyright 2010, 2012-2016 Richard Dymond
// http://skoolkit.ca/

#include "headers.h"
#include "externs.h"

#include "terminal.h"

// Current game version
static const char *version = "0.0.1";

static bool getOptions(int argc, char *argv[], int *fps, int *lives, bool *cheat);

// The game has just loaded
int main(int argc, char *argv[]) {
    int fps, lives;
    bool cheat;

    if (!getOptions(argc, argv, &fps, &lives, &cheat)) {
        return (0);
    }

    Speccy_initialize(fps);
    Game_initialize(lives, cheat);

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

bool getOptions(int argc, char *argv[], int *fps, int *lives, bool *cheat) {
    int c;

    while ((c = getopt(argc, argv, "cl:s:h")) != -1) {
        switch (c) {
            case 'c':
                *cheat = true;
                break;
            case 'l':
                *lives = (int) strtol(optarg, (char **) NULL, 10);
                if (*lives > 2) { *cheat = true; }
                break;
            case 's':
                *fps = (int) strtol(optarg, (char **) NULL, 10);
                break;
            case 'h':
                printf("Manic Miner %s\n", version);
                printf("Copyright © 1983 Matthew Smith\n");
                printf("Z80 to C port Copyright © 2017 Michael R. Cook\n");
                printf("\n");
                printf("USAGE:\n");
                printf("  manic_miner [FLAGS] [OPTIONS]\n");
                printf("\n");
                printf("FLAGS:\n");
                printf("-c                Enable cheat mode (infinite lives)\n");
                printf("\n");
                printf("OPTIONS:\n");
                printf("-l  NUMBER        Set number of lives (max: 2, default: 2)\n");
                printf("-s  NUMBER        Set game speed / frames-per-second (default: 17)\n");
                printf("\n");
                return false;
            default:; // return true;
        }
    }

    if (*cheat) { *lives = 2; }

    return true;
}
