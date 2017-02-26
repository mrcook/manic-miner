// Manic Miner C Port Copyright 2017 Michael R. Cook

#include "headers.h"
#include "externs.h"

#include "terminal.h"

// Original password to enable cheat mode
int CHEATDT[7] = {6, 0, 3, 1, 7, 6, 9};

bool gameIsRunning = true;

void Game_initialize(int lives, bool cheat, int teleport) {
    Terminal_init();
    // Terminal_init(192, 512);

    Willy_initialize(lives);

    game.CHEAT = cheat;

    if (teleport >= 0) {
        game.teleportMode = true;
        game.teleportCavernNumber = teleport;
    }

    strcpy(game.airLabel, "AIR");

    // High score default
    strcpy(game.HGHSCOR, "000000");

    // Score defaults
    strcpy(game.SCORE, "0000"); // Overflow digits (these may be updated, but are never printed)
    strcpy(game.SCORBUF, "000000");

    // Scores text 'High Score 000000   Score 000000'
    strcpy(game.MESSHSSC, "High Score 000000   Score 000000");

    strcpy(game.MESSG, "Game");
    strcpy(game.MESSO, "Over");
}

// Returning true quits the game!
bool Game_play() {
    // Should reset the current cavern data. Used in place of the `goto NEWSHT`
    static bool reinitialiseCavern = true;

    if (game.teleportMode) {
        cavern.SHEET = (uint8_t) game.teleportCavernNumber;
    } else {
        cavern.SHEET = 0;
    }

    // Initialise the in-game music note index at NOTEINDEX
    game.NOTEINDEX = 0;

    // Initialise the screen flash counter at FLASH
    game.FLASH = 0;

    // Initialise in game music state
    game.playMusic = true;

    // Initialise the score at SCORE
    game.current_score = 0;

    EUGDIR = 0;
    EUGHGT = 0;

    // Prepare the screen; clear the entire Spectrum display file
    Speccy_clearScreen();

    int keyIntput;

    // Yup, the game has started successfully
    gameIsRunning = true;

    // The Main Loop
    while (gameIsRunning) {
        keyIntput = processInput();
        keyIntput = processMoveJumpInput(keyIntput);

        if (reinitialiseCavern) {
            loadCurrentCavern();
            reinitialiseCavern = false;
        }

        // The first thing to do is check whether there are any
        // remaining lives to draw at the bottom of the screen.
        drawRemainingLives();

        // Next, prepare the screen and attribute buffers for drawing to the screen.
        resetScreenAttrBuffers();

        // Check key press to toggle the in-game music.
        switch (keyIntput) {
            case MM_KEY_MUTE:
                game.playMusic = ~game.playMusic;
                break;
            case MM_KEY_QUIT:
                return true; // return true so we quit the game!
            case MM_KEY_PAUSE:
                keyIntput = MM_KEY_NONE;
                while (Terminal_getKey() != MM_KEY_PAUSE) {
                    millisleep(50); // keep the FPS under control
                }
                break;
            default:;
        }

        // Move the horizontal guardians in the current cavern
        MOVEHG();

        // Are we in demo mode?
        // If not, check and set the attribute bytes for Willy's sprite in the
        // buffer at 23552, and draw Willy to the screen buffer at 24576.
        if (game.DEMO == 0) {
            // Move Willy
            if (MOVEWILLY(keyIntput)) {
                goto LOOP_4; // Willy has died!
            }
            // Draw Willy
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
                // LIGHTBEAM(); // FIXME: LIGHTBEAM() is broken!
                break;
            default:
                ; // NOOP
        }

        // FIXME: this should be moved up, directly after moving Willy...?
        // Draw the portal, or move to the next cavern if Willy has entered it
        if (CHKPORTAL()) {
            reinitialiseCavern = true;
            continue;
        }

LOOP_4: // This entry point is used by the routine at KILLWILLY.
        copyScrBufToDisplayFile();

        Terminal_redraw();

        flashScreen(); // redraws the screen too!

        copyAttrBufToAttrFile();

        printScores();

        Terminal_redraw();

        // Decrease the air remaining in the current cavern
        DECAIR();

        // Has willy ran our of air or had a fatal accident?
        // Has Willy landed after falling from too great a height, or collided with a nasty or a guardian?
        if (Cavern_isAirDepleted() || willy.AIRBORNE == 255) {
            if (MANDEAD()) {
                gameIsRunning = false;
                break;
                // return false; // goto START, and don't quit!
            } else {
                reinitialiseCavern = true;
                continue;
            }
        }

        // Play some music, unless in-game music been switched off
        if (game.playMusic) {
            playGameMusic();
        }

        // If we're in demo mode, return to the title screen if user presses a key
        if (game.DEMO > 0) {
            // We're in demo mode; is it time to show the next cavern?
            if (game.DEMO == 1) {
                if (MANDEAD()) {
                    return false; // goto START, and don't quit!
                }
                reinitialiseCavern = true;
                continue;
            }

            // Update the game mode indicator at DEMO
            game.DEMO--;

            if (keyIntput == MM_KEY_ENTER) {
                gameIsRunning = false;
                break;
                // return false; // goto START, and don't quit!
            }
        }

        checkCheatCode(); // if in cheat mode, detect cavern teleport number

        Speccy_tick();
    } // end main loop

    // return but don't quit
    return false;
}

// Load the current cavern data (was NEWSHT)
// This entry point is used when:
//     teleporting into a cavern
//     reinitialising the current cavern after Willy has lost a life)
//     after NXSHEET.
void loadCurrentCavern() {
    // Copy the cavern definition into the game status buffer at 32768
    if (!Cavern_loadData(cavern.SHEET)) {
        // Oops! We've not loaded the right amount of cavern data into memory
        Terminal_exit();
        exit(-1);
    }

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
    Speccy_printMessage(cavern.CAVERNNAME, 20480, 32);

    // Print 'AIR' (see MESSAIR) at 20512 (17,0)
    Speccy_printMessage(&game.airLabel, 20512, 3);

    drawAirBar();

    // Print scores text at 20576 (19,0)
    Speccy_printMessage(&game.MESSHSSC, 20576, 32);

    // Set the border colour
    OUT(cavern.BORDER);

    Terminal_redraw();

    // Are we in demo mode?
    if (game.DEMO > 0) {
        // Reset the game mode indicator (we're in demo mode)
        game.DEMO = 64;
    }
}

void flashScreen() {
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
}

// The air in the cavern has run out, or Willy has had a fatal accident, or it's
// demo mode and it's time to show the next cavern.
// IMPORTANT: return `true` is `goto START`, `false` is `goto NEWSHT` -MRC-
bool MANDEAD() {
    // If in demo mode move to the next cavern
    if (game.DEMO) {
        // IMPORTANT: no need to check NXSHEET return value,
        // we know we should `goto NEWSHT` after it -MRC-
        NXSHEET();
        return false; // aka goto NEWSHT;
    }

    uint8_t pitch, duration;

    // The following loop fills the top two thirds of the attribute file with
    // a single value (71 TO 64 STEP -1) and makes a sound effect.
    // A=71 (INK 7: PAPER 0: BRIGHT 1)
    // Decrement `attr` (effectively decrementing the INK colour)
    // Have we used attribute value 64 (INK 0) yet?
    // Update the INK colour in the top two thirds of the screen and make another sound effect
    for (uint8_t attr = 71; attr > 64; attr--) {
        // Fill the top two thirds of the attribute file with the value in A
        for (int i = 0; i < 512; i++) {
            speccy.memory[22528 + i] = attr;
        }

        // D=63-8*(E AND 7); this value determines the pitch of the short note that will be played
        pitch = ~attr;
        pitch = (uint8_t)(pitch & 7);
        pitch = rotl(pitch, 3);
        pitch |= 7;

        // C=8+32*(E AND 7); this value determines the duration of the short note that will be played
        duration = attr;
        duration = rotr(duration, 3);
        // Set bit 4 of A (for no apparent reason)
        duration |= 16;

        // Set A=0 (this will make the border black)
        uint8_t border = 0;

        Speccy_setBorderColour(border);
        Speccy_makeSound(pitch, duration, 1);
    }

    // Finally, check whether any lives remain.
    if (willy.NOMEN < 1) {
        // If not, display the game over sequence
        ENDGAM();
        return true; // goto START;
    }

    // Decrease the number of lives remaining by one
    if (!game.CHEAT) {
        willy.NOMEN--;
    }

    // Jump back to reinitialise the current cavern
    return false; // goto NEWSHT;
}

// Display the game over sequence
void ENDGAM() {
    // First check whether we have a new high score.
    if (game.current_score > game.highscore) {
        game.highscore = game.current_score;
        memcpy(game.HGHSCOR, game.SCORBUF, sizeof(&game.SCORBUF));
    }

    // Now prepare the screen for the game over sequence.

    // Clear the top two-thirds of the display file
    Speccy_clearScreenDownTo(4096);

    Terminal_redraw();

    // determines the distance of the boot from the top of the screen
    uint8_t bootDistanceFromTop = 0;

    // Draw Willy at 18575 (12,15) using frame 3
    DRWFIX(&willy.sprites[64], 18575, 0);

    // Draw the plinth (see PLINTH) underneath Willy at 18639 (14,15)
    DRWFIX(&PLINTH, 18639, 0);

    Terminal_redraw();

    uint8_t msb, lsb;
    uint16_t addr;

    // The following loop draws the boot's descent onto the plinth that supports Willy.
    for (bootDistanceFromTop = 0; bootDistanceFromTop < 98; bootDistanceFromTop += 4) {
        split_address(SBUFADDRS[bootDistanceFromTop], &msb, &lsb);
        addr = build_address((uint8_t)(msb - 32), (uint8_t)(lsb | 15)); // center of screen

        // Draw the boot (see BOOT) at this location, without erasing the boot
        // at the previous location; this leaves the portion of the boot sprite
        // that's above the ankle in place, and makes the boot appear as if
        // it's at the end of a long, extending trouser leg.
        DRWFIX(&BOOT, addr, 0);

        // Pick up the distance variable from EUGHGT  (A=255-A)
        uint8_t distance = (uint8_t) (255 - bootDistanceFromTop);

        // Store this value (63-255) in E; it determines the (rising) pitch of
        // the sound effect that will be made.
        uint8_t border = 0; // (black border)
        Speccy_setBorderColour(border);

        // C=64; this value determines the duration of the sound effect
        // Speccy_makeSound(border, 64, (uint8_t)(distance / 216));

        // FIXME: delay would be in makeSound (which is wrong anyway),
        // so we must delay to get the correct boot descending effect
        for (int d = 64; d > 0; d--) {
            millisleep(distance / 216);
        }

        // Keep only bits 2 and 3
        distance = (uint8_t) (bootDistanceFromTop & 12);
        // Shift bits 2 and 3 into bits 3 and 4; these bits determine the PAPER colour: 0, 1, 2 or 3
        distance = rotl(distance, 1);
        // Set bits 0-2 (INK 7) and 6 (BRIGHT 1)
        distance |= 71;

        // Copy this attribute value into the top two-thirds of the screen
        for (int i = 0; i < 512; i++) {
            Speccy_writeAttribute(22528 + i, distance);
        }
        Terminal_redraw();

        Speccy_tick();
    }

    // Now print the "Game Over" message, just to drive the point home.
    Speccy_printMessage(&game.MESSG, 16586, 4);
    Speccy_printMessage(&game.MESSO, 16594, 4);

    // The following loop makes the "Game Over" message glisten for about 1.57s.
    // The counter will also determine the INK colours to use for the "Game Over" message
    for (int d = 6; d > 0; d--) {
        // Delay for about a millisecond
        millisleep(1);

        for (int a = 0; a < 8; a++) {
            // LD A,C                  // Change the INK colour of the "G" in "Game" at
            // AND 7                   // (6,10)
            // OR 64
            // LD (22730),A
            // INC A                   // Change the INK colour of the "a" in "Game" at
            // AND 7                   // (6,11)
            // OR 64
            // LD (22731),A
            // INC A                   // Change the INK colour of the "m" in "Game" at
            // AND 7                   // (6,12)
            // OR 64
            // LD (22732),A
            // INC A                   // Change the INK colour of the "e" in "Game" at
            // AND 7                   // (6,13)
            // OR 64
            // LD (22733),A
            // INC A                   // Change the INK colour of the "O" in "Over" at
            // AND 7                   // (6,18)
            // OR 64
            // LD (22738),A
            // INC A                   // Change the INK colour of the "v" in "Over" at
            // AND 7                   // (6,19)
            // OR 64
            // LD (22739),A
            // INC A                   // Change the INK colour of the "e" in "Over" at
            // AND 7                   // (6,20)
            // OR 64
            // LD (22740),A
            // INC A                   // Change the INK colour of the "r" in "Over" at
            // AND 7                   // (6,21)
            // OR 64
            // LD (22741),A

            // IMPORTANT: haha, and you think this is correct? -MRC-
            Speccy_writeAttribute(22730 + a, (uint8_t) (((d + a) & 7) | 64));
            Terminal_redraw();
        }
    }

    // IMPORTANT: caller must return to START
}

// Decrease the air remaining in the current cavern,
// and the game CLOCK
void DECAIR() {
    // Update the game clock at CLOCK
    cavern.CLOCK -= 4;

    // Was it just decreased from zero?
    if (cavern.CLOCK == 252) {
        // Has the air supply run out?
        if (Cavern_isAirDepleted()) {
            return;
        }
        cavern.AIR--;
    }

    // A=INT(A/32); this value specifies how many pixels to draw from left to
    // right in the cell at the right end of the air bar
    uint8_t count = (uint8_t) (cavern.CLOCK & 224);
    count = rotl(count, 3);

    // Initialise E to 0 (all bits reset)
    uint8_t pixels = 0;

    // Do we need to draw any pixels in the cell at the right end of the air bar?
    if (count != 0) {
        // Copy the number of pixels to draw (1-7) to B
        for (int i = count; i > 0; i--) {
            // Set this many bits in E (from bit 7 towards bit 0)
            pixels = rotr(pixels, 1);
            pixels |= 1 << 7;
        }
    }

    // Set HL to the display file address at which to draw the top row of
    // pixels in the cell at the right end of the air bar.
    // There are four rows of pixels to draw
    for (uint8_t msb = 82; msb < 86; msb++) {
        // Draw the four rows of pixels at the right end of the air bar
        Speccy_writeScreen(build_address(msb, cavern.AIR), pixels);
    }
    Terminal_redraw();
}

// Draw the current cavern to the screen buffer at 28672
void DRAWSHEET() {
    // FIXME: Screen Buffer: Screen File = 16384
    uint16_t addr = 28672;
    uint8_t msb, lsb;

    uint8_t *sprite;

    int offset = 0;
    int col;

    // The following loop draws the 512 tiles for the cavern
    // to the screen buffer at 28672.
    // This is done for each of the 3 screen blocks.
    for (int i = 0; i < 512; i++) {
        if (i > 255) {
            offset = 2048;
        }

        uint8_t tile_id = cavern.layout[i];

        if (cavern.BACKGROUND.id == tile_id) {
            sprite = &cavern.BACKGROUND.sprite[0];
        } else if (cavern.FLOOR.id == tile_id) {
            sprite = &cavern.FLOOR.sprite[0];
        } else if (cavern.CRUMBLING.id == tile_id) {
            sprite = &cavern.CRUMBLING.sprite[0];
        } else if (cavern.WALL.id == tile_id) {
            sprite = &cavern.WALL.sprite[0];
        } else if (cavern.CONVEYOR.id == tile_id) {
            sprite = &cavern.CONVEYOR.sprite[0];
        } else if (cavern.NASTY1.id == tile_id) {
            sprite = &cavern.NASTY1.sprite[0];
        } else if (cavern.NASTY2.id == tile_id) {
            sprite = &cavern.NASTY2.sprite[0];
        } else if (cavern.EXTRA.id == tile_id) {
            sprite = &cavern.EXTRA.sprite[0];
        }

        // Copy the graphic bytes to their destination cells
        uint16_t row_addr = addr;
        for (int b = 0; b < 8; b++) {
            if (sprite != NULL) {
                speccy.memory[row_addr + offset] = sprite[b];
            }
            split_address(row_addr, &msb, &lsb);
            msb++;
            row_addr = build_address(msb, lsb);
        }
        col++;

        split_address(addr, &msb, &lsb);
        if (col == 31) {
            col = 0;
            msb++;
        }
        lsb++;
        addr = build_address(msb, lsb);
    }

    // The empty cavern has been drawn to the screen buffer at 28672.
    // If we're in The Final Barrier, however, there is further work to do.
    if (cavern.SHEET == 19) {
        // Copy the graphic data from TITLESCR1 to the top half
        // of the screen buffer at 28672
        // FIXME: Blank Screen Buffer: Screen File = 16384
        for (int i = 0; i <= 2048; i++) {
            speccy.memory[28672 + i] = TITLESCR1[i];
        }
    }
}

// Move Willy (1)
// This routine deals with Willy if he's jumping or falling.
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool MOVEWILLY(int keyIntput) {
    // Is Willy jumping?
    if (willy.AIRBORNE == 1) {
        // Willy is currently jumping.

        // Pick up the jumping animation counter (0-17) from JUMPING
        uint8_t jump = willy.JUMPING;

        // Now -8<=A<=8 (and A is even)
        jump &= ~(1 << 0); // RES 0,A
        jump -= 8;         // SUB 8

        // Adjust Willy's pixel y-coordinate at PIXEL_Y depending on where Willy is in the jump
        willy.PIXEL_Y += jump;

        // Adjust Willy's attribute buffer location at LOCATION depending on his pixel y-coordinate
        MOVEWILLY_7(willy.PIXEL_Y);

        // NOTE: Pick up the attribute byte of the wall tile. The byte, not the whole sprite!
        // Is the top-left or top-right cell of Willy's sprite overlapping a wall tile?
        if (speccy.memory[willy.PIXEL_Y] == cavern.WALL.id || speccy.memory[willy.PIXEL_Y + 1] == cavern.WALL.id) {
            MOVEWILLY_10();
            return false;
        }

        // Increment the jumping animation counter at JUMPING
        willy.JUMPING++;

        // A=J-8, where J (1-18) is the new value of the jumping animation counter
        // Jump if J>=8
        // A=8-J (1<=J<=7, 1<=A<=7)
        uint8_t anim_counter;
        if (willy.JUMPING < 8) {
            anim_counter = (uint8_t) (8 - willy.JUMPING);
        } else {
            anim_counter = (uint8_t) (willy.JUMPING - 8);
        }

        // A=1+ABS(J-8)
        anim_counter++;

        // D=8*(1+ABS(J-8)); this value determines the pitch of the jumping
        // sound effect (rising as Willy rises, falling as Willy falls)
        anim_counter = rotl(anim_counter, 3);

        uint8_t delay = anim_counter;

        // Pick up the border colour for the current cavern from BORDER
        uint8_t border = cavern.BORDER;
        Speccy_setBorderColour(border);

        // C=32; this value determines the duration of the jumping sound effect
        Speccy_makeSound(border, 32, delay);

        // Jumping animation counter will have a value of 1-18.
        // FIXME: another part of the program says a value of 0-17.
        // Has Willy reached the end of the jump? Is the jumping animation counter now 16 or 13?
        if (willy.JUMPING == 18) {
            // Willy has just finished a jump.
            // Set the airborne status indicator at AIRBORNE to 6:
            // Willy will continue to fall unless he's landed on a wall or floor block
            willy.AIRBORNE = 6;
            return false;
        } else if (willy.JUMPING == 16) {
            // jump to MOVEWILLY_3
        } else if (willy.JUMPING != 13) {
            MOVEWILLY2_6();
            return false;
        }
    }

    // If we get here, then Willy is standing on the floor, or he's falling,
    // or his jumping animation counter is 13 (at which point Willy is on his
    // way down and is exactly two cell-heights above where he started the jump)
    // or 16 (at which point Willy is on his way down and is exactly one
    // cell-height above where he started the jump).

    uint16_t addr;

    // Does Willy's sprite occupy six cells at the moment?
    if ((willy.PIXEL_Y & 15) == 0) {
        // Point HL at the left-hand cell below Willy's sprite
        addr = (uint16_t) (willy.LOCATION + 64);

        // Pick up the attribute byte of the crumbling floor tile for the current cavern from CRUMBLING
        // Does the left-hand cell below Willy's sprite contain a crumbling floor tile?
        // If so, make it crumble
        if (speccy.memory[addr] == cavern.CRUMBLING.id) {
            CRUMBLE(addr);
        }

        if (speccy.memory[addr] == cavern.NASTY1.id) {
            // Does the left-hand cell below Willy's sprite contain a nasty tile?
            // Jump if so
        } else if (speccy.memory[addr] == cavern.NASTY2.id) {
            // Does the left-hand cell below Willy's sprite contain a nasty tile?
            // Jump if so
        } else {
            // Point HL at the right-hand cell below Willy's sprite
            addr++;

            // Pick up the attribute byte of the crumbling floor tile for the current cavern from CRUMBLING
            // Does the right-hand cell below Willy's sprite contain a crumbling floor tile?
            // If so, make it crumble
            if (speccy.memory[addr] == cavern.CRUMBLING.id) {
                CRUMBLE(addr);
            }

            if (speccy.memory[addr] == cavern.NASTY1.id) {
                // Does the right-hand cell below Willy's sprite contain a nasty tile?
                // Jump if so
            } else if (speccy.memory[addr] == cavern.NASTY2.id) {
                // Does the right-hand cell below Willy's sprite contain a nasty tile?
                // Jump if so
            } else {
                // Pick up the attribute byte of the background tile for the current cavern from BACKGROUND
                // Set the zero flag if the right-hand cell below Willy's sprite is empty
                // Point HL at the left-hand cell below Willy's sprite
                // Jump if the right-hand cell below Willy's sprite is not empty
                if (speccy.memory[addr] != cavern.BACKGROUND.id) {
                    return MOVEWILLY2(keyIntput, addr);
                }
                // Is the left-hand cell below Willy's sprite empty?
                if (speccy.memory[addr - 1] != cavern.BACKGROUND.id) {
                    return MOVEWILLY2(keyIntput, addr);
                }
            }
        }
    }

    // Is Willy jumping?
    if (willy.AIRBORNE == 1) {
        MOVEWILLY2_6();
        return false;
    }

    // If we get here, then Willy is either in the process of falling
    // or just about to start falling.

    // Reset bit 1 at DMFLAGS: Willy is not moving left or right
    willy.DMFLAGS &= ~(1 << 1);

    // Is Willy already falling?
    if (willy.AIRBORNE > 1) {
        // Willy has just started falling.
        // Set the airborne status indicator at AIRBORNE to 2
        willy.AIRBORNE = 2;

        return false;
    }

    // Increment the airborne status indicator at AIRBORNE
    willy.AIRBORNE++;

    // D=16*A; this value determines the pitch of the falling sound effect
    uint8_t pitch = rotl(willy.AIRBORNE, 4);

    // Pick up the border colour for the current cavern from BORDER
    uint8_t border = cavern.BORDER;
    Speccy_setBorderColour(border);

    // C=32; this value determines the duration of the falling sound effect
    Speccy_makeSound(pitch, 32, 1);

    // Add 8 to Willy's pixel y-coordinate at PIXEL_Y; this moves Willy downwards by 4 pixels
    willy.PIXEL_Y += 8;

    MOVEWILLY_7(willy.PIXEL_Y);

    return false;
}

// Adjust Willy's attribute buffer location at LOCATION
// to account for this new pixel y-coordinate.
void MOVEWILLY_7(uint8_t y_coord) {
    uint8_t msb, lsb;

    // L=16*Y, where Y is Willy's screen y-coordinate (0-14)
    lsb = (uint8_t) (y_coord & 240);

    // Now L=32*(Y-8*INT(Y/8)), and the carry flag is set if Willy is in the lower half of the cavern (Y>=8)
    // if LSB bit 7 is set the `RL L` will set the Carry, which means we need to add one to the MSB
    // H=92 or 93 (MSB of the address of Willy's location in the attribute buffer)
    msb = 92;
    // pickup bit-7 of lsb, the [C]arry flag, and check if it's set
    if (((lsb >> 7) & 1) == 1) {
        msb = 93;
    }

    // RL lsb
    lsb = rotl(lsb, 1);
    lsb &= ~(1 << 0); // set bit `0` to the [C]arry, which was set to `0`

    // Pick up Willy's screen x-coordinate (1-29) from bits 0-4 at LOCATION
    uint8_t msb_dummy, x_lsb;
    split_address((uint16_t)(willy.LOCATION & 31), &msb_dummy, &x_lsb);

    // Now L holds the LSB of Willy's attribute buffer address
    x_lsb |= lsb;

    // Store Willy's updated attribute buffer location at LOCATION
    willy.LOCATION = build_address(msb, x_lsb);
}

// The top-left or top-right cell of Willy's sprite is overlapping a wall tile.
void MOVEWILLY_10() {
    // Adjust Willy's pixel y-coordinate at PIXEL_Y so that the top row of
    // cells of his sprite is just below the wall tile
    willy.PIXEL_Y = (uint8_t) ((willy.PIXEL_Y + 16) & 240);

    // Adjust Willy's attribute buffer location at LOCATION to account for this new pixel y-coordinate
    MOVEWILLY_7(willy.PIXEL_Y);

    // Set the airborne status indicator at AIRBORNE to 2: Willy has started falling
    willy.AIRBORNE = 2;

    // Reset bit 1 at DMFLAGS: Willy is not moving left or right
    willy.DMFLAGS &= ~(1 << 1);
}

// Animate a crumbling floor tile in the current cavern
// HL Address of the crumbling floor tile's location in the attribute buffer at 23552
void CRUMBLE(uint16_t addr) {
    uint8_t msb, lsb;

    // Point BC at the bottom row of pixels of the crumbling floor tile
    // in the screen buffer at 28672
    split_address(addr, &msb, &lsb);

    msb += 27;
    msb |= 7;

    while (true) {
        // Collect the pixels from the row above in A
        msb--;

        // Copy these pixels into the row below it. Point BC at the next row of pixels up
        speccy.memory[build_address((uint8_t) (msb + 1), lsb)] = speccy.memory[build_address(msb, lsb)];

        // Have we dealt with the bottom seven pixel rows of the crumbling floor tile yet?
        // If not, jump back to deal with the next one up
        if ((msb & 7) == 0) {
            break;
        }
    }

    // Clear the top row of pixels in the crumbling floor tile
    speccy.memory[build_address(msb, lsb)] = 0;

    // Point BC at the bottom row of pixels in the crumbling floor tile
    msb += 7;

    // Pick up the bottom row of pixels in A. Is the bottom row clear? Return if not
    if (speccy.memory[build_address(msb, lsb)] == 0) {
        return;
    }

    // The bottom row of pixels in the crumbling floor tile is clear.
    // Time to put a background tile in its place.

    // Set HL to the address of the crumbling floor tile's location in the attribute buffer at 24064
    // Set the attribute at this location to that of the background tile
    // Set HL back to the address of the crumbling floor tile's location in the attribute buffer at 23552
    speccy.memory[build_address((uint8_t) (msb + 2), lsb)] = cavern.BACKGROUND.id;
}

// Move Willy (2)
// This routine checks the keyboard and joystick, and moves Willy left or right if necessary.
// HL Attribute buffer address of the left-hand cell below Willy's sprite
bool MOVEWILLY2(int keyIntput, uint16_t addr) {
    // Has Willy just landed after falling from too great a height? If so, kill him!
    if (willy.AIRBORNE == 12) { // FIXME: should this be `>= 12` ?
        KILLWILLY();
        return true;
    }

    // Initialise E to 255 (all bits set); it will be used to hold keyboard and joystick readings
    // uint8_t input = 255;

    // Reset the airborne status indicator at AIRBORNE (Willy has landed safely)
    willy.AIRBORNE = 0;

    uint8_t converyor_dir = 255;
    // Does the attribute byte of the left or right hand cell below Willy's sprite match that of the conveyor tile?
    if (speccy.memory[addr] == cavern.CONVEYOR.id || speccy.memory[addr + 1] == cavern.CONVEYOR.id) {
        // Pick up the direction byte of the conveyor definition from CONVDIR (0=left, 1=right)
        // Now E=253 (bit 1 reset) if the conveyor is moving left, or 254 (bit 0 reset) if it's moving right
        // input = (uint8_t) (cavern.CONVEYOR.CONVDIR - 3);
        converyor_dir = cavern.CONVEYOR.CONVDIR;
    }

    // Read keys P-O-I-U-Y (right, left, right, left, right) into bits 0-4 of A. Set bit 5 and reset bits 6 and 7
    // Reset bit 0 if the conveyor is moving right, or bit 1 if it's moving left. Save the result in E
    // Read keys Q-W-E-R-T (left, right, left, right, left) into bits 0-4 of A
    // Keep only bits 0-4, shift them into bits 1-5, and set bit 0
    // Merge this keyboard reading into bits 1-5 of E
    // Read keys 1-2-3-4-5 ('5' is left) into bits 0-4 of A
    // Rotate the result right and set bits 0-2 and 4-7; this ignores every key except '5' (left)
    // Merge this reading of the '5' key into bit 3 of E
    // Read keys 0-9-8-7-6 ('8' is right) into bits 0-4 of A
    // Set bits 0, 1 and 3-7; this ignores every key except '8' (right)
    // Merge this reading of the '8' key into bit 2 of E

    // At this point, bits 0-5 in E indicate the direction in which Willy is being
    // moved or trying to move. If bit 0, 2 or 4 is reset, Willy is being moved or
    // trying to move right; if bit 1, 3 or 5 is reset, Willy is being moved or
    // trying to move left.

    // LD C,0                  // Initialise C to 0 (no movement)
    // LD A,E                  // Copy the movement bits into A
    // AND 42                  // Keep only bits 1, 3 and 5 (the 'left' bits)
    // CP 42                   // Are any of these bits reset?
    // JR Z,MOVEWILLY2_3       // Jump if not
    // LD C,4                  // Set bit 2 of C: Willy is moving left
    // MOVEWILLY2_3:
    // LD A,E                  // Copy the movement bits into A
    // AND 21                  // Keep only bits 0, 2 and 4 (the 'right' bits)
    // CP 21                   // Are any of these bits reset?
    // JR Z,MOVEWILLY2_4       // Jump if not
    // SET 3,C                 // Set bit 3 of C: Willy is moving right
    // FIXME: does this really work?
    uint8_t movement = 0;
    if (keyIntput == MM_KEY_LEFT || keyIntput == MM_KEY_LEFT_JUMP || converyor_dir == 0) {
        movement = 4;
    } else if (keyIntput == MM_KEY_RIGHT || keyIntput == MM_KEY_RIGHT_JUMP || converyor_dir == 1) {
        movement = (1 << 3);
    }

    // Point HL at the entry in the left-right movement table at LRMOVEMENT
    // that corresponds to the direction Willy is facing, and the direction
    // in which he is being moved or trying to move.
    movement = willy.DMFLAGS + movement;
    // Update Willy's direction and movement flags at DMFLAGS with the entry
    // from the left-right movement table.
    willy.DMFLAGS = LRMOVEMENT[movement];


    // That is left-right movement taken care of. Now check the jump keys.

    // Is jump being pressed?
    if (keyIntput == MM_KEY_JUMP || keyIntput == MM_KEY_LEFT_JUMP || keyIntput == MM_KEY_RIGHT_JUMP) {
        // Time to make Willy jump.
        // Initialise the jumping animation counter at JUMPING
        willy.JUMPING = 0;

        // Set the airborne status indicator at AIRBORNE to 1: Willy is jumping
        willy.AIRBORNE = 1;
    }

    MOVEWILLY2_6(); // IMPORTANT: implicit function call -MRC-

    return false; // willy is not dead. -MRC-
}

// This entry point is used by the routine at MOVEWILLY.
void MOVEWILLY2_6() {
    // Is Willy moving? Return if not
    if ((willy.DMFLAGS & 2) == 0) {
        return;
    }

    // Is Willy facing right? (0 = right, 1 = left) Return if not.
    if ((willy.DMFLAGS & 1) == 0) {
        MOVEWILLY2_9();
        return;
    }

    // Willy is moving left.

    // Pick up Willy's animation frame from FRAME.
    // Is it 0? If so, jump to move Willy's sprite left across a cell boundary
    if (willy.FRAME == 0) {
        MOVEWILLY2_7();
        return;
    }

    // Decrement Willy's animation frame at FRAME
    willy.FRAME--;
}

// Willy's sprite is moving left across a cell boundary.
// In the comments that follow, (x,y) refers to the coordinates
// of the top-left cell currently occupied by Willy's sprite.
void MOVEWILLY2_7() {
    // Collect Willy's attribute buffer coordinates from LOCATION
    // Point HL at the cell at (x-1,y+1)
    uint16_t addr = (uint16_t) (willy.LOCATION - 1);

    addr += 32;

    // Pick up the attribute byte of the wall tile for the current cavern from WALL
    // Is there a wall tile in the cell pointed to by HL?
    // Return if so without moving Willy (his path is blocked)
    if (speccy.memory[addr] == cavern.WALL.id) {
        return;
    }

    // Does Willy's sprite currently occupy only two rows of cells?
    if (willy.PIXEL_Y & 15) {
        // Pick up the attribute byte of the wall tile for the current cavern from WALL
        // Point HL at the cell at (x-1,y+2)
        // Is there a wall tile in the cell pointed to by HL?
        // Return if so without moving Willy (his path is blocked)
        if (speccy.memory[addr + 32] == cavern.WALL.id) {
            return;
        }
    }

    // Pick up the attribute byte of the wall tile for the current cavern from WALL
    // FIXME: does this Carry flag get used anywhere?
    // OR A                    // Clear the carry flag for subtraction
    // SBC HL,DE               // Point HL at the cell at (x-1,y)
    addr -= 32;
    // Is there a wall tile in the cell pointed to by HL?
    // Return if so without moving Willy (his path is blocked)
    if (speccy.memory[addr] == cavern.WALL.id) {
        return;
    }

    // Save Willy's new attribute buffer coordinates (in HL) at LOCATION
    willy.LOCATION = addr;

    // Change Willy's animation frame at FRAME from 0 to 3
    willy.FRAME = 3;
}

// Willy is moving right.
void MOVEWILLY2_9() {
    // Pick up Willy's animation frame from FRAME
    // Is it 3? If so, move Willy's sprite right across a cell boundary
    if (willy.FRAME == 3) {
        MOVEWILLY2_10();
    } else {
        // Increment Willy's animation frame at FRAME
        willy.FRAME++;
    }
}

// Willy's sprite is moving right across a cell boundary.
// In the comments that follow, (x,y) refers to the coordinates
// of the top-left cell currently occupied by Willy's sprite.
void MOVEWILLY2_10() {
    // Collect Willy's attribute buffer coordinates from LOCATION
    // Point HL at the cell at (x+2,y)
    uint16_t addr = (uint16_t)(willy.LOCATION + 2);

    // Pick up the attribute byte of the wall tile for the current cavern from WALL
    // Point HL at the cell at (x+2,y+1)
    addr += 32;

    // Is there a wall tile in the cell pointed to by HL?
    // Return if so without moving Willy (his path is blocked)
    if (speccy.memory[addr] == cavern.WALL.id) {
        return;
    }

    // Does Willy's sprite currently occupy only two rows of cells?
    if ((willy.PIXEL_Y & 15)) {
        // Pick up the attribute byte of the wall tile for the current cavern from WALL
        // Point HL at the cell at (x+2,y+2)
        // Is there a wall tile in the cell pointed to by HL?
        // Return if so without moving Willy (his path is blocked)
        if (speccy.memory[addr + 32] == cavern.WALL.id) {
            return;
        }
    }

    addr -= 32;

    // Is there a wall tile in the cell pointed to by HL?
    // Return if so without moving Willy (his path is blocked)
    if (speccy.memory[addr] == cavern.WALL.id) {
        return;
    }

    // Point HL at the cell at (x+1,y)
    addr--;

    // Save Willy's new attribute buffer coordinates (in HL) at LOCATION
    willy.LOCATION = addr;

    // Change Willy's animation frame at FRAME from 3 to 0
    willy.FRAME = 0;
}

// Kill Willy
// when Willy hits a nasty
// when Willy lands after falling from too great a height
// when Willy collides with a horizontal guardian)
// when Willy collides with Eugene)
// when Willy collides with a vertical guardian)
// when Willy collides with the Kong Beast
bool KILLWILLY() {
    // NOTE: merged KILLWILLY_0 and KILLWILLY_1 with this function as it was only caller,
    // even though the following message:
    //     This entry point is used by the routine at SKYLABS when a Skylab falls on Willy.

    // Set the airborne status indicator at AIRBORNE to 255 (meaning Willy has had a fatal accident)
    willy.AIRBORNE = 255;

    // Jump back into the main loop
    return true;
}

// Move the horizontal guardians in the current cavern
void MOVEHG() {
    uint8_t msb, lsb;

    // The guardian-moving loop begins here.
    for (int i = 0; i < 4; i++) {
        GuardianHorizontal *guardian = &HGUARDS[i];

        // FIXME: double check this, but probably correct.
        if (guardian->speedColour == 255) {
            return;
        }

        if (GuardianH_is_blank(guardian)) {
            continue;
        }

        uint8_t current_clock = cavern.CLOCK;
        current_clock &= 4;
        current_clock = rotr(current_clock, 3);

        // Jump to consider the next guardian if this one is not due to be moved on this pass
        if ((current_clock & guardian->speedColour)) {
            continue;
        }

        // The guardian will be moved on this pass.
        // Pick up the current animation frame (0-7)

        // Is it 3 (the terminal frame for a guardian moving right)?
        // If so move the guardian right across a cell boundary or turn it round
        if (guardian->frame == 3) {
            // Pick up the LSB of the address of the guardian's location in the attribute buffer at 23552
            split_address(guardian->attributeAddress, &msb, &lsb);

            // Has the guardian reached the rightmost point in its path?
            if (lsb == guardian->addressRightLSB) {
                // Set the animation frame to 7 (turning the guardian round to face left)
                guardian->frame = 7;
                // Jump forward to consider the next guardian
            } else {
                // Set the animation frame to 0 (the initial frame for a guardian moving right)
                guardian->frame = 0;
                // Increment the guardian's x-coordinate (moving it right across a cell boundary)
                lsb++;
                guardian->attributeAddress = build_address(msb, lsb);
                // Jump forward to consider the next guardian
            }

            // Is the current animation frame 4 (the terminal frame for a guardian moving left)?
            // If so move the guardian left across a cell boundary or turn it round
        } else if (guardian->frame == 4) {
            // Pick up the LSB of the address of the guardian's location in the attribute buffer at 23552
            split_address(guardian->attributeAddress, &msb, &lsb);

            // Has the guardian reached the leftmost point in its path?
            if (lsb == guardian->addressLeftLSB) {
                // Set the animation frame to 0 (turning the guardian round to face right)
                guardian->frame = 0;
            } else {
                // Set the animation frame to 7 (the initial frame for a guardian moving left)
                guardian->frame = 7;
                // Decrement the guardian's x-coordinate (moving it left across a cell boundary)
                lsb--;
                guardian->attributeAddress = build_address(msb, lsb);
            }

            // Jump if the animation frame is 5, 6 or 7
        } else if (guardian->frame == 5 || guardian->frame == 6 || guardian->frame == 7) {
            // Decrement the animation frame (this guardian is moving left)
            guardian->frame--;
        } else {
            // Increment the animation frame (this guardian is moving right)
            guardian->frame++;
        }

        // The current guardian definition has been dealt with. Time for the next one.
    }
}

// Move and draw the light beam in Solar Power Generator
void LIGHTBEAM() {
    // Point HL at the cell at (0,23) in the attribute buffer at 23552 (the source of the light beam)
    // FIXME: Screen Buffer: Screen File = 22551
    uint16_t addr = 23575;

    // Prepare DE for addition (the beam travels vertically downwards to start with)
    int8_t dir = 32; // gets toggled between 32 and -1.

    // The beam-drawing loop begins here.
    while (true) {
        // Does HL point at a floor or wall tile? Return if so (the light beam stops here)
        if (speccy.memory[addr] == cavern.FLOOR.id || speccy.memory[addr] == cavern.WALL.id) {
            return;
        }

        // A=39 (INK 7: PAPER 4)
        // Does HL point at a tile with this attribute value?
        // Jump if not (the light beam is not touching Willy)
        if (speccy.memory[addr] == 39) {
            // Decrease the air supply by four units
            DECAIR();
            DECAIR();
            DECAIR();
            DECAIR();
            // Jump forward to draw the light beam over Willy
        } else {
            // Does HL point at a background tile? Jump if so (the light beam will not be reflected at this point)
            if (speccy.memory[addr] == cavern.BACKGROUND.id) {
                // Toggle the value in DE between 32 and -1 (and therefore the
                // direction of the light beam between vertically downwards and
                // horizontally to the left): the light beam has hit a guardian
                dir ^= 223;
                dir = ~dir;
            }
        }

        // Draw a portion of the light beam with attribute value 119 (INK 7: PAPER 6: BRIGHT 1)
        speccy.memory[addr] = 119;

        // Point HL at the cell where the next portion of the light beam will be drawn
        addr += dir;

        // Jump back to draw the next portion of the light beam
    }
}

// Draw the horizontal guardians in the current cavern
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool DRAWHG() {
    uint8_t msb, lsb;
    uint16_t addr;

    // Point IY at the first byte of the first horizontal guardian definition at HGUARDS

    // The guardian-drawing loop begins here.
    for (int i = 0; i < 4; i++) {
        GuardianHorizontal *guardian = &HGUARDS[i];

        // Have we dealt with all the guardians yet?
        if (guardian->speedColour == 255) {
            return false;
        }

        // Is this guardian definition blank? If so, skip it and consider the next one.
        if (guardian->attributeAddress == 0) {
            continue;
        }

        // Point HL at the address of the guardian's location in the attribute buffer at 23552
        addr = guardian->attributeAddress;

        // Reset bit 7 (which specifies the animation speed) of the attribute byte, ensuring no FLASH
        uint8_t attr = (uint8_t) guardian->speedColour;
        attr &= 127;

        // Set the attribute bytes for the guardian in the buffer at 23552
        speccy.memory[addr] = attr;
        addr++;
        speccy.memory[addr] = attr;
        addr += 31;
        speccy.memory[addr] = attr;
        addr++;
        speccy.memory[addr] = attr;

        // Pick up the animation frame (0-7)
        uint8_t anim_frame = (uint8_t) guardian->frame;
        // Multiply it by 32
        anim_frame = rotr(anim_frame, 3);

        // FIXME: we need to check this logic.
        // Are we not in one of the first seven caverns, The Endorian Forest, or The Sixteenth Cavern?
        if (cavern.SHEET > 7 && cavern.SHEET != 9 && cavern.SHEET != 15) {
            // Add 128 to E (the horizontal guardians in this cavern use frames 4-7 only)
            anim_frame |= (1 << 7);
        }

        // Point DE at the graphic data for the appropriate guardian sprite (at GGDATA+E)
        // Point HL at the address of the guardian's location in the screen buffer at 24576
        split_address(guardian->attributeAddress, &msb, &lsb);
        addr = build_address((uint8_t) guardian->addressMSB, lsb);

        // Draw the guardian to the screen buffer at 24576
        bool kill_willy = DRWFIX(&guardian->GGDATA[anim_frame], addr, 1);

        // Kill Willy if the guardian collided with him
        if (kill_willy) {
            KILLWILLY();
            return true;
        }
    }

    return false; // IMPORTANT Willy has not died! -MRC-
}

// Move and draw Eugene in Eugene's Lair
// First we move Eugene up or down, or change his direction.
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool EUGENE() {
    // Have all the items been collected, or is Eugene moving downwards?
    if (game.ITEMATTR != 0 && EUGDIR != 0) {
        // Pick up Eugene's pixel y-coordinate from EUGHGT
        // Decrement it (moving Eugene up)
        // Jump if Eugene has reached the top of the cavern
        if (EUGHGT - 1 == 0) {
            // Toggle Eugene's direction at EUGDIR
            EUGDIR = (uint8_t) !EUGDIR;
        } else {
            // Update Eugene's pixel y-coordinate at EUGHGT
            EUGHGT--;
        }
    } else {
        // Pick up Eugene's pixel y-coordinate from EUGHGT
        // Increment it (moving Eugene down)
        // Has Eugene reached the portal yet? Jump if so
        if (EUGHGT + 1 == 88) {
            EUGDIR = (uint8_t) !EUGDIR;
        } else {
            // Update Eugene's pixel y-coordinate at EUGHGT
            EUGHGT++;
        }
    }

    uint8_t msb, lsb;
    uint8_t x_msb, x_lsb;
    uint16_t addr;

    // Now that Eugene's movement has been dealt with, it's time to draw him.

    // Pick up Eugene's pixel y-coordinate from EUGHGT
    // Point DE at the entry in the screen buffer address lookup table at
    // SBUFADDRS that corresponds to Eugene's y-coordinate.
    // Point HL at the address of Eugene's location in the screen buffer at 24576
    uint8_t y_coord = (uint8_t) (EUGHGT & 127);
    y_coord = rotl(y_coord, 1);
    addr = SBUFADDRS[y_coord];
    addr |= 15;
    split_address(addr, &msb, &lsb);
    y_coord++;
    addr = SBUFADDRS[y_coord];
    split_address(addr, &x_msb, &x_lsb);
    addr = build_address(x_msb, lsb);

    // Draw Eugene to the screen buffer at 24576
    bool kill_willy = DRWFIX(&EUGENEG, addr, 1);

    // Kill Willy if Eugene collided with him
    if (kill_willy) {
        KILLWILLY();
        return true;
    }


    // Pick up Eugene's pixel y-coordinate from EUGHGT
    // Point HL at the address of Eugene's location in the attribute buffer at 23552
    y_coord = (uint8_t) (EUGHGT & 120);
    y_coord = rotl(y_coord, 1);
    y_coord |= 7;

    // IMPORTANT: SCF should set the carry flag, and the following RL loads that into bit 0 -MRC-
    // SCF
    // RL A
    msb = 0;
    if ((y_coord >> 7) & 1) {
        msb = 1;
    }
    y_coord = rotl(y_coord, 1);
    y_coord |= 1 << 0;
    // LD L,A
    // LD A,0
    // ADC A,92
    msb += 92;
    // LD H,A
    addr = build_address(msb, y_coord);

    // Assume we will draw Eugene with white INK
    uint8_t ink_colour = 7;

    // Pick up the attribute of the last item drawn from ITEMATTR
    // Set the zero flag if all the items have been collected
    // Jump if there are items remaining to be collected
    if (game.ITEMATTR == 0) {
        // Pick up the value of the game clock at CLOCK
        // Move bits 2-4 into bits 0-2 and clear the other bits; this value
        // (which decreases by one on each pass through the main loop) will
        // be Eugene's INK colour
        ink_colour = (uint8_t) (rotr(cavern.CLOCK, 2) & 7);
    }

    EUGENE_3(addr, ink_colour); // IMPORTANT: implicit jump -MRC-

    return false;
}

// Sets the colour attributes for a 16x16 pixel sprite
// SKYLABS:    to set the attributes for a Skylab
// VGUARDIANS: to set the attributes for a vertical guardian
// KONGBEAST:  to set the attributes for the Kong Beast
void EUGENE_3(uint16_t addr, uint8_t ink_colour) {
    // Save the INK colour in the attribute buffer temporarily
    speccy.memory[addr] = ink_colour;

    // Pick up the attribute byte of the background tile for the current cavern from BACKGROUND
    // Combine its PAPER colour with the chosen INK colour
    // Set the attribute byte for the top-left cell of the sprite in the attribute buffer at 23552
    speccy.memory[addr] = (uint8_t) ((cavern.BACKGROUND.sprite[0] & 248) | speccy.memory[addr]);

    // Set the attribute byte for the top-right cell of the sprite in the attribute buffer at 23552
    addr++;
    speccy.memory[addr] = ink_colour;

    // Set the attribute byte for the middle-left cell of the sprite in the attribute buffer at 23552
    addr += 31;
    speccy.memory[addr] = ink_colour;

    // Set the attribute byte for the middle-right cell of the sprite in the attribute buffer at 23552
    addr++;
    speccy.memory[addr] = ink_colour;

    // Set the attribute byte for the bottom-left cell of the sprite in the attribute buffer at 23552
    addr += 31;
    speccy.memory[addr] = ink_colour;

    // Set the attribute byte for the bottom-right cell of the sprite in the attribute buffer at 23552
    addr++;
    speccy.memory[addr] = ink_colour;
}

// Move and draw the Skylabs in Skylab Landing Bay
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool SKYLABS() {
    uint8_t msb;
    uint8_t lsb;
    uint16_t addr;

    // The Skylab-moving loop begins here.
    for (int i = 0; i < 4; i++) {
        GuardianVertical *guardian = &VGUARDS[i];

        // Have we dealt with all the Skylabs yet? If so, re-enter the main loop
        if (guardian->attribute == 255) {
            break;
        }

        // Has it reached its crash site yet?
        if (guardian->yCoord != guardian->yCoordMaximum) {
            // Increment the Skylab's y-coordinate (moving it downwards)
            guardian->yCoord += guardian->yPixelIncrement;
        } else {
            // The Skylab has reached its crash site. Start or continue its disintegration.

            // Increment the animation frame
            guardian->frame++;

            // Pick up the animation frame.
            // Has the Skylab completely disintegrated yet?
            if (guardian->frame == 8) {
                // Reset the Skylab's pixel y-coordinate
                guardian->yCoord = guardian->yCoordMinimum;

                // Add 8 to the Skylab's x-coordinate (wrapping around at the right side of the screen)
                guardian->xCoord += 8;
                guardian->xCoord &= 31;

                // Reset the animation frame to 0
                guardian->frame = 0;
            }
        }

        // Now that the Skylab's movement has been dealt with, time to draw it.

        // FIXME: are these address calculations correct? -MRC-
        // Are x/y coords uint16_t or uint8_t values?

        // LD E,(IY+2)             // Pick up the Skylab's pixel y-coordinate in E
        // RLC E                   // Point DE at the entry in the screen buffer address
        // LD D,131                // lookup table at SBUFADDRS that corresponds to the Skylab's pixel y-coordinate
        uint8_t y_coord = rotl(guardian->yCoord, 1);
        // LD A,(DE)               // Point HL at the address of the Skylab's location in
        addr = SBUFADDRS[y_coord];
        // ADD A,(IY+3)            // the screen buffer at 24576
        addr += guardian->xCoord;
        split_address(addr, &msb, &lsb);
        // LD L,A
        // INC DE
        y_coord++;
        // LD A,(DE)
        addr = SBUFADDRS[y_coord];

        uint8_t y_msb, y_lsb;
        split_address(addr, &y_msb, &y_lsb);
        // LD H,A
        addr = build_address(y_msb, lsb);

        // Pick up the animation frame (0-7). Multiply it by 32.
        // Skylab sprite (at GGDATA+A)
        uint8_t sprite_offset = rotr(guardian->frame, 3);

        // Draw the Skylab to the screen buffer at 24576
        bool kill_willy = DRWFIX(&guardian->GGDATA[sprite_offset], addr, 1);

        // Kill Willy if the Skylab collided with him
        if (kill_willy) {
            KILLWILLY();
            return true;
        }

        // Point HL at the address of the Skylab's location in the attribute buffer at 23552
        addr = (uint16_t) (rotl((uint8_t) (guardian->yCoord & 64), 2) + 92);
        split_address(addr, &msb, &lsb);
        uint8_t msb_bak = msb;
        addr = (uint8_t) (rotl(guardian->yCoord, 2) & 224);
        addr |= guardian->xCoord;
        split_address(addr, &msb, &lsb);
        addr = build_address(msb_bak, lsb);

        // Set the attribute bytes for the Skylab
        EUGENE_3(addr, guardian->attribute);

        // The current guardian definition has been dealt with. Time for the next one.
    }

    return false; // Willy is not dead!
}

// Move and draw the vertical guardians in the current cavern
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool VGUARDIANS() {
    uint8_t msb, lsb;
    uint8_t msb_bak, lsb_bak;

    // The guardian-moving loop begins here.
    for (int i = 0; i < 4; i++) {
        GuardianVertical *guardian = &VGUARDS[i];

        // Have we dealt with all the guardians yet?
        if (guardian->attribute == 255) {
            return false;
        }

        // Increment the guardian's animation frame, or reset to 0 if it overflowed to 4
        if (guardian->frame < 3) {
            guardian->frame++;
        } else {
            guardian->frame = 0;
        }

        // Pick up the guardian's pixel y-coordinate
        // Add the current y-coordinate increment
        uint16_t y_coord = guardian->yCoord + guardian->yPixelIncrement;

        // FIXME: JR based on C(arry) flag
        // Has the guardian reached the lowest/highest point of its path (minimum y-coordinate)?
        // If so, jump to change its direction of movement
        if (y_coord != guardian->yCoordMinimum && y_coord != guardian->yCoordMaximum) {
            // Update the guardian's pixel y-coordinate
            guardian->yCoord += y_coord;
        } else {
            // Negate the y-coordinate increment; this changes the guardian's direction of movement
            // FIXME: this of course should still be a positive number! -MRC-
            guardian->yPixelIncrement = (uint8_t) (-guardian->yPixelIncrement);
        }

        // Now that the guardian's movement has been dealt with, time to draw it.

        // Pick up the guardian's pixel y-coordinate
        // Point DE at the entry in the screen buffer address lookup table at
        // SBUFADDRS that corresponds to the guardian's pixel y-coordinate.
        y_coord = rotl((uint8_t) (guardian->yCoord & 127), 1);
        uint16_t addr = SBUFADDRS[y_coord];

        // Point HL at the address of the guardian's location in the screen buffer at 24576
        split_address(addr, &msb, &lsb);
        lsb_bak = lsb | guardian->xCoord;
        y_coord++;
        addr = SBUFADDRS[y_coord];
        split_address(addr, &msb, &lsb);
        addr = build_address(msb, lsb_bak);

        // Pick up the guardian's animation frame (0-3). Multiply it by 32.
        uint8_t anim_frame = rotr(guardian->frame, 3);

        // Draw the guardian to the screen buffer at 24576
        bool kill_willy = DRWFIX(&guardian->GGDATA[anim_frame], addr, 1);

        // Kill Willy if the guardian collided with him
        if (kill_willy) {
            KILLWILLY();
            return true;
        }

        // Pick up the guardian's pixel y-coordinate
        // Point HL at the address of the guardian's location in the attribute buffer at 23552
        addr = (uint16_t) (rotl((uint8_t) (guardian->yCoord & 64), 2) + 92);
        split_address(addr, &msb, &lsb);
        msb_bak = msb;

        addr = (uint16_t) (rotl(guardian->yCoord, 2) & 224);
        split_address(addr, &msb, &lsb);
        lsb |= guardian->xCoord;
        addr = build_address(msb_bak, lsb);

        // Set the attribute bytes for the guardian
        EUGENE_3(addr, guardian->attribute);

        // The current guardian definition has been dealt with. Time for the next one.
    }

    return false; // Willy is not dead.
}

// Draw the items in the current cavern and collect any that Willy is touching
void DRAWITEMS() {
    uint8_t msb;
    uint8_t lsb;
    uint16_t addr;

    // Initialise the attribute of the last item drawn at ITEMATTR
    // to 0 (in case there are no items left to draw)
    game.ITEMATTR = 0;

    // The item-drawing loop begins here.
    for (int i = 0; i < 5; i++) {
        // Have we dealt with all the items yet?
        if (cavern.ITEMS[i].attribute == 255) {
            return;
        }

        // Has this item already been collected? If so, skip it and consider the next one.
        if (cavern.ITEMS[i].attribute == 0) {
            continue;
        }

        // Point DE at the address of the item's location in the attribute buffer at 23552
        addr = cavern.ITEMS[i].address;

        // Pick up the current attribute byte at the item's location
        // Is the INK white (which happens if Willy is touching the item)?
        if ((speccy.memory[addr] & 7) == 7) {
            // Willy is touching this item, so add it to his collection.

            // Add 100 to the score
            game.current_score += 100;
            INCSCORE_0(33836);

            // Set the item's attribute byte to 0 so that it will be skipped the next time
            cavern.ITEMS[i].attribute = 0;

            // Jump forward to consider the next item
        } else {
            // This item has not been collected yet.

            // Pick up the item's current attribute byte
            // Keep the BRIGHT and PAPER bits, and set the INK to 3 (magenta)
            // Store this value in B
            uint8_t attribute = (uint8_t) ((cavern.ITEMS[i].attribute & 248) | 3);

            // Pick up the item's current attribute byte again
            // Keep only bits 0 and 1 and add the value in B; this maintains the
            // BRIGHT and PAPER bits, and cycles the INK colour through 3, 4, 5 and 6
            attribute += (cavern.ITEMS[i].attribute & 3);

            // Store the new attribute byte
            cavern.ITEMS[i].attribute = attribute;

            // Update the attribute byte at the item's location in the buffer at 23552
            speccy.memory[addr] = attribute;

            // Store the new attribute byte at ITEMATTR as well
            game.ITEMATTR = attribute;

            // Point DE at the address of the item's location in the screen buffer at 24576
            split_address(addr, &msb, &lsb);
            msb = (uint8_t) cavern.ITEMS[i].addressMSB;
            addr = build_address(msb, lsb);

            // Point HL at the item graphic for the current cavern (at ITEM)
            // There are eight pixel rows to copy
            // Draw the item to the screen buffer at 24576
            Speccy_printSpriteAt(cavern.ITEMS[i].tile, addr, 8);
        }

        // The current item definition has been dealt with. Time for the next one.
    }

    // All the items have been dealt with. Check whether there were any left.

    // Pick up the attribute of the last item drawn at ITEMATTR
    // Were any items drawn? Return if so (some remain to be collected).
    if (game.ITEMATTR != 0) {
        return;
    }

    // Ensure that the portal is flashing by setting bit 7 of its attribute byte at PORTAL
    cavern.portal.PORTAL |= (1 << 7);
}

// Draw the portal, or move to the next cavern if Willy has entered it
// First check whether Willy has entered the portal.
// IMPORTANT: the CALL-ers should be able to handle the `goto NEWSHT` themselves, on the return. -MRC-
bool CHKPORTAL() {
    uint8_t msb, lsb;
    uint8_t w_msb, w_lsb;

    // Pick up the address of the portal's location in the attribute buffer at 23552 from PORTALLOC1
    uint16_t addr = cavern.portal.PORTALLOC1;
    split_address(addr, &msb, &lsb);

    // Pick up the LSB of the address of Willy's location in the attribute buffer at 23552 from LOCATION
    uint16_t w_addr = willy.LOCATION;
    split_address(w_addr, &w_msb, &w_lsb);

    // Does it match that of the portal?
    if (lsb == w_lsb) {
        // Pick up the MSB of the address of Willy's location in the attribute buffer at 23552 from 32877
        // Does it match that of the portal?
        if (w_msb == msb) {
            // Is the portal flashing?
            if (((cavern.portal.PORTAL >> 7) & 1) == 1) {
                // Move Willy to the next cavern

                // IMPORTANT: no need to check NXSHEET, we know we should `goto NEWSHT` -MRC-
                NXSHEET();
                return true;
            }
        }
    }

    // Willy has not entered the portal, or it's not flashing, so just draw it.

    // Pick up the portal's attribute byte from PORTAL
    // Set the attribute bytes for the portal in the buffer at 23552
    speccy.memory[addr] = cavern.portal.PORTAL;
    addr++;
    speccy.memory[addr] = cavern.portal.PORTAL;
    addr += 31;
    speccy.memory[addr] = cavern.portal.PORTAL;
    addr++;
    speccy.memory[addr] = cavern.portal.PORTAL;

    // Point DE at the graphic data for the portal at PORTALG
    // Pick up the address of the portal's location in the screen buffer at 24576 from PORTALLOC2
    // C=0: overwrite mode
    DRWFIX(&cavern.portal.PORTALG, cavern.portal.PORTALLOC2, 0);
    // This routine continues into the one at DRWFIX.

    return false; // NOTE: callers should not `goto NEWSHT`.
}

// Draw a sprite
//
// Used by the routines at:
//   START:      draw Willy on the title screen
//   LOOP:       draw the remaining lives
//   ENDGAM:     draw Willy, the boot and the plinth during the game over sequence
//   DRAWHG:     draw horizontal guardians
//   EUGENE:     draw Eugene in Eugene's Lair
//   SKYLABS:    draw the Skylabs in Skylab Landing Bay
//   VGUARDIANS: draw vertical guardians
//   CHKPORTAL:  draw the portal in the current cavern
//   NXSHEET:    draw Willy above ground and the swordfish graphic over the portal in The Final Barrier
//   KONGBEAST:  draw the Kong Beast in Miner Willy meets the Kong Beast and Return of the Alien Kong Beast
//
// If C=1 on entry, this routine returns with the zero flag reset if any of the
// set bits in the sprite being drawn collides with a set bit in the background.
//
// C Drawing mode: 0 (overwrite) or 1 (blend)
// DE Address of sprite graphic data
// HL Address to draw at
bool DRWFIX(void *gfx_sprite, uint16_t addr, uint8_t mode) {
    uint8_t *sprite = gfx_sprite;

    uint8_t msb;
    uint8_t lsb;

    // Note: each sprite is 16x16 pixels, so we need to work in pairs of bytes
    for (int i = 0; i < 32; i += 2) {
        // Are we in blend mode?
        if (mode == 1) {
            if (sprite[i] & Speccy_readScreen(addr) || sprite[i + 1] & Speccy_readScreen(addr + 1)) {
                return true; // collision detected
            }
            sprite[i] |= Speccy_readScreen(addr);
            sprite[i + 1] |= Speccy_readScreen(addr + 1);
        }

        // Copy the graphic bytes to their destination cells
        Speccy_writeScreen(addr, sprite[i]);
        Speccy_writeScreen(addr + 1, sprite[i + 1]);

        // Move down to the next pixel row
        split_address(addr, &msb, &lsb);
        msb++;

        // Have we drawn the bottom pixel row in this pair of cells yet?
        if (msb & 7) {
            addr = build_address(msb, lsb);
            continue;
        }

        // Otherwise move to the top pixel row in the cell below
        msb -= 8;
        lsb += 32;

        // Was the last pair of cells at y-coordinate 7 or 15?
        // Otherwise adjust to account for the movement from the top
        // or middle third of the screen to the next one down
        if ((lsb & 224) == 0) {
            msb += 8;
        }

        addr = build_address(msb, lsb);
    }

    return false; // no collision detected
}

// Move to the next cavern
// IMPORTANT: the CALLers should be able to handle the `goto NEWSHT` themselves, on the return. -MRC-
bool NXSHEET() {
    uint16_t addr;

    // Pick up the number of the current cavern from SHEET
    // Increment the cavern number
    uint8_t next_sheet = (uint8_t) (cavern.SHEET + 1);

    // Is the current cavern The Final Barrier?
    if (cavern.SHEET == 19) {
        // Are we in demo mode, or cheat mode activated?
        if (game.DEMO == 0 && game.CHEAT == false) {
            // Willy has made it through The Final Barrier without cheating.

            // Draw Willy at (2,19) on the ground above the portal
            DRWFIX(&willy.sprites[96], 16467, 0);

            // Draw the swordfish graphic (see SWORDFISH) over the portal
            DRWFIX(&SWORDFISH, 16563, 0);

            // Point HL at (2,19) in the attribute file
            addr = 22611;

            // Set the attributes for the upper half of Willy's
            // sprite at (2,19) and (2,20) to 47 (INK 7: PAPER 5)
            Speccy_writeAttribute(addr, 47);
            addr++;
            Speccy_writeAttribute(addr, 47);

            // Set the attributes for the lower half of Willy's
            // sprite at (3,19) and (3,20) to 39 (INK 7: PAPER 4)
            addr += 31;
            Speccy_writeAttribute(addr, 39);
            addr++;
            Speccy_writeAttribute(addr, 39);

            // Point HL at (5,19) in the attribute file
            addr += 31;
            addr++;
            addr += 31;

            // Set the attributes for the fish at (5,19) and (5,20) to 69 (INK 5: PAPER 0: BRIGHT 1)
            Speccy_writeAttribute(addr, 69);
            addr++;
            Speccy_writeAttribute(addr, 69);

            // Set the attribute for the handle of the sword at (6,19) to 70 (INK 6: PAPER 0: BRIGHT 1)
            addr += 31;
            Speccy_writeAttribute(addr, 70);

            // Set the attribute for the blade of the sword at (6,20) to 71 (INK 7: PAPER 0: BRIGHT 1)
            addr++;
            Speccy_writeAttribute(addr, 71);

            // Set the attributes at (7,19) and (7,20) to 0 (to hide Willy's feet just below where the portal was)
            addr += 31;
            Speccy_writeAttribute(addr, 0);
            addr++;
            Speccy_writeAttribute(addr, 0);

            // Prepare C and D for the celebratory sound effect
            uint8_t border = 0; // (black border)

            for (int i = 0; i < 50; i++) {
                // Produce the celebratory sound effect: Willy has escaped from the mine
                OUT(border);
                border ^= 24;

                millisleep(1);
            }
        }

        // The next cavern will be Central Cavern
        next_sheet = 0;
    }

    // Update the cavern number at SHEET
    if (game.teleportMode) {
        cavern.SHEET = (uint8_t) game.teleportCavernNumber;
    } else {
        cavern.SHEET = next_sheet;
    }

    // The next section of code cycles the INK and PAPER colours of the current cavern.

    // Initialise A to 63 (INK 7: PAPER 7)
    uint8_t colours = 63;

    for (uint8_t ink = colours; ink > 0; ink--) {
        // Set the attributes for the top two-thirds of the screen to the value in A
        for (int i = 0; i < 512; i++) {
            Speccy_writeAttribute(22528 + i, ink);
        }

        // Pause for about 0.004s
        millisleep(4);

        Terminal_redraw();

        // Jump back until we've gone through all attribute values from 63 down to 1
    }

    // Are we in demo mode?
    if (game.DEMO) {
        // If so, demo the next cavern
        return true;
    }

    // The following loop increases the score and decreases the air supply until it runs out.
    while (true) {
        // Decrease the air remaining in the current cavern
        DECAIR();

        // Move to the next cavern if the air supply is now gone
        if (Cavern_isAirDepleted()) {
            return true;
        }

        // Add 1 to the score
        game.current_score++;
        INCSCORE_0(33838);

        // Print the new score at (19,26)
        Speccy_printMessage(&game.SCORBUF, 20602, 6);

        // C=4; this value determines the duration of the sound effect
        uint8_t duration = 4;

        // Pick up the remaining air supply (S) from AIR
        // D=2*(63-S); this value determines the pitch of the sound effect
        // (which decreases with the amount of air remaining)
        uint8_t pitch = rotl((uint8_t) (~cavern.AIR & 63), 1);

        for (int i = duration; i > 0; i--) {
            // Produce a short note
            OUT(0);
            millisleep(pitch);
            OUT(24);
            millisleep(1);
        }

        // Jump back to decrease the air supply again
    }

    return false; // IMPORTANT: no `goto NEWSHT` required. -MRC-
}

// The entry point to this routine is here and is used by the routines at
// DRAWITEMS, NXSHEET and KONGBEAST with HL pointing at the digit of the
// score (see SCORBUF) to be incremented.
void INCSCORE_0(uint16_t addr) {
    uint8_t msb;
    uint8_t lsb;

    for (;;) {
        // Pick up a digit of the score, is it '9'?
        if (speccy.memory[addr] < 57) {
            speccy.memory[addr]++;
            return;
        }

        // Roll the digit over from '9' to '0'
        speccy.memory[addr] = 48;

        // Point HL at the next digit to the left
        addr--;

        // Is this the 10000s digit?
        split_address(addr, &msb, &lsb);
        if (lsb == 42) {
            game.FLASH = 8;
            willy.NOMEN++;
        }
    }
}

// Move the conveyor in the current cavern
void MVCONVEYOR() {
    uint8_t h, l, d, e; // MSB/LSB for the address.
    uint8_t pixels_a, pixels_c;

    // Pick up the address of the conveyor's location in the screen buffer at 28672 from CONVLOC
    uint16_t row_hl = cavern.CONVEYOR.CONVLOC;

    // Copy this address to DE
    uint16_t row_de = cavern.CONVEYOR.CONVLOC;

    // Is the conveyor moving left?
    if (cavern.CONVEYOR.CONVDIR == 0) {
        // Copy the first pixel row of the conveyor tile to A
        // Rotate it left twice
        pixels_a = rotl(speccy.memory[row_hl], 2);

        // Point HL at the third pixel row of the conveyor tile
        split_address(row_hl, &h, &l);
        h += 2;
        row_hl = build_address(h, l);

        // Copy this pixel row to C
        pixels_c = rotr(speccy.memory[row_hl], 2);
    } else {
        // The conveyor is moving right.

        // Copy the first pixel row of the conveyor tile to A
        pixels_a = rotr(speccy.memory[row_hl], 2);

        // Point HL at the third pixel row of the conveyor tile
        split_address(row_hl, &h, &l);
        h += 2;
        row_hl = build_address(h, l);

        // Copy this pixel row to C
        pixels_c = rotl(speccy.memory[row_hl], 2);
    }

    for (int b = cavern.CONVEYOR.CONVLEN; b > 0; b--) {
        // Update the first and third pixel rows of every conveyor tile in the screen buffer at 28672
        speccy.memory[row_de] = pixels_a;
        speccy.memory[row_hl] = pixels_c;

        split_address(row_hl, &h, &l);
        l++;
        row_hl = build_address(h, l);

        split_address(row_de, &d, &e);
        e++;
        row_de = build_address(d, e);
    }
}

// Move and draw the Kong Beast in the current cavern
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool KONGBEAST() {
    uint16_t addr;
    uint8_t msb, lsb;

    // Flip the left-hand switch at (0,6) if Willy is touching it
    // FIXME: Screen Buffer: Screen File = 22534
    CHKSWITCH(23558);

    // Pick up the Kong Beast's status from EUGDIR
    // Is the Kong Beast already dead?
    if (EUGDIR == 2) {
        return false; // NOTE: willy is not dead!
    }

    // Pick up the sixth pixel row of the left-hand switch from the screen buffer at 28672

    // Has the switch been flipped?
    // FIXME: Blank Screen Buffer: Screen File = 17670
    if (speccy.memory[29958] != 16) {
        return KONGBEAST_8(); // return dead-ness state of Willy! -MRC-
    }

    // The left-hand switch has been flipped. Deal with opening up the wall if that is still in progress.

    // Pick up the attribute byte of the tile at (11,17) in the buffer at 24064
    // Has the wall there been removed yet?
    // FIXME: Blank Screen Buffer: Screen File = 22897
    if (speccy.memory[24433] != 0) {
        // Point HL at the bottom row of pixels of the wall tile at (11,17) in the screen buffer at 28672
        // FIXME: Blank Screen Buffer: Screen File = 20337
        addr = 32625;

        while (true) {
            split_address(addr, &msb, &lsb);

            // Pick up a pixel row. Is it blank yet?
            if (speccy.memory[addr] != 0) {
                // Clear a pixel row of the wall tile at (11,17) in the screen buffer at 28672
                speccy.memory[addr] = 0;

                // Point HL at the opposite pixel row of the wall tile one cell down at (12,17)
                lsb = 145;
                msb ^= 7;
                addr = build_address(msb, lsb);

                // Clear that pixel row as well
                speccy.memory[addr] = 0;

                break;
            }

            // Point HL at the next pixel row up
            msb--;
            // Have we checked all 8 pixel rows yet?
            // If not, jump back to check the next one
            if (msb != 119) {
                break;
            }

            addr = build_address(msb, lsb);
        }

        // Change the attributes at (11,17) and (12,17) in the buffer at 24064
        // to match the background tile (the wall there is now gone)
        // FIXME: Blank Screen Buffer: Screen File = 22897
        speccy.memory[24433] = cavern.BACKGROUND.id;
        // FIXME: Blank Screen Buffer: Screen File = 22929
        speccy.memory[24465] = cavern.BACKGROUND.id;

        // FIXME: I guess we need to update HGUARD2 directly rather than this memeory address.
        // Update the seventh byte of the guardian definition at HGUARD2 so
        // that the guardian moves through the opening in the wall
        speccy.memory[32971] = 114;
    }

    // Now check the right-hand switch.

    // Flip the right-hand switch at (0,18) if Willy is touching it (and it hasn't already been flipped)
    // Jump if the switch was not flipped
    // FIXME: Screen Buffer: Screen File = 22546
    if (!CHKSWITCH(23570)) {
        // Initialise the Kong Beast's pixel y-coordinate at EUGHGT to 0
        EUGHGT = 0;

        // Update the Kong Beast's status at EUGDIR to 1: he is falling
        EUGDIR = 1;

        // Change the attributes of the floor beneath the Kong Beast in the
        // buffer at 24064 to match that of the background tile
        // FIXME: Blank Screen Buffer: Screen File = 22607
        speccy.memory[24143] = cavern.BACKGROUND.id;
        // FIXME: Blank Screen Buffer: Screen File = 16463
        speccy.memory[24144] = cavern.BACKGROUND.id;

        // Point HL at (2,15) in the screen buffer at 28672
        // FIXME: Blank Screen Buffer: Screen File = 22608
        addr = 28751;

        // Clear the cells at (2,15) and (2,16), removing the floor beneath the Kong Beast
        for (int i = 8; i > 0; i--) {
            split_address(addr, &msb, &lsb);
            speccy.memory[addr] = 0;
            lsb++;
            addr = build_address(msb, lsb);
            speccy.memory[addr] = 0;
            lsb--;
            msb++;
            addr = build_address(msb, lsb);
        }
    }

    // Pick up the Kong Beast's status from EUGDIR
    // Is the Kong Beast still on the ledge?
    if (EUGDIR == 0) {
        return KONGBEAST_8();  // return dead-ness state of Willy! -MRC-
    }

    // The Kong Beast is falling.

    // Pick up the Kong Beast's pixel y-coordinate from EUGHGT
    // Has he fallen into the portal yet?
    if (EUGHGT != 100) {
        // Add 4 to the Kong Beast's pixel y-coordinate at EUGHGT (moving him downwards)
        EUGHGT += 4;

        // Copy the pixel y-coordinate to C; this value determines the pitch of the sound effect

        // D=16; this value determines the duration of the sound effect
        // Pick up the border colour for the current cavern from BORDER
        uint8_t border = cavern.BORDER;

        for (int i = 0; i < 16; i++) {
            // Make a falling sound effect
            OUT(border);
            border ^= 24;
            millisleep(4);
        }

        // Copy the Kong Beast's pixel y-coordinate back into A
        // Point DE at the entry in the screen buffer address lookup table at
        // SBUFADDRS that corresponds to the Kong Beast's pixel y-coordinate
        // Point HL at the address of the Kong Beast's location in the screen buffer at 24576
        addr = (uint16_t) (SBUFADDRS[rotl(EUGHGT, 1)] | 15);

        // Use bit 5 of the value of the game clock at CLOCK (which is toggled
        // once every eight passes through the main loop) to point DE at the
        // graphic data for the appropriate Kong Beast sprite
        uint16_t sprite_id = (uint16_t) ((cavern.CLOCK & 32) | 64);

        // Draw the Kong Beast to the screen buffer at 24576
        DRWFIX(&GGDATA[sprite_id], addr, 0);

        // Add 100 to the score
        game.current_score += 100;
        INCSCORE_0(33836);

        // Pick up the Kong Beast's pixel y-coordinate from EUGHGT
        // Point HL at the address of the Kong Beast's location in the attribute buffer at 23552
        lsb = (uint8_t) (EUGHGT & 120);
        msb = 23;
        addr = build_address(msb, lsb);

        // FIXME: HL + HL ? what is this actually doing?
        addr += addr;
        addr += addr;

        split_address(addr, &msb, &lsb);
        lsb |= 15;
        addr = build_address(msb, lsb);

        // The Kong Beast is drawn with yellow INK
        // Set the attribute bytes for the Kong Beast
        EUGENE_3(addr, 6);

        // return false; // FIXME: do we need to return here? Probably not.
    }

    // The Kong Beast has fallen into the portal.
    // Set the Kong Beast's status at EUGDIR to 2: he is dead
    EUGDIR = 2;

    return false; // return dead-ness state of Willy! -MRC-
}

// The Kong Beast is still on the ledge.
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool KONGBEAST_8() {
    // Use bit 5 of this value (which is toggled once every eight passes
    // through the main loop) to point DE at the graphic data for the
    // appropriate Kong Beast sprite
    uint8_t sprite_id = (uint8_t) (cavern.CLOCK & 32);

    // Draw the Kong Beast at (0,15) in the screen buffer at 24576
    // FIXME: Screen Buffer: Screen File = 16399
    bool kill_willy = DRWFIX(&GGDATA[sprite_id], 24591, 1);

    // Kill Willy if he collided with the Kong Beast
    if (kill_willy) {
        return true;
    }

    // A=68 (INK 4: PAPER 0: BRIGHT 1)
    // Set the attribute bytes for the Kong Beast in the buffer at 23552
    // FIXME: Screen Buffer: Screen File = 22575
    speccy.memory[23599] = 68;
    speccy.memory[23600] = 68;
    // FIXME: Screen Buffer: Screen File = 22543
    speccy.memory[23567] = 68;
    speccy.memory[23568] = 68;

    return false; // NOTE: willy is not dead.
}

// Flip a switch in a Kong Beast cavern if Willy is touching it
// Returns with the zero flag set if Willy flips the switch.
// HL Address of the switch's location in the attribute buffer at 23552
bool CHKSWITCH(uint16_t addr) {
    uint8_t msb, lsb;
    uint16_t w_addr;

    uint8_t sw_msb, sw_lsb;
    split_address(addr, &sw_msb, &sw_lsb);

    // Pick up the LSB of the address of Willy's location in the attribute buffer at 23552 from LOCATION
    // Is it equal to or one less than the LSB of the address of the switch's location?
    // Return (with the zero flag reset) if not
    // Pick up the MSB of the address of Willy's location in the attribute buffer at 23552 from 32877
    // Does it match the MSB of the address of the switch's location?
    // Return (with the zero flag reset) if not
    w_addr = willy.LOCATION;
    split_address(w_addr, &msb, &lsb);
    lsb++;
    lsb &= 254;
    if (msb == sw_msb || lsb == sw_lsb) {
        return false;
    }

    // IMPORTANT: I believe 32869 is the EXTRA[9] array, so I am using that -MRC-

    // Pick up the sixth byte of the graphic data for the switch tile from 32869
    // Point HL at the sixth row of pixels of the switch tile in the screen buffer at 28672
    sw_msb = 117;
    addr = build_address(sw_msb, sw_lsb);

    // Has the switch already been flipped?
    // Return (with the zero flag reset) if so
    if (speccy.memory[addr] == cavern.EXTRA.id) {
        return true;
    }

    // Willy is flipping the switch.

    // Update the sixth, seventh and eighth rows of pixels of the switch tile
    // in the screen buffer at 28672 to make it appear flipped
    speccy.memory[addr] = 8;
    sw_msb++;
    addr = build_address(sw_msb, sw_lsb);
    speccy.memory[addr] = 6;
    sw_msb++;
    addr = build_address(sw_msb, sw_lsb);
    speccy.memory[addr] = 6;

    // Return true: Willy has flipped the switch
    return true;
}

// Check and set the attribute bytes for Willy's sprite in the buffer at 23552
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool WILLYATTRS() {
    bool kill_willy = false;

    // Pick up the address of Willy's location in the attribute buffer at 23552 from LOCATION
    uint16_t addr = willy.LOCATION;

    // Set C=15 (`pix_y`) for the top two rows of cells
    // (to make the routine at WILLYATTR force white INK)
    uint8_t pix_y = 15;

    // Check and set the attribute byte for the top-left cell
    if (WILLYATTR(addr, pix_y)) {
        kill_willy = true;
    }

    // Move HL to the next cell to the right
    addr++;

    // Check and set the attribute byte for the top-right cell
    if (WILLYATTR(addr, pix_y)) {
        kill_willy = true;
    }

    // Move down a row and back one cell to the left
    addr += 31;

    // Check and set the attribute byte for the mid-left cell
    if (WILLYATTR(addr, pix_y)) {
        kill_willy = true;
    }

    // Move to the next cell to the right
    addr++;

    // Check and set the attribute byte for the mid-right cell
    if (WILLYATTR(addr, pix_y)) {
        kill_willy = true;
    }

    // Pick up Willy's pixel y-coordinate from PIXEL_Y. Copy it to pix_y
    pix_y = willy.PIXEL_Y;

    // Move down a row and back one cell to the left
    addr += 31;

    // Check and set the attribute byte for the bottom-left cell
    if (WILLYATTR(addr, pix_y)) {
        kill_willy = true;
    }

    // Move to the next cell to the right
    addr++;

    // Check and set the attribute byte for the bottom-right cell
    if (WILLYATTR(addr, pix_y)) {
        kill_willy = true;
    }

    // Draw Willy to the screen buffer at 24576
    DRAWWILLY();

    // Is Willy dead?
    if (kill_willy) {
        return true;
    } else {
        return false;
    }
}

// Check and set the attribute byte for a cell occupied by Willy's sprite
//
// C=15 or Willy's pixel y-coordinate
// HL Address of the cell in the attribute buffer at 23552
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool WILLYATTR(uint16_t addr, uint8_t pix_y) {
    // Does this cell contain a background tile?
    if (speccy.memory[addr] == cavern.BACKGROUND.id) {
        // Set the zero flag if we are going to retain the INK colour in this cell;
        // this happens only if the cell is in the bottom row and Willy's sprite
        // is confined to the top two rows.

        // Jump if we are going to retain the current INK colour in this cell
        if (pix_y == 15) {
            // Pick up the attribute byte of the BACKGROUND tile.
            // Set bits 0-2, making the INK white
            // Set the attribute byte for this cell in the buffer at 23552
            speccy.memory[addr] = (uint8_t) (cavern.BACKGROUND.id | 7);
        }
    }

    // Pick up the attribute byte of the nasty tiles for the current cavern
    // Has Willy hit a NASTY1 or NASTY2 kind? Kill Willy if so.
    if (speccy.memory[addr] == cavern.NASTY1.id || speccy.memory[addr] == cavern.NASTY2.id) {
        KILLWILLY();
        return true;
    }

    return false;
}

// Draw Willy to the screen buffer at 24576
void DRAWWILLY() {
    // Pick up Willy's pixel y-coordinate from PIXEL_Y so we can
    // point to the entry in SBUFADDRS (the screen buffer address
    // lookup table) that corresponds to Willy's y-coordinate
    uint8_t pix_y = (uint8_t) (willy.PIXEL_Y / 2);

    // Pick up Willy's direction and movement flags from DMFLAGS
    // Now 0 if Willy is facing right, or 128 if he's facing left
    uint8_t frame = rotr((uint8_t) (willy.DMFLAGS & 1), 1);

    // Pick up Willy's current animation frame (0-3) (see MANDAT)
    frame = rotr((uint8_t) (willy.FRAME & 3), 3) | frame;

    // Pick up Willy's screen x-coordinate (0-31) from LOCATION.
    uint8_t msb_dummy, pix_x;
    split_address((uint16_t) (willy.LOCATION & 31), &msb_dummy, &pix_x);

    // There are 16 rows of pixels in a sprite
    uint8_t h, l;
    for (int i = 0; i < 16; i++) {
        // Set to the address in the screen buffer at 24576 that corresponds
        // to where we are going to draw the next pixel row of the sprite graphic
        split_address(SBUFADDRS[pix_y], &h, &l);
        uint16_t addr = build_address(h, l | pix_x);

        // Merge the sprite byte with the background.
        speccy.memory[addr] = willy.sprites[frame] | speccy.memory[addr];

        // Move HL along to the next cell to the right
        addr++;

        // Point to the next sprite byte
        frame++;

        // Merge the sprite byte with the background.
        speccy.memory[addr] = willy.sprites[frame] | speccy.memory[addr];

        // Point to the next entry in the screen buffer address lookup table at SBUFADDRS
        pix_y += 1;

        // Point to the next sprite byte
        frame++;
    }
}

// Play the theme tune (The Blue Danube)
//
// Returns with the zero flag reset if ENTER or the fire button
// is pressed while the tune is being played.
//
// IY THEMETUNE (tune data)
bool PLAYTUNE() {
    // FIXME: no need to play the tune just yet, so return early :)
    if (true) {
        return false;
    }

    uint8_t freq1, freq2;
    uint16_t addr;

    for (int i = 0; i < 95; i++) {
        uint8_t *note = THEMETUNE[i];

        // Pick up the second byte of data for this note. Copy it to A
        freq1 = note[1];

        // Calculate the attribute file address for the corresponding piano key
        addr = PIANOKEY(freq1);

        // Set the attribute byte for the piano key to 80 (INK 0: PAPER 2: BRIGHT 1)
        speccy.memory[addr] = 80;

        // Pick up the third byte of data for this note
        freq2 = note[2];

        // Copy it to A
        uint8_t pitch = freq2;

        // Calculate the attribute file address for the corresponding piano key
        addr = PIANOKEY(freq2);

        // Set the attribute byte for the piano key to 40 (INK 0: PAPER 5: BRIGHT 0)
        speccy.memory[addr] = 40;

        for (uint8_t d = note[0]; d > 0; d--) {
            // Produce a sound based on the frequency parameters in the second
            // and third bytes of data for this note (copied into D and E)
            OUT(pitch);

            freq1--;
            if (freq1 == 0) {
                freq1 = note[1];
                pitch ^= 24;
            }
            freq2--;
            if (freq2 == 0) {
                freq2 = note[2];
                pitch ^= 24;
            }

            // FIXME: reg B (delay) is initialized to 0, but never set anywhere else, so this code is obsolete...?
            millisleep(1);
        }

        // Check whether ENTER or the fire button is being pressed
        if (Terminal_getKey() == MM_KEY_ENTER) {
            return true;
        }

        // Pick up the second byte of data for this note
        // Calculate the attribute file address for the corresponding piano key
        addr = PIANOKEY(note[1]);

        // Set the attribute byte for the piano key back to 56 (INK 0: PAPER 7: BRIGHT 0)
        speccy.memory[addr] = 56;

        // Pick up the third byte of data for this note
        // Calculate the attribute file address for the corresponding piano key
        addr = PIANOKEY(note[2]);

        // Set the attribute byte for the piano key back to 56 (INK 0: PAPER 7: BRIGHT 0)
        speccy.memory[addr] = 56;
    }

    return false;
}

// Calculate the attribute file address for a piano key
// Returns with the attribute file address in HL.
// A Frequency parameter from the tune data table at THEMETUNE
uint16_t PIANOKEY(uint8_t frequency) {
    // Compute the piano key index (K) based on the frequency parameter (F),
    // and store it in bits 0-4 of A: K=31-INT((F-8)/8)
    frequency -= 8;
    frequency = rotr(frequency, 3);
    frequency = (uint8_t) ~frequency;

    // A=224+K; this is the LSB
    frequency |= 224;

    // Set HL to the attribute file address for the piano key
    return build_address(89, frequency);
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
        Speccy_writeAttribute(addr + i, Data_cavernLayouts[19][i]);
    }

    // Copy LOWERATTRS to the bottom two-thirds of the attribute file
    addr += 256;
    for (int i = 0; i < 512; i++) {
        Speccy_writeAttribute(addr + i, LOWERATTRS[i]);
    }

    Terminal_redraw();

    // And finally, play the theme tune and check for key presses.

    // Play the theme tune -- start game if ENTER/FIRE button was pressed
    if (PLAYTUNE()) {
        game.DEMO = 0;
        return;
    }

    // Scroll intro message across the screen
    for (int pos = 0; game.DEMO > 0 && pos < 224; pos++) {
        // Print 32 characters of the message at 20576 (19,0)
        Speccy_printMessage(&MESSINTRO[pos], 20576, 32);

        // Keep only bits 1 and 2, and move them into bits 6 and 7,
        // so that A holds 0, 64, 128 or 192;
        // this value determines the animation frame to use for Willy
        uint8_t sprite_id = rotr((uint8_t) (pos & 6), 3);

        // Draw Willy at 18493 (9,29)
        DRWFIX(&willy.sprites[sprite_id], 18493, 0);

        Terminal_redraw();

        // Pause for about 0.1s
        millisleep(30);
        Speccy_tick();

        // Is ENTER being pressed? If so, start the game
        if ( Terminal_getKey() == MM_KEY_ENTER) {
            game.DEMO = 0;
            return;
        }
    }
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
        // Pick up the in-game music note index from NOTEINDEX;
        // this will determine the animation frame for the Willy sprites
        // Now A=0 (frame 0), 32 (frame 1), 64 (frame 2) or 96 (frame 3)
        uint8_t anim_frame = (uint8_t) (rotl(game.NOTEINDEX, 3) & 96);

        uint8_t *sprite = &willy.sprites[anim_frame];

        // C=0; this tells DRWFIX to overwrite any existing graphics
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

void printScores() {
    // Print the score (see SCORBUF) at (19,26)
    Speccy_printMessage(&game.SCORBUF, 20602, 6);

    // Print the high score (see HGHSCOR) at (19,11)
    Speccy_printMessage(&game.HGHSCOR, 20587, 6);
}

void playGameMusic() {
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

void copyScrBufToDisplayFile() {
    // Copy the contents of the screen buffer at 24576 to the display file
    // FIXME: all good, uses the Display File
    for (int i = 0; i < 4096; i++) {
        Speccy_writeScreen(16384 + i, speccy.memory[24576 + i]);
    }
}

void copyAttrBufToAttrFile() {
    // Copy the contents of the attribute buffer at 23552 to the attribute file
    for (int i = 0; i < 512; i++) {
        Speccy_writeAttribute(22528 + i, speccy.memory[23552 + i]);
    }
}

void resetScreenAttrBuffers() {
    // FIXME: aren't these just clearing the buffers?

    // Copy contents of attribute buffer at 24064 (the empty cavern)
    // into the attribute buffer at 23552.
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
}

int processInput() {
    int input;

    switch (Terminal_getKey()) {
        case T_KEY_SPACE:  case T_KEY_UP:
            input = MM_KEY_JUMP;
            break;
        case T_KEY_LEFT:
            input = MM_KEY_LEFT;
            break;
        case T_KEY_RIGHT:
            input = MM_KEY_RIGHT;
            break;
        case T_KEY_ENTER:
            input = MM_KEY_ENTER;
            break;
        case 'p':
        case 'P':
            input = MM_KEY_PAUSE;
            break;
        case 'Q':
            input = MM_KEY_QUIT;
            break;
        case 'm':
        case 'M':
            input = MM_KEY_MUTE;
            break;
        default:
            input = MM_KEY_NONE;
    }

    return input;
}

// Check if player is pressing movement + jump keys
int processMoveJumpInput(int firstInput) {
    int input;

    do {
        input = processInput();

        // Ignore keft/right key presses, we want JUMP keys!
        if (input == MM_KEY_LEFT || input == MM_KEY_RIGHT) { continue; }

        if (firstInput == MM_KEY_LEFT && input == MM_KEY_JUMP) {
            return MM_KEY_LEFT_JUMP;
        } else if (firstInput == MM_KEY_RIGHT && input == MM_KEY_JUMP) {
            return MM_KEY_RIGHT_JUMP;
        }
    } while (input != MM_KEY_NONE);

    return firstInput;
}
