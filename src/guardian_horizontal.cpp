// Manic Miner C/C++ port Copyright (c) 2016-2021 Michael R. Cook
// Manic Miner Copyright (c) 1983 Bug-Byte Ltd.

#include "headers.h"
#include "globals.h"

// Move the horizontal guardians in the current cavern.
bool GuardianH_update(GuardianHorizontal &guardian) {
    // NOTE: the HGUARDS data array has been changed so that we
    // can now stop iterating on either 255 or 0 values.
    if (guardian.speedColour == 255 || guardian.speedColour == 0) {
        return true;
    }

    uint8_t current_clock = cavern.CLOCK;
    current_clock &= 4;
    current_clock = Speccy_rotR(current_clock, 3);

    // Jump to consider the next guardian if this one is not due to be moved on this pass.
    if ((current_clock & guardian.speedColour)) {
        return false;
    }

    // The guardian will be moved on this pass.
    // Pick up the current animation frame (0-7).

    // Is it 3 (the terminal frame for a guardian moving right)?
    // If so move the guardian right across a cell boundary or turn it round.
    uint8_t msb, lsb;
    if (guardian.frame == 3) {
        // Pick up the LSB of the address of the guardian's location in the attribute buffer at 23552.
        Speccy_splitAddress(guardian.attributeAddress, msb, lsb);

        // Has the guardian reached the rightmost point in its path?
        if (lsb == guardian.addressRightLSB) {
            // Set the animation frame to 7 (turning the guardian round to face left).
            guardian.frame = 7;
            // Jump forward to consider the next guardian.
        } else {
            // Set the animation frame to 0 (the initial frame for a guardian moving right).
            guardian.frame = 0;
            // Increment the guardian's x-coordinate (moving it right across a cell boundary).
            lsb++;
            guardian.attributeAddress = Speccy_buildAddress(msb, lsb);
            // Jump forward to consider the next guardian.
        }

        // Is the current animation frame 4 (the terminal frame for a guardian moving left)?
        // If so move the guardian left across a cell boundary or turn it round.
    } else if (guardian.frame == 4) {
        // Pick up the LSB of the address of the guardian's location in the attribute buffer at 23552.
        Speccy_splitAddress(guardian.attributeAddress, msb, lsb);

        // Has the guardian reached the leftmost point in its path?
        if (lsb == guardian.addressLeftLSB) {
            // Set the animation frame to 0 (turning the guardian round to face right).
            guardian.frame = 0;
        } else {
            // Set the animation frame to 7 (the initial frame for a guardian moving left).
            guardian.frame = 7;
            // Decrement the guardian's x-coordinate (moving it left across a cell boundary).
            lsb--;
            guardian.attributeAddress = Speccy_buildAddress(msb, lsb);
        }
    } else if (guardian.frame > 4) {
        // the animation frame is 5, 6 or 7.

        // Decrement the animation frame (this guardian is moving left).
        guardian.frame--;
    } else {
        // Increment the animation frame (this guardian is moving right).
        guardian.frame++;
    }

    return false;
}

// Draw the horizontal guardians in the current cavern,
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool GuardianH_draw(GuardianHorizontal &guardian) {
    // NOTE: the HGUARDS data array has been changed so that we can
    // skip processing this guardian on either 255 or 0 values.

    // Have we dealt with all the guardians yet?
    // Is this guardian definition blank? If so, skip it and consider the next one.
    if (guardian.speedColour == 255 || guardian.speedColour == 0) {
        return false;
    }

    // Point HL at the address of the guardian's location in the attribute buffer at 23552,
    uint16_t addr;
    addr = guardian.attributeAddress;

    // Reset bit 7 (which specifies the animation speed) of the attribute byte, ensuring no FLASH,
    auto attr = uint8_t(guardian.speedColour & 127);

    // Set the attribute bytes for the guardian in the buffer at 23552,
    Speccy_writeMemory(addr, attr);
    addr++;
    Speccy_writeMemory(addr, attr);
    addr += 31;
    Speccy_writeMemory(addr, attr);
    addr++;
    Speccy_writeMemory(addr, attr);

    // Pick up the animation frame (0-7),
    auto anim_frame = uint8_t(guardian.frame);
    // Multiply it by 32,
    anim_frame = Speccy_rotR(anim_frame, 3);

    // If we are not in one of the first seven caverns, The Endorian Forest, or The Sixteenth Cavern...
    if (cavern.SHEET >= 7 && cavern.SHEET != 9 && cavern.SHEET != 15) {
        // Add 128 to E (the horizontal guardians in this cavern use frames 4-7 only),
        anim_frame |= (1 << 7);
    }

    // Point DE at the graphic data for the appropriate guardian sprite (at GGDATA+E),
    // Point HL at the address of the guardian's location in the screen buffer at 24576,
    uint8_t msb, lsb;
    Speccy_splitAddress(guardian.attributeAddress, msb, lsb);
    addr = Speccy_buildAddress((uint8_t) guardian.addressMSB, lsb);

    // Draw the guardian to the screen buffer at 24576,
    bool kill_willy = DRWFIX(&GGDATA[anim_frame], addr, 1);

    // Kill Willy if the guardian collided with him,
    if (kill_willy) {
        Willy_kill();
        return true;
    }

    return false; // IMPORTANT Willy has not died! -MRC-
}
