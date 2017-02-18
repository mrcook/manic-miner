// Manic Miner C Port Copyright 2017 Michael R. Cook

#include "externs.h"

void Game_initialize() {
    initialize_curses();
    Willy_initialize();

    // FIXME: no joystick support just yet! -MRC-
    // Now check whether there is a joystick connected.
    game.KEMP = false;
}

void Game_new() {
    // Initialise the in-game music note index at NOTEINDEX
    game.NOTEINDEX = 0;

    // Initialise the screen flash counter at FLASH
    game.FLASH = 0;

    // Initialise in game music state
    game.playMusic = true;

    // Initialise the score at SCORE
    game.current_score = 0;

    // Prepare the screen; clear the entire Spectrum display file
    for (int i = 0; i < 6144; i++) {
        MEM[16384 + i] = 0;
    }
}

void Game_play_intro() {
    // Clear the entire Spectrum display file
    for (int i = 0; i < 6144; i++) {
        MEM[16384 + i] = 0;
    }

    // Copy TITLESCR1 and TITLESCR2 to the top two-thirds of the display file
    for (int i = 0; i < 2048; i++) {
        MEM[16384 + i] = TITLESCR1[i];
    }
    for (int i = 0; i < 2048; i++) {
        MEM[16384 + 2048 + i] = TITLESCR2[i];
    }

    redraw_screen();

    // Draw Willy at 18493 (9,29)
    DRWFIX(&willy.sprites[64], 18493, 0);

    // Copy the attribute bytes from CAVERN19 to the top third of the attribute file
    uint16_t addr = 22528;
    for (int i = 0; i < 256; i++) {
        MEM[addr + i] = CAVERN19[i];
    }

    // Copy LOWERATTRS to the bottom two-thirds of the attribute file
    addr += 256;
    for (int i = 0; i < 512; i++) {
        MEM[addr + i] = LOWERATTRS[i];
    }

    redraw_screen();

    // And finally, play the theme tune and check for key presses.

    // Play the theme tune -- start game if ENTER/FIRE button was pressed
    // FIXME: commented out -- no need to run the tune, just yet :)
    // if (PLAYTUNE()) {
    //   game.DEMO = 0;
    //   return;
    // }

    // Scroll intro message across the screen
    char *introMessage = ".  .  .  .  .  .  .  .  .  .  . MANIC MINER . . "
            "© BUG-BYTE ltd. 1983 . . By Matthew Smith . . . "
            "Q to P = Left & Right . . Bottom row = Jump . . "
            "A to G = Pause . . H to L = Tune On/Off . . . "
            "Guide Miner Willy through 20 lethal caverns"
            " .  .  .  .  .  .  .  .";
    for (int pos = 0; game.DEMO > 0 && pos < 224; pos++) {
        // Print 32 characters of the message at 20576 (19,0)
        PMESS(&introMessage[pos], 20576, 32);

        // Keep only bits 1 and 2, and move them into bits 6 and 7,
        // so that A holds 0, 64, 128 or 192;
        // this value determines the animation frame to use for Willy
        uint8_t sprite_id = rotr((uint8_t) (pos & 6), 3);

        // Draw Willy at 18493 (9,29)
        DRWFIX(&willy.sprites[sprite_id], 18493, 0);

        redraw_screen();

        // Pause for about 0.1s
        millisleep(72);

        // Is ENTER being pressed? If so, start the game
        if (check_enter_keypress()) {
            game.DEMO = 0;
            break;
        }
    }
}