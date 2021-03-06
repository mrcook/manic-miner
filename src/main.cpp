// Manic Miner C/C++ port Copyright (c) 2016-2021 Michael R. Cook
// Manic Miner Copyright (c) 1983 Bug-Byte Ltd.

#include "headers.h"
#include "globals.h"
#include "version.h"

static bool getOptions(int argc, char **argv, int &teleport, int &fps, int &lives, bool &cheat, int &zoom);

// The game has just loaded
int main(int argc, char *argv[]) {
    int fps = 50;
    bool cheat = false;
    int lives = 2;
    int teleport = -1;
    int zoom = 2;

    if (!getOptions(argc, argv, teleport, fps, lives, cheat, zoom)) {
        return 0;
    }

    if (!Speccy_initialize("Manic Miner: Retro!", fps, zoom)) {
        printf("Game initialization failure. Can not continue.");
        return -1;
    }

    Game_initialize(cheat, teleport);

    while (true) {
        Speccy_clearDisplayFile();
        Speccy_clearAttributesFile();
        Game_clearBuffers();

        Willy_reset(lives);

        // Initialise demo mode
        game.DEMO = 64;

        // Display the title screen and play the theme tune
        Game_play_intro();

        // Play the game!
        if (Game_play()) {
            break; // aka QUIT!
        }
    }

    Speccy_quit();

    printf("You helped Miner Willy acquire treasure worth %d.\n", game.highScore);
    if (game.highScore > 19837) {
        printf("He's on his way to joining the Jet Set!\n");
    }

    return 0;
}

bool getOptions(int argc, char **argv, int &teleport, int &fps, int &lives, bool &cheat, int &zoom) {
    int c;
    int cavernNumber;

    while ((c = getopt(argc, argv, "cl:s:t:z:h")) != -1) {
        switch (c) {
            case 'c':
                cheat = true;
                break;
            case 'l':
                lives = (int) strtol(optarg, (char **) nullptr, 10);
                if (lives > 2) { cheat = true; }
                break;
            case 's':
                fps = (int) strtol(optarg, (char **) nullptr, 10);
                break;
            case 't':
                cavernNumber = (int) strtol(optarg, (char **) nullptr, 10);
                if (cavernNumber >= 1 && cavernNumber <= 20) {
                    teleport = cavernNumber - 1;
                    cheat = true;
                }
                break;
            case 'z':
                zoom = (int) strtol(optarg, (char **) nullptr, 10);
                if (zoom < 1 || zoom > 4) {
                    zoom = 2;
                }
                break;
            case 'h':
                printf("Manic Miner %d.%d.%d\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
                printf("Copyright (c) 1983 Matthew Smith\n");
                printf("C/C++ port Copyright (c) 2016-2021 Michael R. Cook\n");
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
                printf("-z  NUMBER        Zoom is the window size: 1 - 4 (default: 2 - 512px x 384px)\n");
                printf("\n");
                return false;
            default:;
        }
    }

    if (cheat) {
        lives = 2;
    }

    return true;
}
