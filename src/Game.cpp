// Manic Miner C Port Copyright 2017 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.

#include "Headers.h"
#include "Data.h"
#include "Globals.h"
#include "Helpers.h"
#include "Window.h"
#include "Sound.h"
#include "KongBeast.h"

static bool gameIsRunning = true;

void Game_initialize(bool cheat, int teleport) {
    Willy_loadSprites();

    game.CHEAT = cheat;

    if (teleport >= 0) {
        game.teleportMode = true;
        game.teleportCavernNumber = teleport;
    }

    strcpy(game.airLabel, "AIR");

    // Scores text 'High Score 000000   Score 000000'
    strcpy(game.MESSHSSC, "High Score 000000   Score 000000");

    strcpy(game.MESSG, "Game");
    strcpy(game.MESSO, "Over");
}

void Game_clearBuffers() {
    // Screen buffers
    for (int i = 0; i < 4096; i++) {
        // Screen Buffer
        speccy.writeMemory(24576 + i, 0);
        // Empty Screen Buffer
        speccy.writeMemory(28672 + i, 0);
    }

    // Attributes Buffers
    for (int i = 0; i < 512; i++) {
        // Attributes Buffer
        speccy.writeMemory(23552 + i, 0);
        // Empty Attributes Buffer
        speccy.writeMemory(24064 + i, 0);
    }
}

// Play the game already!
// Returning `true` quits the application.
bool Game_play() {
    // We should reset the current cavern data. This is used in place of those `goto NEWSHT`.
    static bool reinitialiseCavern = true;

    if (game.teleportMode) {
        cavern.SHEET = (uint8_t) game.teleportCavernNumber;
    } else {
        cavern.SHEET = 0;
    }

    // Initialise the in-game music note index.
    game.NOTEINDEX = 0;

    // Initialise the screen flash counter.
    game.FLASH = 0;

    // Initialise in game music play state.
    game.playMusic = true;

    // Initialise the current game score.
    resetCurrentScore();

    EUGDIR = 0;
    EUGHGT = 0;

    // Prepare the screen; clear the entire Spectrum display file.
    speccy.clearDisplayFile();

    // Store the keyboard input for use within the loop.
    int keyIntput;

    // Yup, the game has started successfully.
    gameIsRunning = true;

    // The Main Loop
    while (gameIsRunning) {
        keyIntput = processInput();

        if (reinitialiseCavern) {
            loadCurrentCavern();
            reinitialiseCavern = false;
        }

        // The first thing to do is check whether there are any
        // remaining lives to draw at the bottom of the screen.
        drawRemainingLives();

        // Next, prepare the screen and attribute buffers for drawing to the screen.
        resetScreenAttrBuffers();

        // Check to see if the user muted the in-game music, or paused, or wants to quit.
        switch (keyIntput) {
            case Keyboard::MM_KEY_MUTE:
                game.playMusic = !game.playMusic;
                break;
            case Keyboard::MM_KEY_QUIT:
                return true; // return true so we quit the game!
            case Keyboard::MM_KEY_PAUSE:
                keyIntput = Keyboard::MM_KEY_NONE;

                do {
                    millisleep(25); // keep the FPS under control.
                } while (processInput() != Keyboard::MM_KEY_PAUSE);

                break;
            default:;
        }

        // Move the horizontal guardians in the current cavern.
        // The guardian-moving loop begins here.
        for (GuardianHorizontal &guardian : HGUARDS) {
            if (guardian.update()) {
                break;
            }
        }

        // Are we in demo mode?
        // If not, check and set the attribute bytes for Willy's sprite in the
        // buffer at 23552, and draw Willy to the screen buffer at 24576.
        if (game.DEMO == 0) {
            // Move Willy
            if (MOVEWILLY(keyIntput)) {
                goto LOOP_4; // Willy has died!
            }
            // Draw Willy
            if (Willy_checkAttrs()) {
                goto LOOP_4; // Willy has died!
            }
        }

        // Draw the horizontal guardians in the current cavern.
        // The guardian-drawing loop begins here.
        for (GuardianHorizontal &guardian : HGUARDS) {
            if (guardian.draw()) {
                goto LOOP_4; // Willy has died!
            }
        }

        // Move the conveyor in the current cavern.
        cavern.moveConveyorBelts();

        // Draw the items in the current cavern and collect any that Willy is touching.
        DRAWITEMS();

        // Handle the different vertical guardians.

        // Eugene's Lair
        if (cavern.SHEET == 4) {
            if (EUGENE()) {
                goto LOOP_4; // Willy has died!
            }
        }
        // Skylab Landing Bay.
        if (cavern.SHEET == 13) {
            if (SKYLABS()) {
                goto LOOP_4; // Willy has died!
            }
        }
        // Wacky Amoebatrons, and other regular guardians.
        if (cavern.SHEET >= 8 && cavern.SHEET != 13) {
            // The guardian-moving loop begins here.
            for (GuardianVertical &guardian : VGUARDS) {
                if (guardian.updateAndDraw()) {
                    goto LOOP_4; // Willy has died!
                }
            }
        }
        // Miner Willy meets the Kong Beast and Return of the Alien Kong Beast.
        if (cavern.SHEET == 7 || cavern.SHEET == 11) {
            if (KONGBEAST()) {
                goto LOOP_4; // Willy has died!
            }
        }
        // Solar Power Generator.
        if (cavern.SHEET == 18) {
            LIGHTBEAM();
        }

        // FIXME: this should be moved up, directly after moving Willy...?
        // Draw the portal, or move to the next cavern if Willy has entered it.
        if (CHKPORTAL()) {
            reinitialiseCavern = true;
            continue;
        }

        // Okay, ugly goto! We should try to remove this.
        LOOP_4:
        copyScrBufToDisplayFile();

        Window::instance().redraw();

        // this also redraws the screen.
        flashScreen();

        copyAttrBufToAttrFile();

        printScores();

        Window::instance().redraw();

        // Decrease the air remaining in the current cavern.
        bool depletedAir = cavern.decreaseAir();

        // Has willy ran our of air or had a fatal accident?
        // Has Willy landed after falling from too great a height, or collided with a nasty or a guardian?
        if (depletedAir || willy.AIRBORNE == 255) {
            if (MANDEAD()) {
                reinitialiseCavern = true;
                gameIsRunning = false;
                break;
                // return false; // goto START, and don't quit!
            } else {
                reinitialiseCavern = true;
                continue;
            }
        }

        // Play some music, unless in-game music been switched off.
        if (game.playMusic) {
            playGameMusic();
        }

        // If we're in demo mode, return to the title screen if user presses the enter key.
        if (game.DEMO > 0) {
            // We're in demo mode; is it time to show the next cavern?
            if (game.DEMO == 1) {
                if (MANDEAD()) {
                    reinitialiseCavern = true;
                    return false; // goto START, and don't quit!
                }
                reinitialiseCavern = true;
                continue;
            }

            // Update the game mode indicator at DEMO.
            game.DEMO--;

            if (keyIntput == Keyboard::MM_KEY_ENTER) {
                reinitialiseCavern = true;
                gameIsRunning = false;
                break;
                // return false; // goto START, and don't quit!
            }
        }

        speccy.tick();
    }

    // return, but don't quit!
    return false;
}

void resetCurrentScore() {
    game.playerScore = 0;
}

void Game_scoreAdd(int amount) {
    game.playerScore += amount;
    awardExtraLife();
}

// Load/Reload the cavern data for the current SHEET (was NEWSHT)
// This function is called when:
//   * teleporting into a cavern
//   * reinitialising the current cavern after Willy has lost a life)
//   * after NXSHEET.
void loadCurrentCavern() {
    // Copy the cavern definition into the game status buffer at 32768.
    if (!cavern.loadData(cavern.SHEET)) {
        // Oops! We've not loaded the right amount of cavern data into memory.
        Window::instance().quit();
        exit(-1);
    }

    //
    // Now, draw the screen
    //

    // Copy the cavern's attribute bytes into the buffer at 24064.
    // FIXME: uses Attr Buff (Blank): Attr File = 22528
    for (int i = 0; i < 512; i++) {
        speccy.writeMemory(24064 + i, cavern.layout[i]);
    }

    // Draw the current cavern to the screen buffer at 28672.
    DRAWSHEET();

    speccy.clearBottomThirdOfDisplayFile();

    // Print the cavern name at 20480 (16,0).
    speccy.printString(cavern.CAVERNNAME, 20480, 32);

    // Print 'AIR' label at 20512 (17,0).
    speccy.printString(&game.airLabel, 20512, 3);

    cavern.drawAirBar();

    // Print scores text at 20576 (19,0).
    speccy.printString(&game.MESSHSSC, 20576, 32);

    // Set the border colour.
    Speccy::OUT(cavern.BORDER);

    Window::instance().redraw();

    // Are we in demo mode?
    if (game.DEMO > 0) {
        // Reset the game mode indicator (we're in demo mode).
        game.DEMO = 64;
    }
}

void flashScreen() {
    if (game.FLASH > 0) {
        game.FLASH--;

        // Move bits 0-2 into bits 3-5 and clear all the other bits.
        uint8_t new_flash_value = (uint8_t) (Speccy::rotL(game.FLASH, 3) & 56);

        // Set every attribute byte in the buffer at 23552 to this value.
        // FIXME: Uses Attr Buffer: Attr File= 22528
        for (int i = 0; i < 512; i++) {
            speccy.writeMemory(23552 + i, new_flash_value);
        }

        Window::instance().redraw();
    }
}

// Willy is Dead!
//   * The air in the cavern has run out;
//   * or Willy has had a fatal accident;
//   * or it's demo mode and it's time to show the next cavern.
// IMPORTANT: return `true` is `goto START`, `false` is `goto NEWSHT`
bool MANDEAD() {
    // If in demo mode move to the next cavern.
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
    // Decrement `attr` (effectively decrementing the INK colour).
    // Have we used attribute value 64 (INK 0) yet?
    // Update the INK colour in the top two thirds of the screen and make another sound effect.
    for (uint8_t attr = 71; attr > 64; attr--) {
        speccy.fillTopTwoThirdsOfAttributeFileWith(attr);

        // D=63-8*(E AND 7).
        // This value determines the pitch of the short note that will be played.
        pitch = ~attr;
        pitch = (uint8_t) (pitch & 7);
        pitch = Speccy::rotL(pitch, 3);
        pitch |= 7;

        // C=8+32*(E AND 7).
        // This value determines the duration of the short note that will be played.
        duration = attr;
        duration = Speccy::rotR(duration, 3);
        // Set bit 4 of A (for no apparent reason).
        duration |= 16;

        // Set A=0 (this will make the border black).
        uint8_t border = 0;

        speccy.setBorderColour(border);
        speccy.makeSound(pitch, duration, 1);
    }

    // Finally, check whether any lives remain.
    if (willy.NOMEN < 1) {
        // If not, display the game over sequence.
        ENDGAM();
        return true; // goto START;
    }

    // Decrease the number of lives remaining by one.
    if (!game.CHEAT) {
        willy.NOMEN--;
    }

    // Jump back to reinitialise the current cavern.
    return false; // goto NEWSHT;
}

// Display the game over sequence
void ENDGAM() {
    // Check if we have a new high score.
    if (game.playerScore > game.highScore) {
        game.highScore = game.playerScore;
    }

    // Now prepare the screen for the game over sequence.

    speccy.clearTopTwoThirdsOfDisplayFile();
    Window::instance().redraw();

    // determines the distance of the boot from the top of the screen.
    uint8_t bootDistanceFromTop = 0;

    // Draw Willy at 18575 (12,15) using frame 3.
    DRWFIX(&willy.sprites[64], 18575, 0);

    // Draw the plinth (see PLINTH) underneath Willy at 18639 (14,15).
    DRWFIX(&PLINTH, 18639, 0);

    Window::instance().redraw();

    uint8_t msb, lsb;
    uint16_t addr;

    // The following loop draws the boot's descent onto the plinth that supports Willy.
    for (bootDistanceFromTop = 0; bootDistanceFromTop < 98; bootDistanceFromTop += 4) {
        Speccy::splitAddress(SBUFADDRS[bootDistanceFromTop], &msb, &lsb);

        // center of screen
        addr = Speccy::buildAddress((uint8_t) (msb - 32), (uint8_t) (lsb | 15));

        // Draw the boot (see BOOT) at this location, without erasing the boot
        // at the previous location; this leaves the portion of the boot sprite
        // that's above the ankle in place, and makes the boot appear as if
        // it's at the end of a long, extending trouser leg.
        DRWFIX(&BOOT, addr, 0);

        // Pick up the distance variable from EUGHGT  (A=255-A).
        uint8_t distance = (uint8_t) (255 - bootDistanceFromTop);

        // Store this value (63-255) in E; it determines the (rising) pitch of
        // the sound effect that will be made.
        uint8_t border = 0; // (black border)
        speccy.setBorderColour(border);

        // C=64; this value determines the duration of the sound effect
        // speccy.makeSound(border, 64, (uint8_t)(distance / 216));

        // FIXME: delay would be in makeSound (which is wrong anyway),
        // so we must delay to get the correct boot descending effect.
        millisleep(distance >> 4);

        // Keep only bits 2 and 3.
        distance = (uint8_t) (bootDistanceFromTop & 12);
        // Shift bits 2 and 3 into bits 3 and 4; these bits determine the PAPER colour: 0, 1, 2 or 3.
        distance = Speccy::rotL(distance, 1);
        // Set bits 0-2 (INK 7) and 6 (BRIGHT 1).
        distance |= 71;

        speccy.fillTopTwoThirdsOfAttributeFileWith(distance);
        Window::instance().redraw();

        speccy.tick();
    }

    // Now print the "Game Over" message, just to drive the point home.
    speccy.printString(&game.MESSG, 16586, 4);
    speccy.printString(&game.MESSO, 16594, 4);

    // The following loop makes the "Game Over" message glisten for about 1.57s.
    // The counter will also determine the INK colours to use for the "Game Over" message.
    for (int d = 6; d > 0; d--) {
        // Delay for about a millisecond.
        millisleep(1);

        for (int a = 0; a < 8; a++) {
            // LD A,C                  // Change the INK colour of the "G" in "Game" at (6,10)
            // AND 7
            // OR 64
            // LD (22730),A
            // INC A                   // Change the INK colour of the "a" in "Game" at (6,11)
            // AND 7
            // OR 64
            // LD (22731),A
            // INC A                   // Change the INK colour of the "m" in "Game" at (6,12)
            // AND 7
            // OR 64
            // LD (22732),A
            // INC A                   // Change the INK colour of the "e" in "Game" at (6,13)
            // AND 7
            // OR 64
            // LD (22733),A
            // INC A                   // Change the INK colour of the "O" in "Over" at (6,18)
            // AND 7
            // OR 64
            // LD (22738),A
            // INC A                   // Change the INK colour of the "v" in "Over" at (6,19)
            // AND 7
            // OR 64
            // LD (22739),A
            // INC A                   // Change the INK colour of the "e" in "Over" at (6,20)
            // AND 7
            // OR 64
            // LD (22740),A
            // INC A                   // Change the INK colour of the "r" in "Over" at (6,21)
            // AND 7
            // OR 64
            // LD (22741),A

            // IMPORTANT: haha, and you think this is correct? -MRC-
            speccy.writeAttribute(22730 + a, (uint8_t) (((d + a) & 7) | 64));
            Window::instance().redraw();
        }
    }

    // IMPORTANT: caller must return to START
}

// Draw the current cavern to the screen buffer at 28672.
void DRAWSHEET() {
    // FIXME: Screen Buffer: Screen File = 16384
    uint16_t addr = 28672;
    uint8_t msb, lsb;

    uint8_t *sprite;

    int offset = 0;
    int col;

    // The following loop draws the 512 tiles for the cavern to the screen buffer at 28672.
    // This is done for each of the 2 screen blocks (top two thirds of the screen)
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
        } else {
            sprite = NULL;
        }

        // Copy the graphic bytes to their destination cells.
        uint16_t row_addr = addr;
        for (int b = 0; b < 8; b++) {
            if (sprite != NULL) {
                speccy.writeMemory(row_addr + offset, sprite[b]);
            }
            Speccy::splitAddress(row_addr, &msb, &lsb);
            msb++;
            row_addr = Speccy::buildAddress(msb, lsb);
        }
        col++;

        Speccy::splitAddress(addr, &msb, &lsb);
        if (col == 31) {
            col = 0;
            msb++;
        }
        lsb++;
        addr = Speccy::buildAddress(msb, lsb);
    }

    // The empty cavern has been drawn to the screen buffer at 28672.
    // If we're in The Final Barrier, however, there is further work to do.
    if (cavern.SHEET == 19) {
        // Copy the graphic data from TITLESCR1 to the top half
        // of the screen buffer at 28672.
        // FIXME: Blank Screen Buffer: Screen File = 16384
        for (int i = 0; i < 2048; i++) {
            speccy.writeMemory(28672 + i, TITLESCR1[i]);
        }
    }
}

// Move Willy (1)
// This routine deals with Willy if he's jumping or falling.
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool MOVEWILLY(int keyIntput) {
    // Has Willy finished jumping?
    if (willy.AIRBORNE == 1 && Willy_updateJumpingState()) {
        // Willy has finished jumping.
        return false; // but he's not dead, so return false.
    }

    // If we get here, then
    //   * Willy is standing on the floor
    //   * Willy is falling,
    //   * JUMPING is 13: at which point Willy is on his way down and is
    //     exactly two cell-heights above where he started the jump.
    //   * JUMPING is 16: at which point Willy is on his way down and is
    //     exactly one cell-height above where he started the jump.

    uint16_t addr;

    // Does Willy's sprite occupy six cells (two rows) at the moment?
    if ((willy.PIXEL_Y & 15) == 0) {
        // Point HL at the left-hand cell below Willy's sprite.
        addr = (uint16_t) (willy.LOCATION + 64);

        // Pick up the attribute byte of the crumbling floor tile for the current cavern.
        // Does the left-hand cell below Willy's sprite contain a crumbling floor tile?
        // If so, make it crumble.
        if (speccy.readMemory(addr) == cavern.CRUMBLING.id) {
            CRUMBLE(addr);
        }

        if (speccy.readMemory(addr) == cavern.NASTY1.id) {
            // the left-hand cell below Willy's sprite contain a nasty tile.
        } else if (speccy.readMemory(addr) == cavern.NASTY2.id) {
            // the left-hand cell below Willy's sprite contain a nasty tile.
        } else {
            // Point to the right-hand cell below Willy's sprite
            addr++;

            // Pick up the attribute byte of the crumbling floor tile for the current cavern.
            // Does the right-hand cell below Willy's sprite contain a crumbling floor tile?
            if (speccy.readMemory(addr) == cavern.CRUMBLING.id) {
                CRUMBLE(addr);
            }

            if (speccy.readMemory(addr) == cavern.NASTY1.id) {
                // the right-hand cell below Willy's sprite contain a nasty tile.
            } else if (speccy.readMemory(addr) == cavern.NASTY2.id) {
                // the right-hand cell below Willy's sprite contain a nasty tile.
            } else {
                // Pick up the attribute byte of the background tile for the current cavern.
                // Set the zero flag if the right-hand cell below Willy's sprite is empty.
                // Point HL at the left-hand cell below Willy's sprite.

                // Is the right-hand cell below Willy's sprite empty?
                if (speccy.readMemory(addr) != cavern.BACKGROUND.id) {
                    return Willy_gerUserInputAndMove(keyIntput, addr);
                }
                addr--;
                // Is the left-hand cell below Willy's sprite empty?
                if (speccy.readMemory(addr) != cavern.BACKGROUND.id) {
                    return Willy_gerUserInputAndMove(keyIntput, addr);
                }
            }
        }
    }

    // Is Willy jumping?
    if (willy.AIRBORNE == 1) {
        Willy_moveInDirectionFacing();
        return false;
    }

    // If we get here, then Willy is either in the process of falling or just about to start falling.

    Willy_setToUnmoving();

    // Is Willy already falling?
    if (willy.AIRBORNE == 0) {
        // Willy has just started falling. Set the airborne status indicator at AIRBORNE to 2.
        willy.AIRBORNE = 2;

        return false;
    }

    // Increment the airborne status indicator.
    willy.AIRBORNE++;

    // D=16*A; this value determines the pitch of the falling sound effect.
    uint8_t pitch = Speccy::rotL(willy.AIRBORNE, 4);

    // Pick up the border colour for the current cavern.
    uint8_t border = cavern.BORDER;
    speccy.setBorderColour(border);

    // C=32; this value determines the duration of the falling sound effect.
    speccy.makeSound(pitch, 32, 1);

    // Add 8 to Willy's pixel y-coordinate at PIXEL_Y; this moves Willy downwards by 4 pixels.
    willy.PIXEL_Y += 8;

    Willy_adjustAttributes(willy.PIXEL_Y);

    return false;
}

// Animate a crumbling floor tile in the current cavern.
// Takes the address of the crumbling floor tile's location in the attribute buffer at 23552.
void CRUMBLE(uint16_t addr) {
    uint8_t msb, lsb;

    // Point to the bottom row of pixels of the crumbling
    // floor tile in the screen buffer at 28672.
    Speccy::splitAddress(addr, &msb, &lsb);

    msb += 27;
    msb |= 7;

    while (true) {
        // Collect the pixels from the row above.
        msb--;
        uint8_t pixel = speccy.readMemory(Speccy::buildAddress(msb, lsb));

        // Copy these pixels into the row below it. Point BC at the next row of pixels up.
        speccy.writeMemory(Speccy::buildAddress((uint8_t) (msb + 1), lsb), pixel);

        // Have we dealt with the bottom seven pixel rows of the crumbling floor tile yet?
        // If not, jump back to deal with the next one up.
        if ((msb & 7) == 0) {
            break;
        }
    }

    // Clear the top row of pixels in the crumbling floor tile.
    speccy.writeMemory(Speccy::buildAddress(msb, lsb), 0);

    // Point to the bottom row of pixels in the crumbling floor tile.
    msb += 7;

    // Pick up the bottom row of pixels. Is the bottom row clear? Return if not.
    if (speccy.readMemory(Speccy::buildAddress(msb, lsb)) != 0) {
        return;
    }

    // The bottom row of pixels in the crumbling floor tile is clear.
    // Time to put a background tile in its place.
    speccy.writeMemory(addr, cavern.BACKGROUND.id);
}

// Move and draw the light beam in Solar Power Generator.
void LIGHTBEAM() {
    // Point to the cell at (0,23) in the attribute buffer at 23552 (the source of the light beam).
    // FIXME: Screen Buffer: Screen File = 22551
    uint16_t addr = 23575;

    // Prepare DE for addition (the beam travels vertically downwards to start with).
    int16_t dir = 32; // gets toggled between 32 and -1.

    // The beam-drawing loop begins here.
    while (true) {
        // Does HL point at a floor or wall tile? Return if so (the light beam stops here).
        if (speccy.readMemory(addr) == cavern.FLOOR.id || speccy.readMemory(addr) == cavern.WALL.id) {
            return;
        }

        // A=39 (INK 7: PAPER 4)
        // Does HL point at a tile with this attribute value?
        // Jump if not (the light beam is not touching Willy).
        if (speccy.readMemory(addr) == 39) {
            // Decrease the air supply by four units
            cavern.decreaseAir();
            cavern.decreaseAir();
            cavern.decreaseAir();
            cavern.decreaseAir();
            // Jump forward to draw the light beam over Willy.
        } else {
            // Does HL point at a background tile? Jump if so (the light beam will not be reflected at this point).
            if (speccy.readMemory(addr) != cavern.BACKGROUND.id) {
                // Toggle the value in DE between 32 and -1 (and therefore the direction of the light beam between
                // vertically downwards and horizontally to the left): the light beam has hit a guardian.
                if (dir == 32) {
                    dir = -1;
                } else {
                    dir = 32;
                }
            }
        }

        // Draw a portion of the light beam with attribute value 119 (INK 7: PAPER 6: BRIGHT 1).
        speccy.writeMemory(addr, 119);

        // Point HL at the cell where the next portion of the light beam will be drawn.
        addr += dir;
    }
}

// Move and draw Eugene in Eugene's Lair.
// First we move Eugene up or down, or change his direction.
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool EUGENE() {
    // Have all the items been collected, or is Eugene moving downwards?
    if (game.ITEMATTR != 0 && EUGDIR != 0) {
        // Pick up Eugene's pixel y-coordinate from EUGHGT.
        // Decrement it (moving Eugene up)
        // Jump if Eugene has reached the top of the cavern.
        if (EUGHGT - 1 == 0) {
            // Toggle Eugene's direction at EUGDIR.
            EUGDIR = (uint8_t) !EUGDIR;
        } else {
            // Update Eugene's pixel y-coordinate at EUGHGT.
            EUGHGT--;
        }
    } else {
        // Pick up Eugene's pixel y-coordinate from EUGHGT.
        // Increment it (moving Eugene down).
        // Has Eugene reached the portal yet? Jump if so.
        if (EUGHGT + 1 == 88) {
            EUGDIR = (uint8_t) !EUGDIR;
        } else {
            // Update Eugene's pixel y-coordinate at EUGHGT.
            EUGHGT++;
        }
    }

    // Now that Eugene's movement has been dealt with, it's time to draw him.

    uint8_t msb, lsb;
    uint8_t x_msb, x_lsb;
    uint16_t addr;

    // Pick up Eugene's pixel y-coordinate from EUGHGT.
    // Point DE at the entry in the screen buffer address lookup table at
    // SBUFADDRS that corresponds to Eugene's y-coordinate.
    // Point HL at the address of Eugene's location in the screen buffer at 24576.
    uint8_t y_coord = (uint8_t) (EUGHGT & 127);
    y_coord = Speccy::rotL(y_coord, 1);
    addr = SBUFADDRS[y_coord / 2];
    addr |= 15;
    Speccy::splitAddress(addr, &msb, &lsb);
    y_coord++;
    addr = SBUFADDRS[y_coord / 2];
    Speccy::splitAddress(addr, &x_msb, &x_lsb);
    addr = Speccy::buildAddress(x_msb, lsb);

    // Draw Eugene to the screen buffer at 24576.
    bool kill_willy = DRWFIX(&EUGENEG, addr, 1);

    // Kill Willy if Eugene collided with him.
    if (kill_willy) {
        Willy_kill();
        return true;
    }


    // Pick up Eugene's pixel y-coordinate from EUGHGT.
    // Point HL at the address of Eugene's location in the attribute buffer at 23552.
    y_coord = (uint8_t) (EUGHGT & 120);
    y_coord = Speccy::rotL(y_coord, 1);
    y_coord |= 7;

    // IMPORTANT: SCF should set the carry flag, and the following RL loads that into bit 0 -MRC-
    msb = 0;
    if ((y_coord >> 7) & 1) {
        msb = 1;
    }
    y_coord = Speccy::rotL(y_coord, 1);
    y_coord |= 1 << 0;
    msb += 92;
    addr = Speccy::buildAddress(msb, y_coord);

    // Assume we will draw Eugene with white INK.
    uint8_t ink_colour = 7;

    // Pick up the attribute of the last item drawn from ITEMATTR.
    // Set the zero flag if all the items have been collected.
    // Jump if there are items remaining to be collected.
    if (game.ITEMATTR == 0) {
        // Pick up the value of the game clock at CLOCK.
        // Move bits 2-4 into bits 0-2 and clear the other bits; this value
        // (which decreases by one on each pass through the main loop) will
        // be Eugene's INK colour.
        ink_colour = (uint8_t) (Speccy::rotR(cavern.CLOCK, 2) & 7);
    }

    EUGENE_3(addr, ink_colour);

    return false;
}

// Sets the colour attributes for a 16x16 pixel sprite.
// SKYLABS:    to set the attributes for a Skylab.
// GuardianVertical::updateAndDraw: to set the attributes for a vertical guardian.
// KONGBEAST:  to set the attributes for the Kong Beast.
void EUGENE_3(uint16_t addr, uint8_t ink_colour) {
    // Pick up the attribute byte of the background tile for the current cavern.
    // Combine its PAPER colour with the chosen INK colour.
    // Set the attribute byte for the top-left cell of the sprite in the attribute buffer at 23552.
    speccy.writeMemory(addr, (uint8_t) ((cavern.BACKGROUND.id & 248) | ink_colour));

    // Set the attribute byte for the top-right cell of the sprite in the attribute buffer at 23552.
    addr++;
    speccy.writeMemory(addr, ink_colour);

    // Set the attribute byte for the middle-left cell of the sprite in the attribute buffer at 23552.
    addr += 31;
    speccy.writeMemory(addr, ink_colour);

    // Set the attribute byte for the middle-right cell of the sprite in the attribute buffer at 23552.
    addr++;
    speccy.writeMemory(addr, ink_colour);

    // Set the attribute byte for the bottom-left cell of the sprite in the attribute buffer at 23552.
    addr += 31;
    speccy.writeMemory(addr, ink_colour);

    // Set the attribute byte for the bottom-right cell of the sprite in the attribute buffer at 23552.
    addr++;
    speccy.writeMemory(addr, ink_colour);
}

// Move and draw the Skylabs in Skylab Landing Bay.
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool SKYLABS() {
    uint8_t msb;
    uint8_t lsb;
    uint16_t addr;

    // The Skylab-moving loop begins here.
    for (int i = 0; i < 4; i++) {
        // Have we dealt with all the Skylabs yet? If so, re-enter the main loop.
        if (VGUARDS[i].attribute == 255) {
            return false;
        }

        // Has it reached its crash site yet?
        if (VGUARDS[i].yCoord < VGUARDS[i].yCoordMaximum) {
            // Increment the Skylab's y-coordinate (moving it downwards).
            VGUARDS[i].yCoord += VGUARDS[i].yPixelIncrement;
        } else {
            // The Skylab has reached its crash site. Start or continue its disintegration.

            // Increment the animation frame.
            VGUARDS[i].frame++;

            // Pick up the animation frame.
            // Has the Skylab completely disintegrated yet?
            if (VGUARDS[i].frame == 8) {
                // Reset the Skylab's pixel y-coordinate
                VGUARDS[i].yCoord = VGUARDS[i].yCoordMinimum;

                // Add 8 to the Skylab's x-coordinate (wrapping around at the right side of the screen).
                VGUARDS[i].xCoord += 8;
                VGUARDS[i].xCoord &= 31;

                // Reset the animation frame to 0.
                VGUARDS[i].frame = 0;
            }
        }

        // Now that the Skylab's movement has been dealt with, time to draw it.

        // Pickup the entry in the screen buffer address lookup table at SBUFADDRS
        // that corresponds to the Skylab's pixel y-coordinate.
        uint8_t y_coord = Speccy::rotL(VGUARDS[i].yCoord, 1);

        // Point HL at the address of the Skylab's location in the screen buffer at 24576.
        addr = SBUFADDRS[y_coord / 2];
        addr += VGUARDS[i].xCoord;

        Speccy::splitAddress(addr, &msb, &lsb);
        y_coord++;

        addr = SBUFADDRS[y_coord / 2];

        uint8_t y_msb, y_lsb;
        Speccy::splitAddress(addr, &y_msb, &y_lsb);
        addr = Speccy::buildAddress(y_msb, lsb);

        // Pick up the animation frame (0-7). Multiply it by 32.
        // Skylab sprite (at GGDATA+A).
        uint8_t sprite_offset = Speccy::rotR(VGUARDS[i].frame, 3);

        // Draw the Skylab to the screen buffer at 24576.
        bool kill_willy = DRWFIX(&VGUARDS[i].GGDATA[sprite_offset], addr, 1);

        // Kill Willy if the Skylab collided with him.
        if (kill_willy) {
            Willy_kill();
            return true;
        }

        // Point HL at the address of the Skylab's location in the attribute buffer at 23552.
        msb = (uint8_t) (Speccy::rotL((uint8_t) (VGUARDS[i].yCoord & 64), 2) + 92);
        lsb = (uint8_t) (Speccy::rotL(VGUARDS[i].yCoord, 2) & 224);
        lsb |= VGUARDS[i].xCoord;
        addr = Speccy::buildAddress(msb, lsb);

        // Set the attribute bytes for the Skylab.
        EUGENE_3(addr, VGUARDS[i].attribute);

        // The current guardian definition has been dealt with. Time for the next one.
    }

    return false; // Willy is not dead!
}

// Draw the items in the current cavern and collect any that Willy is touching.
void DRAWITEMS() {
    uint8_t msb;
    uint8_t lsb;
    uint16_t addr;

    // Initialise the attribute of the last item drawn in ITEMATTR to 0
    // (in case there are no items left to draw).
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

        // Point DE at the address of the item's location in the attribute buffer at 23552.
        addr = cavern.ITEMS[i].address;

        // Pick up the current attribute byte at the item's location.
        // Is the INK white (which happens if Willy is touching the item)?
        if ((speccy.readMemory(addr) & 7) == 7) {
            // Willy is touching this item, so add it to his collection.

            // Add 100 to the score.
            Game_scoreAdd(100);

            // Set the item's attribute byte to 0 so that it will be skipped the next time.
            cavern.ITEMS[i].attribute = 0;

            // Jump forward to consider the next item.
        } else {
            // This item has not been collected yet.

            // Pick up the item's current attribute byte.
            // Keep the BRIGHT and PAPER bits, and set the INK to 3 (magenta).
            // Store this value in B.
            uint8_t attribute = (uint8_t) ((cavern.ITEMS[i].attribute & 248) | 3);

            // Pick up the item's current attribute byte again.
            // Keep only bits 0 and 1 and add the value in B; this maintains the BRIGHT and
            // PAPER bits, and cycles the INK colour through 3, 4, 5 and 6
            attribute += (cavern.ITEMS[i].attribute & 3);

            // Store the new attribute byte.
            cavern.ITEMS[i].attribute = attribute;

            // Update the attribute byte at the item's location in the buffer at 23552.
            speccy.writeMemory(addr, attribute);

            // Store the new attribute byte at ITEMATTR as well.
            game.ITEMATTR = attribute;

            // Point DE at the address of the item's location in the screen buffer at 24576.
            Speccy::splitAddress(addr, &msb, &lsb);
            msb = (uint8_t) cavern.ITEMS[i].addressMSB;
            addr = Speccy::buildAddress(msb, lsb);

            // Point HL at the item graphic for the current cavern (at ITEM).
            // There are eight pixel rows to copy.
            // Draw the item to the screen buffer at 24576.
            speccy.drawSprite(cavern.ITEMS[i].tile, addr, 8);
        }

        // The current item definition has been dealt with. Time for the next one.
    }

    // All the items have been dealt with. Check whether there were any left.

    // Pick up the attribute of the last item drawn at ITEMATTR.
    // Were any items drawn? Return if so (some remain to be collected).
    if (game.ITEMATTR != 0) {
        return;
    }

    // Ensure that the portal is flashing by setting bit 7 of its attribute byte at PORTAL.
    cavern.portal.PORTAL |= (1 << 7);
}

// Draw the portal, or move to the next cavern if Willy has entered it.
// First check whether Willy has entered the portal.
// IMPORTANT: the CALL-ers should be able to handle the `goto NEWSHT` themselves, on the return. -MRC-
bool CHKPORTAL() {
    uint8_t msb, lsb;
    uint8_t w_msb, w_lsb;

    // Pick up the address of the portal's location in the attribute buffer at 23552 from PORTALLOC1.
    uint16_t addr = cavern.portal.PORTALLOC1;
    Speccy::splitAddress(addr, &msb, &lsb);

    // Pick up the LSB of the address of Willy's location in the attribute buffer at 23552 from LOCATION.
    uint16_t w_addr = willy.LOCATION;
    Speccy::splitAddress(w_addr, &w_msb, &w_lsb);

    // Does it match that of the portal?
    if (lsb == w_lsb) {
        // Pick up the MSB of the address of Willy's location in the attribute buffer at 23552 from 32877.
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

    // Pick up the portal's attribute byte from PORTAL.
    // Set the attribute bytes for the portal in the buffer at 23552.
    speccy.writeMemory(addr, cavern.portal.PORTAL);
    addr++;
    speccy.writeMemory(addr, cavern.portal.PORTAL);
    addr += 31;
    speccy.writeMemory(addr, cavern.portal.PORTAL);
    addr++;
    speccy.writeMemory(addr, cavern.portal.PORTAL);

    // Point DE at the graphic data for the portal at PORTALG.
    // Pick up the address of the portal's location in the screen buffer at 24576 from PORTALLOC2.
    // C=0: overwrite mode.
    DRWFIX(&cavern.portal.PORTALG, cavern.portal.PORTALLOC2, 0);
    // This routine continues into the one at DRWFIX.

    return false; // NOTE: callers should not `goto NEWSHT`.
}

// Draw a sprite.
//
// Used by the routines at:
//   START:      draw Willy on the title screen
//   LOOP:       draw the remaining lives
//   ENDGAM:     draw Willy, the boot and the plinth during the game over sequence
//   GuardianHorizontal::draw:     draw horizontal guardians
//   EUGENE:     draw Eugene in Eugene's Lair
//   SKYLABS:    draw the Skylabs in Skylab Landing Bay
//   GuardianVertical::updateAndDraw: draw vertical guardians
//   CHKPORTAL:  draw the portal in the current cavern
//   NXSHEET:    draw Willy above ground and the swordfish graphic over the portal in The Final Barrier
//   KONGBEAST:  draw the Kong Beast in Miner Willy meets the Kong Beast and Return of the Alien Kong Beast
//
// If C=1 on entry, this routine returns with the zero flag reset if any of the
// set bits in the sprite being drawn collides with a set bit in the background.
//
// `sprite`: Address of sprite graphic data.
// `addr`:   Address to draw at.
// `mode`:   Drawing mode: 0 (overwrite) or 1 (blend).
bool DRWFIX(void *gfx_sprite, uint16_t addr, uint8_t mode) {
    uint8_t *sprite = (uint8_t *) gfx_sprite;

    uint8_t msb;
    uint8_t lsb;

    // Note: each sprite is 16x16 pixels, so we need to work in pairs of bytes.
    for (int i = 0; i < 32; i += 2) {
        // Are we in blend mode?
        if (mode == 1) {
            if (sprite[i] & speccy.readMemory(addr) || sprite[i + 1] & speccy.readMemory(addr + 1)) {
                // collision detected.
                return true;
            }
            sprite[i] |= speccy.readMemory(addr);
            sprite[i + 1] |= speccy.readMemory(addr + 1);
        }

        // Copy the graphic bytes to their destination cells.
        speccy.writeMemory(addr, sprite[i]);
        speccy.writeMemory(addr + 1, sprite[i + 1]);

        // Move down to the next pixel row
        Speccy::splitAddress(addr, &msb, &lsb);
        msb++;

        // Have we drawn the bottom pixel row in this pair of cells yet?
        if (msb & 7) {
            addr = Speccy::buildAddress(msb, lsb);
            continue;
        }

        // Otherwise move to the top pixel row in the cell below.
        msb -= 8;
        lsb += 32;

        // Was the last pair of cells at y-coordinate 7 or 15?
        // Otherwise adjust to account for the movement from the top
        // or middle third of the screen to the next one down.
        if ((lsb & 224) == 0) {
            msb += 8;
        }

        addr = Speccy::buildAddress(msb, lsb);
    }

    // no collision detected.
    return false;
}

// Move to the next cavern.
// IMPORTANT: the CALLers should be able to handle the `goto NEWSHT` themselves, on the return. -MRC-
bool NXSHEET() {
    uint16_t addr;

    // Pick up the number of the current cavern from SHEET.
    // Increment the cavern number.
    uint8_t next_sheet = (uint8_t) (cavern.SHEET + 1);

    // Is the current cavern The Final Barrier?
    if (cavern.SHEET == 19) {
        // Are we in demo mode, or cheat mode activated?
        if (game.DEMO == 0 && !game.CHEAT) {
            // Willy has made it through The Final Barrier without cheating.

            // Draw Willy at (2,19) on the ground above the portal.
            DRWFIX(&willy.sprites[96], 16467, 0);

            // Draw the swordfish graphic (see SWORDFISH) over the portal.
            DRWFIX(&SWORDFISH, 16563, 0);

            // Point HL at (2,19) in the attribute file.
            addr = 22611;

            // Set the attributes for the upper half of Willy's sprite
            // at (2,19) and (2,20) to 47 (INK 7: PAPER 5).
            speccy.writeAttribute(addr, 47);
            addr++;
            speccy.writeAttribute(addr, 47);

            // Set the attributes for the lower half of Willy's sprite
            // at (3,19) and (3,20) to 39 (INK 7: PAPER 4).
            addr += 31;
            speccy.writeAttribute(addr, 39);
            addr++;
            speccy.writeAttribute(addr, 39);

            // Point HL at (5,19) in the attribute file
            addr += 31;
            addr++;
            addr += 31;

            // Set the attributes for the fish at (5,19) and (5,20) to 69 (INK 5: PAPER 0: BRIGHT 1).
            speccy.writeAttribute(addr, 69);
            addr++;
            speccy.writeAttribute(addr, 69);

            // Set the attribute for the handle of the sword at (6,19) to 70 (INK 6: PAPER 0: BRIGHT 1).
            addr += 31;
            speccy.writeAttribute(addr, 70);

            // Set the attribute for the blade of the sword at (6,20) to 71 (INK 7: PAPER 0: BRIGHT 1).
            addr++;
            speccy.writeAttribute(addr, 71);

            // Set the attributes at (7,19) and (7,20) to 0 (to hide Willy's feet just below where the portal was).
            addr += 31;
            speccy.writeAttribute(addr, 0);
            addr++;
            speccy.writeAttribute(addr, 0);

            // Prepare C and D for the celebratory sound effect.
            uint8_t border = 0; // (black border)

            for (int i = 0; i < 50; i++) {
                // Produce the celebratory sound effect: Willy has escaped from the mine.
                Speccy::OUT(border);
                border ^= 24;

                millisleep(1);
            }
        }

        // The next cavern will be Central Cavern.
        next_sheet = 0;
    }

    // Update the cavern number at SHEET.
    if (game.teleportMode) {
        cavern.SHEET = (uint8_t) game.teleportCavernNumber;
    } else {
        cavern.SHEET = next_sheet;
    }

    // The next section of code cycles the INK and PAPER colours of the current cavern.

    // Initialise A to 63 (INK 7: PAPER 7).
    uint8_t colours = 63;

    // Iterate through all attribute values from 63 down to 1.
    for (uint8_t ink = colours; ink > 0; ink--) {
        speccy.fillTopTwoThirdsOfAttributeFileWith(ink);
        Window::instance().redraw();

        // Pause for about 0.004s
        millisleep(4);
    }

    // Are we in demo mode?
    if (game.DEMO) {
        // If so, demo the next cavern.
        return true;
    }

    // The following loop increases the score and decreases the air supply until it runs out.
    while (true) {
        Window::instance().redraw();

        // Decrease the air remaining in the current cavern.
        // Move to the next cavern if the air supply is now gone.
        if (cavern.decreaseAir()) {
            return true;
        }

        // Add 1 to the score and print at (19,26).
        Game_scoreAdd(1);
        printCurrentScore(game.playerScore);

        // C=4; this value determines the duration of the sound effect.
//        uint8_t duration = 4;

        // Pick up the remaining air supply (S) from AIR.
        // D=2*(63-S); this value determines the pitch of the sound effect
        // (which decreases with the amount of air remaining).
        // uint8_t pitch = Speccy::rotL((uint8_t) (~(cavern.AIR & 63)), 1);

//        for (int i = duration; i > 0; i--) {
//            // Produce a short note
//            Speccy::OUT(0);
//            // millisleep(pitch);
//            Speccy::OUT(24);
//            // millisleep(1);
//        }

        // Jump back to decrease the air supply again.
    }

    return false; // IMPORTANT: no `goto NEWSHT` required. -MRC-
}

void Game_play_intro() {
    // Clear the entire Spectrum display file.
    speccy.clearDisplayFile();

    // Copy TITLESCR1 and TITLESCR2 to the top two-thirds of the display file.
    for (int i = 0; i < 2048; i++) {
        speccy.writeScreen(16384 + i, TITLESCR1[i]);
    }
    for (int i = 0; i < 2048; i++) {
        speccy.writeScreen(16384 + 2048 + i, TITLESCR2[i]);
    }

    Window::instance().redraw();

    // Draw Willy at 18493 (9,29).
    DRWFIX(&willy.sprites[64], 18493, 0);

    // Copy the attribute bytes from CAVERN19 to the top third of the attribute file.
    uint16_t addr = 22528;
    for (int i = 0; i < 256; i++) {
        speccy.writeAttribute(addr + i, Data_cavernLayouts[19][i]);
    }

    // Copy LOWERATTRS to the bottom two-thirds of the attribute file.
    addr += 256;
    for (int i = 0; i < 512; i++) {
        speccy.writeAttribute(addr + i, LOWERATTRS[i]);
    }

    Window::instance().redraw();

    // And finally, play the theme tune and check for key presses.

    // Play the theme tune -- start game if ENTER/FIRE button was pressed.
    if (PLAYTUNE()) {
        game.DEMO = 0;
        return;
    }

    // Scroll intro message across the screen.
    for (int pos = 0; game.DEMO > 0 && pos < 224; pos++) {
        // Is ENTER being pressed? If so, start the game.
        if (processInput() == Keyboard::MM_KEY_ENTER) {
            game.DEMO = 0;
            return;
        }

        // Print 32 characters of the message at 20576 (19,0).
        speccy.printString((void *) &MESSINTRO[pos], 20576, 32);

        // Keep only bits 1 and 2, and move them into bits 6 and 7,
        // so that A holds 0, 64, 128 or 192;
        // this value determines the animation frame to use for Willy.
        uint8_t sprite_id = Speccy::rotR((uint8_t) (pos & 6), 3);

        // Draw Willy at 18493 (9,29).
        DRWFIX(&willy.sprites[sprite_id], 18493, 0);

        Window::instance().redraw();

        // Pause for about 0.1s
        millisleep(30);
        speccy.tick();
    }
}

void drawRemainingLives() {
    // Set address to the display file address at which to draw the first Willy sprite.
    uint16_t addr = 20640;

    // The following loop draws the remaining lives at the bottom of the screen.
    for (int i = 0; i < willy.NOMEN; i++) {
        // Pick up the in-game music note index from NOTEINDEX;
        // this will determine the animation frame for the Willy sprites.
        // Now A=0 (frame 0), 32 (frame 1), 64 (frame 2) or 96 (frame 3).
        uint8_t anim_frame = (uint8_t) (Speccy::rotL(game.NOTEINDEX, 3) & 96);

        uint8_t *sprite = &willy.sprites[anim_frame];

        // C=0; this tells DRWFIX to overwrite any existing graphics.
        DRWFIX(sprite, addr, 0);

        // Move to the location at which to draw the next Willy sprite.
        addr += 2;
    }

    // If cheat mode has been activated draw the boot at the bottom
    // of the screen next to the remaining lives.
    if (game.CHEAT) {
        DRWFIX(&BOOT, addr, 0);
    }
}

// INCSCORE_0
void awardExtraLife() {
    static int extraLivesAwarded = 0;

    // Has players' score reached a 10000s digit?
    if (game.playerScore / 10000 > extraLivesAwarded) {
        game.FLASH = 8;

        willy.NOMEN++;

        extraLivesAwarded++;
    }
}

void printScores() {
    printCurrentScore(game.playerScore);
    printHighScore(game.highScore);
}

void printCurrentScore(int currentScore) {
    assert(currentScore >= 0 && currentScore <= 999999);

    char score[7];
    sprintf(score, "%06d", currentScore);

    // Print the score (see SCORBUF) at (19,26).
    speccy.printString(&score, 20602, 6);
}

void printHighScore(int highestScore) {
    assert(highestScore >= 0 && highestScore <= 999999);

    char score[7];
    sprintf(score, "%06d", highestScore);

    // Print the high score (see HGHSCOR) at (19,11).
    speccy.printString(&score, 20587, 6);
}

void playGameMusic() {
    // Increment the in-game music note index at NOTEINDEX.
    game.NOTEINDEX++;

    // Point HL at the appropriate entry in the tune data table at GAMETUNE.
    uint8_t index = Speccy::rotR((uint8_t) (game.NOTEINDEX & 126), 1);

    // Pick up the border colour for the current cavern from BORDER.
    uint8_t note = cavern.BORDER;

    // Initialise the pitch delay counter in E.
    uint8_t pitch_delay_counter = GAMETUNE[index];

    // Initialise the duration delay counters in B (0) and C (3).
    for (int i = 0; i < 3; i++) {
        // Produce a note of the in-game music.
        Speccy::OUT(note);

        pitch_delay_counter--;

        if (pitch_delay_counter > 0) {
            pitch_delay_counter = GAMETUNE[index];
            note ^= 24;
        }

        millisleep(1);
    }
}

void copyScrBufToDisplayFile() {
    // Copy the contents of the screen buffer at 24576 to the display file.
    // FIXME: all good, uses the Display File
    for (int i = 0; i < 4096; i++) {
        speccy.writeScreen(16384 + i, speccy.readMemory(24576 + i));
    }
}

void copyAttrBufToAttrFile() {
    // Copy the contents of the attribute buffer at 23552 to the attribute file.
    for (int i = 0; i < 512; i++) {
        speccy.writeAttribute(22528 + i, speccy.readMemory(23552 + i));
    }
}

void resetScreenAttrBuffers() {
    // FIXME: aren't these just clearing the buffers?

    // Copy contents of attribute buffer at 24064 (the empty cavern)
    // into the attribute buffer at 23552.
    // FIXME: all good, uses the Display File
    for (int i = 0; i < 512; i++) {
        speccy.writeMemory(23552 + i, speccy.readMemory(24064 + i));
    }
    // Copy the contents of the screen buffer at 28672 (empty cavern tiles)
    // into the screen buffer at 24576.
    // FIXME: all good, uses the Display File
    for (int i = 0; i < 4096; i++) {
        speccy.writeMemory(24576 + i, speccy.readMemory(28672 + i));
    }
}

int processInput() {
    int input;

    switch (Window::instance().getKey()) {
        case Input::INPUT_KEY_SPACE:
            input = Keyboard::MM_KEY_JUMP;
            break;
        case Input::INPUT_KEY_LEFT:
            input = Keyboard::MM_KEY_LEFT;
            break;
        case Input::INPUT_KEY_RIGHT:
            input = Keyboard::MM_KEY_RIGHT;
            break;
        case Input::INPUT_KEY_LEFT_SPACE:
            input = Keyboard::MM_KEY_LEFT_JUMP;
            break;
        case Input::INPUT_KEY_RIGHT_SPACE:
            input = Keyboard::MM_KEY_RIGHT_JUMP;
            break;
        case Input::INPUT_KEY_RETURN:
            input = Keyboard::MM_KEY_ENTER;
            break;
        case Input::INPUT_KEY_P:
            input = Keyboard::MM_KEY_PAUSE;
            break;
        case Input::INPUT_KEY_Q:
            input = Keyboard::MM_KEY_QUIT;
            break;
        case Input::INPUT_KEY_M:
            input = Keyboard::MM_KEY_MUTE;
            break;
        default:
            input = Keyboard::MM_KEY_NONE;
    }

    return input;
}
