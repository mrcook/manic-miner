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

static bool getOptions(int argc, char *argv[], int *teleport, int *fps, int *lives, bool *cheat);

// The game has just loaded
int main(int argc, char *argv[]) {
    int fps = 17;
    bool cheat = false;
    int lives = 2;
    int teleport = -1;

    if (!getOptions(argc, argv, &teleport, &fps, &lives, &cheat)) {
        return (0);
    }

    Speccy_initialize(fps);
    Game_initialize(lives, cheat, teleport);

    while (true) {
        // Initialise the game mode indicator at DEMO.
        game.DEMO = 64;

        // Display the title screen and play the theme tune
        Game_play_intro();

        game.DEMO = 64;//FIXME: testing

        // Initialise a new game, and play!
        if (Game_play()) {
            break; // aka QUIT!
        }
    }

    Terminal_exit();
    return 0;
}

bool getOptions(int argc, char *argv[], int *teleport, int *fps, int *lives, bool *cheat) {
    int c;
    int cavernNumber;

    while ((c = getopt(argc, argv, "cl:s:t:h")) != -1) {
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
            case 't':
                cavernNumber = (int) strtol(optarg, (char **) NULL, 10);
                if (cavernNumber >= 1 && cavernNumber <= 20) {
                    *teleport = cavernNumber - 1;
                    *cheat = true;
                }
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
                printf("-t  NUMBER        Teleport, and play only this cavern number (1-20, enables CHEAT mode)\n");
                printf("\n");
                return false;
            default:; // return true;
        }
    }

    if (*cheat) { *lives = 2; }

    return true;
}
