// Manic Miner C Port Copyright 2017 Michael R. Cook

#include "headers.h"
#include "externs.h"

#include "terminal.h"

// private functions

// Draws the AIR bar graphics to the screen
void drawAirBar(void);

// Draws the remaining lives at the bottom of the screen
void drawRemainingLives(void);

// check if the user has entered the cheat code
bool checkCheatCode(void);

//static const int FRAMES_PER_SECOND = 30;
//static const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
// returns the current number of clock ticks
// that have elapsed since the system was started
// clock_t current_time;

void Game_initialize() {
    Terminal_init();
    // Terminal_init(192, 512);

    Willy_initialize();

    strcpy(game.MESSAIR, "AIR");

    // High score default
    strcpy(game.HGHSCOR, "000000");

    // Score defaults
    strcpy(game.SCORE, "0000"); // Overflow digits (these may be updated, but are never printed)
    strcpy(game.SCORBUF, "000000");

    // Scores text 'High Score 000000   Score 000000'
    strcpy(game.MESSHSSC, "High Score 000000   Score 000000");

    strcpy(game.MESSG, "Game");
    strcpy(game.MESSO, "Over");

    // Set initial clock tick value - for calculating FSP.
    current_time = clock();
}

void Game_tick() {
    if (Terminal_keyExit()) {
        Terminal_exit();
    }
    Terminal_redraw();

    millisleep(60);
    return;

//    clock_t last_tick = clock();
//    int sleep_time = SKIP_TICKS - timediff(current_time, last_tick);
//
//    if (sleep_time > 0) {
//        usleep((useconds_t) sleep_time * 1000);
//        current_time = clock();
//    } else {
//        // Shit, we are running behind!
//    }
}

void Game_play_intro() {
    // Clear the entire Spectrum display file
    Speccy_clearScreen();

    // Copy TITLESCR1 and TITLESCR2 to the top two-thirds of the display file
    for (int i = 0; i < 2048; i++) {
        Speccy_writeScreen(16384 + i, TITLESCR1[i]);
    }
    for (int i = 0; i < 2048; i++) {
        Speccy_writeScreen(16384 + 2048 + i, TITLESCR2[i]);
    }

    Terminal_redraw();

    // Draw Willy at 18493 (9,29)
    DRWFIX(&willy.sprites[64], 18493, 0);

    // Copy the attribute bytes from CAVERN19 to the top third of the attribute file
    uint16_t addr = 22528;
    for (int i = 0; i < 256; i++) {
        Speccy_writeAttribute(addr + i, CAVERN19[i]);
    }

    // Copy LOWERATTRS to the bottom two-thirds of the attribute file
    addr += 256;
    for (int i = 0; i < 512; i++) {
        Speccy_writeAttribute(addr + i, LOWERATTRS[i]);
    }

    Terminal_redraw();

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

        Terminal_redraw();

        // Pause for about 0.1s
        millisleep(72);

        // Is ENTER being pressed? If so, start the game
        if (Terminal_keyEnter()) {
            game.DEMO = 0;
            break;
        }
    }
}

// Returning true quits the game!
// FIXME: uses `goto` statements!
bool Game_play() {
    // FIXME: temporarily disable DEMO mode
    game.DEMO = 0;

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
        speccy.memory[16384 + i] = 0;
    }

    // This entry point is used by the routines at LOOP (when teleporting into a cavern
    // or reinitialising the current cavern after Willy has lost a life) and NXSHEET.
    NEWSHT:

    // FIXME: only using CAVERN0, while porting -MRC-

    // Copy the cavern definition into the game status buffer at 32768
    Cavern_initialize();

    //
    // Now, draw the screen
    //

    // Copy the cavern's attribute bytes into the buffer at 24064
    // FIXME: uses Attr Buff (Blank): Attr File = 22528
    for (int i = 0; i < 512; i++) {
        Speccy_writeAttribute(24064 + i, cavern.layout[i]);
    }

    // Draw the current cavern to the screen buffer at 28672
    DRAWSHEET();

    // Clear the bottom third of the display file
    for (int i = 0; i < 2048; i++) {
        Speccy_writeScreen(20480 + i, 0);
    }

    // Print the cavern name (see CAVERNNAME) at 20480 (16,0)
    PMESS(cavern.CAVERNNAME, 20480, 32);

    // Print 'AIR' (see MESSAIR) at 20512 (17,0)
    PMESS(&game.MESSAIR, 20512, 3);

    drawAirBar();

    // Print scores text at 20576 (19,0)
    PMESS(&game.MESSHSSC, 20576, 32);

    // Set the border colour
    OUT(cavern.BORDER);

    Terminal_redraw();

    // Are we in demo mode?
    if (game.DEMO > 0) {
        // Reset the game mode indicator (we're in demo mode)
        game.DEMO = 64;
    }

    // Main loop
    //
    // The first thing to do is check whether there are any
    // remaining lives to draw at the bottom of the screen.
    while (true) {
        drawRemainingLives();

        // Next, prepare the screen and attribute buffers for drawing to the screen.

        // Copy contents of attribute buffer at 24064 (the empty cavern) into the
        // attribute buffer at 23552
        // FIXME: all good, uses the Display File
        for (int i = 0; i < 512; i++) {
            speccy.memory[23552 + i] = speccy.memory[24064 + i];
        }

        // Copy the contents of the screen buffer at 28672 (empty cavern tiles)
        // into the screen buffer at 24576
        // FIXME: all good, uses the Display File
        for (int i = 0; i < 4096; i++) {
            speccy.memory[24576 + i] = speccy.memory[28672 + i];
        }

        // Move the horizontal guardians in the current cavern
        MOVEHG();

        // Are we in demo mode?
        // If not, check and set the attribute bytes for Willy's sprite in the
        // buffer at 23552, and draw Willy to the screen buffer at 24576.
        if (game.DEMO == 0) {
            if (MOVEWILLY()) {
                goto LOOP_4; // Willy has died!
            }
            if (WILLYATTRS()) {
                goto LOOP_4; // Willy has died!
            }
        }

        // Draw the horizontal guardians in the current cavern
        DRAWHG();

        // Move the conveyor in the current cavern
        MVCONVEYOR();

        // Draw the items in the current cavern and collect any that Willy is touching
        DRAWITEMS();

        switch (cavern.SHEET) {
            case 4:
                // Eugene's Lair
                if (EUGENE()) {
                    goto LOOP_4; // Willy has died!
                }
                break;
            case 13:
                // Skylab Landing Bay
                if (SKYLABS()) {
                    goto LOOP_4; // Willy has died!
                }
                break;
            case 8:
                // Wacky Amoebatrons, or beyond
                if (VGUARDIANS()) {
                    goto LOOP_4; // Willy has died!
                }
                break;
            case 7:
                // Miner Willy meets the Kong Beast
                if (KONGBEAST()) {
                    goto LOOP_4; // Willy has died!
                }
                break;
            case 11:
                // Return of the Alien Kong Beast
                if (KONGBEAST()) {
                    goto LOOP_4; // Willy has died!
                }
                break;
            case 18:
                // Solar Power Generator
                LIGHTBEAM();
                break;
            default:; // NOOP
        }

        // Draw the portal, or move to the next cavern if Willy has entered it
        if (CHKPORTAL()) {
            goto NEWSHT;
        }

        // This entry point is used by the routine at KILLWILLY.
        LOOP_4:
        // Copy the contents of the screen buffer at 24576 to the display file
        // FIXME: all good, uses the Display File
        for (int i = 0; i < 4096; i++) {
            Speccy_writeScreen(16384 + i, speccy.memory[24576 + i]);
        }

        Terminal_redraw();

        if (game.FLASH > 0) {
            game.FLASH--;

            // Move bits 0-2 into bits 3-5 and clear all the other bits
            uint8_t new_flash_value = (uint8_t) (rotl(game.FLASH, 3) & 56);

            // Set every attribute byte in the buffer at 23552 to this value
            // FIXME: Uses Attr Buffer: Attr File= 22528
            for (int i = 0; i < 512; i++) {
                speccy.memory[23552 + i] = new_flash_value;
            }

            Terminal_redraw();
        }

        // Copy the contents of the attribute buffer at 23552 to the attribute file
        // FIXME: all good, uses the Display File
        for (int i = 0; i < 512; i++) {
            Speccy_writeAttribute(22528 + i, speccy.memory[23552 + i]);
        }

        // Print the score (see SCORBUF) at (19,26)
        PMESS(&game.SCORBUF, 20602, 6);

        // Print the high score (see HGHSCOR) at (19,11)
        PMESS(&game.HGHSCOR, 20587, 6);

        Terminal_redraw();

        // Decrease the air remaining in the current cavern
        if (DECAIR()) {
            if (MANDEAD()) {
                return false; // goto START, and don't quit!
            } else {
                goto NEWSHT;
            }
        }

        if (Terminal_keyQuit()) {
            return true; // goto START, and make sure to quit the game!
        }

        if (Terminal_keyPaused()) {
            do {
                millisleep(50); // keep the FPS under control
            } while (!Terminal_keyPaused());
        }

        // Here we check whether Willy has had a fatal accident.
        // Has Willy landed after falling from too great a height, or collided with a nasty or a guardian?
        if (willy.AIRBORNE == 255) {
            if (MANDEAD()) {
                return false; // goto START, and don't quit!
            } else {
                goto NEWSHT;
            }
        }

        // Check key press to toggle the in-game music.
        if (Terminal_keyMuteMusic()) {
            game.playMusic = ~game.playMusic;
        }

        // Play some music, unless in-game music been switched off
        if (game.playMusic) {
            // Increment the in-game music note index at NOTEINDEX
            game.NOTEINDEX++;

            // Point HL at the appropriate entry in the tune data table at GAMETUNE
            uint8_t index = rotr((uint8_t) (game.NOTEINDEX & 126), 1);

            // Pick up the border colour for the current cavern from BORDER
            uint8_t note = cavern.BORDER;

            // Initialise the pitch delay counter in E
            uint8_t pitch_delay_counter = GAMETUNE[index];

            // Initialise the duration delay counters in B (0) and C (3)
            for (int i = 0; i < 3; i++) {
                // Produce a note of the in-game music
                OUT(note);

                pitch_delay_counter--;

                if (pitch_delay_counter > 0) {
                    pitch_delay_counter = GAMETUNE[index];
                    note ^= 24;
                }

                millisleep(1);
            }
        }

        // If we're in demo mode, check the keyboard and return
        // to the title screen if there's any input.
        if (game.DEMO > 0) {
            // We're in demo mode; is it time to show the next cavern?
            if (game.DEMO - 1 == 0) {
                if (MANDEAD()) {
                    return false; // goto START, and don't quit!
                } else {
                    goto NEWSHT;
                }
            }
            // Update the game mode indicator at DEMO
            game.DEMO--;

            if (Terminal_keyAny()) {
                return false; // goto START, and don't quit!
            }
        }

        checkCheatCode();

        Game_tick();
    } // end main loop
}


bool checkCheatCode() {
    // IMPORTANT: not handling cheat codes just yet -MRC-
    /* TODO
      // Here we check the teleport keys.
      // NODEM1:
      LD BC,61438             // Read keys 6-7-8-9-0
      IN A,(C)
      BIT 4,A                 // Is '6' (the activator key) being pressed?
      JP NZ,CKCHEAT           // Jump if not
      LD A,(CHEAT)            // Pick up the 6031769 key counter from CHEAT
      CP 7                    // Has 6031769 been keyed in yet?
      JP NZ,CKCHEAT           // Jump if not
      LD B,247                // Read keys 1-2-3-4-5
      IN A,(C)
      CPL                     // Keep only bits 0-4 and flip them
      AND 31
      CP 20                   // Is the result 20 or greater?
      JP NC,CKCHEAT           // Jump if so (this is not a cavern number)
      LD (SHEET),A            // Store the cavern number at SHEET
      JP NEWSHT               // Teleport into the cavern

    // Now check the 6031769 keys.
    CKCHEAT:
      LD A,(CHEAT)            // Pick up the 6031769 key counter from CHEAT
      CP 7                    // Has 6031769 been keyed in yet?
      JP Z,LOOP               // If so, jump back to the start of the main loop
      RLCA                    // Point IX at the corresponding entry in the 6031769
      LD E,A                  // table at CHEATDT
      LD D,0
      LD IX,CHEATDT
      ADD IX,DE
      LD BC,63486             // Read keys 1-2-3-4-5
      IN A,(C)
      AND 31                  // Keep only bits 0-4
      CP (IX+0)               // Does this match the first byte of the entry in the 6031769 table?
      JR Z,CKNXCHT            // Jump if so
      CP 31                   // Are any of the keys 1-2-3-4-5 being pressed?
      JP Z,LOOP               // If not, jump back to the start of the main loop
      CP (IX-2)               // Does the keyboard reading match the first byte of the previous entry in the 6031769 table?
      JP Z,LOOP               // If so, jump back to the start of the main loop
      XOR A                   // Reset the 6031769 key counter at CHEAT to 0 (an
      LD (CHEAT),A            // incorrect key is being pressed)
      JP LOOP                 // Jump back to the start of the main loop
    CKNXCHT:
      LD B,239                // Read keys 6-7-8-9-0
      IN A,(C)
      AND 31                  // Keep only bits 0-4
      CP (IX+1)               // Does this match the second byte of the entry in the 6031769 table?
      JR Z,INCCHT             // If so, jump to increment the 6031769 key counter
      CP 31                   // Are any of the keys 6-7-8-9-0 being pressed?
      JP Z,LOOP               // If not, jump back to the start of the main loop
      CP (IX-1)               // Does the keyboard reading match the second byte of the previous entry in the 6031769 table?
      JP Z,LOOP               // If so, jump back to the start of the main loop
      XOR A                   // Reset the 6031769 key counter at CHEAT to 0 (an
      LD (CHEAT),A            // incorrect key is being pressed)
      JP LOOP                 // Jump back to the start of the main loop
    INCCHT:
      LD A,(CHEAT)            // Increment the 6031769 key counter at CHEAT (the
      INC A                   // next key in the sequence is being pressed)
      LD (CHEAT),A
    */

    return false;
}

void drawAirBar() {
    // Initialise A to 82 (is 20992); this is the MSB of the display file address
    // at which to start drawing the bar that represents the air supply.
    for (uint8_t a = 82; a < 86; a++) {
        uint16_t addr = build_address(a, 36);

        // Draw a single row of pixels across C cells
        for (uint16_t i = 0; i < cavern.AIR - 36; i++) {
            Speccy_writeScreen(addr + i, 255);
        }
    }
}

void drawRemainingLives() {
    // Set address to the display file address at which to draw the first Willy sprite
    uint16_t addr = 20640;

    // The following loop draws the remaining lives at the bottom of the screen.
    for (int i = 0; i < willy.NOMEN; i++) {
        // LD C,0                  // C=0; this tells the sprite-drawing routine at DRWFIX to overwrite any existing graphics

        // PUSH HL                 // Save HL and BC briefly
        // PUSH BC
        // LD A,(NOTEINDEX)        // Pick up the in-game music note index from NOTEINDEX; this will determine the animation frame for the Willy sprites
        // RLCA                    // Now A=0 (frame 0), 32 (frame 1), 64 (frame 2) or 96 (frame 3)
        // RLCA
        // RLCA
        // AND 96
        uint8_t anim_frame = (uint8_t) (rotl(game.NOTEINDEX, 3) & 96);

        uint8_t *sprite = &willy.sprites[anim_frame];
        DRWFIX(sprite, addr, 0);

        // Move to the location at which to draw the next Willy sprite
        addr += 2;
    }

    // If cheat mode has been activated draw the boot at the bottom
    // of the screen next to the remaining lives.
    if (game.CHEAT) {
        DRWFIX(&BOOT, addr, 0);
    }
}
