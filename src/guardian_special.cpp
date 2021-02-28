// Manic Miner C/C++ port Copyright (c) 2016-2021 Michael R. Cook
// Manic Miner Copyright (c) 1983 Bug-Byte Ltd.

#include "headers.h"
#include "globals.h"
#include "data.h"
#include "guardian_special.h"
#include "kong_beast.h"

bool UpdateSpecialVerticalGuardians(uint8_t sheet) {
    // Eugene's Lair
    if (sheet == 4) {
        EugeneMove();
        if (EugeneDraw()) {
            return true;
        }
    }

    // Skylab Landing Bay.
    if (sheet == 13) {
        if (SKYLABS()) {
            return true;
        }
    }

    // Wacky Amoebatrons, and other regular guardians.
    if (sheet >= 8 && sheet != 13) {
        // The guardian-moving loop begins here.
        for (GuardianVertical &guardian : VGUARDS) {
            if (GuardianV_updateAndDraw(guardian)) {
                return true;
            }
        }
    }

    // Miner Willy meets the Kong Beast and Return of the Alien Kong Beast.
    if (sheet == 7 || sheet == 11) {
        if (KONGBEAST()) {
            return true;
        }
    }

    // Solar Power Generator.
    if (sheet == 18) {
        LIGHTBEAM();
    }

    return false;
}

void LIGHTBEAM() {
    // Point to the cell at (0,23) in the attribute buffer at 23552 (the source of the light beam).
    // FIXME: Screen Buffer: Screen File = 22551
    uint16_t addr = 23575;

    // Prepare DE for addition (the beam travels vertically downwards to start with).
    int16_t dir = 32; // gets toggled between 32 and -1.

    // The beam-drawing loop begins here.
    while (true) {
        // Does HL point at a floor or wall tile? Return if so (the light beam stops here).
        if (Speccy_readMemory(addr) == cavern.FLOOR.id || Speccy_readMemory(addr) == cavern.WALL.id) {
            return;
        }

        // A=39 (INK 7: PAPER 4)
        // Does HL point at a tile with this attribute value?
        // Jump if not (the light beam is not touching Willy).
        if (Speccy_readMemory(addr) == 39) {
            // Decrease the air supply by four units
            Cavern_decreaseAir();
            Cavern_decreaseAir();
            Cavern_decreaseAir();
            Cavern_decreaseAir();
            // Jump forward to GuardianH_draw the light beam over Willy.
        } else {
            // Does HL point at a background tile? Jump if so (the light beam will not be reflected at this point).
            if (Speccy_readMemory(addr) != cavern.BACKGROUND.id) {
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
        Speccy_writeMemory(addr, 119);

        // Point HL at the cell where the next portion of the light beam will be drawn.
        addr += dir;
    }
}

void EugeneMove() {
    // Have all the items been collected, or is Eugene moving downwards?.
    if (game.ITEMATTR == 0 || EUGDIR == 0) {
        // Pick up Eugene's pixel y-coordinate from EUGHGT.
        // Increment it (moving Eugene down).
        // Has Eugene reached the portal yet? Jump if so.
        if (EUGHGT + 1 == 88) {
            // Toggle Eugene's direction at EUGDIR.
            EUGDIR ^= 1;
        } else {
            // Update Eugene's pixel y-coordinate at EUGHGT.
            EUGHGT++;
        }
    } else {
        // Pick up Eugene's pixel y-coordinate from EUGHGT.
        // Decrement it (moving Eugene up)
        // Jump if Eugene has reached the top of the cavern.
        if (EUGHGT - 1 == 0) {
            // Toggle Eugene's direction at EUGDIR.
            EUGDIR ^= 1;
        } else {
            // Update Eugene's pixel y-coordinate at EUGHGT.
            EUGHGT--;
        }
    }
}

bool EugeneDraw() {
    // Pick up Eugene's pixel y-coordinate from EUGHGT.
    // Point DE at the entry in the screen buffer address lookup table at
    // SBUFADDRS that corresponds to Eugene's y-coordinate.
    // Point HL at the address of Eugene's location in the screen buffer at 24576.
    uint8_t y_coord = Speccy_rotL(uint8_t(EUGHGT & 127), 1);
    uint16_t addr = uint16_t(SBUFADDRS[y_coord / 2] | 15);

    uint8_t msb, lsb;
    Speccy_splitAddress(addr, msb, lsb);

    addr = SBUFADDRS[(y_coord + 1) / 2];

    uint8_t _lsb;
    Speccy_splitAddress(addr, msb, _lsb);

    addr = Speccy_buildAddress(msb, lsb);

    // Draw Eugene to the screen buffer at 24576.
    bool kill_willy = DRWFIX(&EUGENEG, addr, 1);

    // Kill Willy if Eugene collided with him.
    if (kill_willy) {
        Willy_kill();
        return true;
    }

    // Pick up Eugene's pixel y-coordinate from EUGHGT.
    // Point HL at the address of Eugene's location in the attribute buffer at 23552.
    y_coord = (uint8_t) (Speccy_rotL(uint8_t(EUGHGT & 120), 1) | 7);

    // IMPORTANT: SCF should set the carry flag, and the following RL loads that into bit 0 -MRC-
    msb = 92;
    if (y_coord >> 7 == 1) {
        msb = 93;
    }
    y_coord = uint8_t(Speccy_rotL(y_coord, 1) | (1 << 0));

    addr = Speccy_buildAddress(msb, y_coord);

    // Assume we will GuardianH_draw Eugene with white INK.
    uint8_t ink_colour = 7;

    // Pick up the attribute of the last item drawn from ITEMATTR.
    // Set the zero flag if all the items have been collected.
    // Jump if there are items remaining to be collected.
    if (game.ITEMATTR == 0) {
        // Pick up the value of the game clock at CLOCK.
        // Move bits 2-4 into bits 0-2 and clear the other bits; this value
        // (which decreases by one on each pass through the main loop) will
        // be Eugene's INK colour.
        ink_colour = uint8_t(Speccy_rotR(cavern.CLOCK, 2) & 7);
    }

    UpdateGuardianColourAttributes(addr, ink_colour);

    return false;
}

void UpdateGuardianColourAttributes(uint16_t addr, uint8_t ink_colour) {
    // Pick up the attribute byte of the background tile for the current cavern.
    // Combine its PAPER colour with the chosen INK colour.
    ink_colour = uint8_t((cavern.BACKGROUND.id & 248) | ink_colour);

    // Set the attribute byte for the top-left cell of the sprite in the attribute buffer at 23552.
    Speccy_writeMemory(addr, ink_colour);

    // Set the attribute byte for the top-right cell of the sprite in the attribute buffer at 23552.
    addr++;
    Speccy_writeMemory(addr, ink_colour);

    // Set the attribute byte for the middle-left cell of the sprite in the attribute buffer at 23552.
    addr += 31;
    Speccy_writeMemory(addr, ink_colour);

    // Set the attribute byte for the middle-right cell of the sprite in the attribute buffer at 23552.
    addr++;
    Speccy_writeMemory(addr, ink_colour);

    // Set the attribute byte for the bottom-left cell of the sprite in the attribute buffer at 23552.
    addr += 31;
    Speccy_writeMemory(addr, ink_colour);

    // Set the attribute byte for the bottom-right cell of the sprite in the attribute buffer at 23552.
    addr++;
    Speccy_writeMemory(addr, ink_colour);
}

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

        // Now that the Skylab's movement has been dealt with, time to GuardianH_draw it.

        // Pickup the entry in the screen buffer address lookup table at SBUFADDRS
        // that corresponds to the Skylab's pixel y-coordinate.
        uint8_t y_coord = Speccy_rotL(VGUARDS[i].yCoord, 1);

        // Point HL at the address of the Skylab's location in the screen buffer at 24576.
        addr = SBUFADDRS[y_coord / 2];
        addr += VGUARDS[i].xCoord;

        Speccy_splitAddress(addr, msb, lsb);
        y_coord++;

        addr = SBUFADDRS[y_coord / 2];

        uint8_t y_msb, y_lsb;
        Speccy_splitAddress(addr, y_msb, y_lsb);
        addr = Speccy_buildAddress(y_msb, lsb);

        // Pick up the animation frame (0-7). Multiply it by 32.
        // Skylab sprite (at GGDATA+A).
        uint8_t sprite_offset = Speccy_rotR(VGUARDS[i].frame, 3);

        // Draw the Skylab to the screen buffer at 24576.
        bool kill_willy = DRWFIX(&VGUARDS[i].GGDATA[sprite_offset], addr, 1);

        // Kill Willy if the Skylab collided with him.
        if (kill_willy) {
            Willy_kill();
            return true;
        }

        // Point HL at the address of the Skylab's location in the attribute buffer at 23552.
        msb = uint8_t(Speccy_rotL((uint8_t) (VGUARDS[i].yCoord & 64), 2) + 92);
        lsb = uint8_t(Speccy_rotL(VGUARDS[i].yCoord, 2) & 224);
        lsb |= VGUARDS[i].xCoord;
        addr = Speccy_buildAddress(msb, lsb);

        // Set the attribute bytes for the Skylab.
        UpdateGuardianColourAttributes(addr, VGUARDS[i].attribute);

        // The current guardian definition has been dealt with. Time for the next one.
    }

    return false; // Willy is not dead!
}
