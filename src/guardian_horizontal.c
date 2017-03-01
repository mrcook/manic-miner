// Manic Miner C Port Copyright 2017 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.

#include "headers.h"
#include "globals.h"

// Move the horizontal guardians in the current cavern.
void GuardianHorizontal_update() {
    uint8_t msb, lsb;

    // The guardian-moving loop begins here.
    for (int i = 0; i < 4; i++) {
        if (HGUARDS[i].speedColour == 255) {
            return;
        }

        if (HGUARDS[i].speedColour == 0) {
            continue;
        }

        uint8_t current_clock = cavern.CLOCK;
        current_clock &= 4;
        current_clock = rotr(current_clock, 3);

        // Jump to consider the next guardian if this one is not due to be moved on this pass.
        if ((current_clock & HGUARDS[i].speedColour)) {
            continue;
        }

        // The guardian will be moved on this pass.
        // Pick up the current animation frame (0-7).

        // Is it 3 (the terminal frame for a guardian moving right)?
        // If so move the guardian right across a cell boundary or turn it round.
        if (HGUARDS[i].frame == 3) {
            // Pick up the LSB of the address of the guardian's location in the attribute buffer at 23552.
            split_address(HGUARDS[i].attributeAddress, &msb, &lsb);

            // Has the guardian reached the rightmost point in its path?
            if (lsb == HGUARDS[i].addressRightLSB) {
                // Set the animation frame to 7 (turning the guardian round to face left).
                HGUARDS[i].frame = 7;
                // Jump forward to consider the next guardian.
            } else {
                // Set the animation frame to 0 (the initial frame for a guardian moving right).
                HGUARDS[i].frame = 0;
                // Increment the guardian's x-coordinate (moving it right across a cell boundary).
                lsb++;
                HGUARDS[i].attributeAddress = build_address(msb, lsb);
                // Jump forward to consider the next guardian.
            }

            // Is the current animation frame 4 (the terminal frame for a guardian moving left)?
            // If so move the guardian left across a cell boundary or turn it round.
        } else if (HGUARDS[i].frame == 4) {
            // Pick up the LSB of the address of the guardian's location in the attribute buffer at 23552.
            split_address(HGUARDS[i].attributeAddress, &msb, &lsb);

            // Has the guardian reached the leftmost point in its path?
            if (lsb == HGUARDS[i].addressLeftLSB) {
                // Set the animation frame to 0 (turning the guardian round to face right).
                HGUARDS[i].frame = 0;
            } else {
                // Set the animation frame to 7 (the initial frame for a guardian moving left).
                HGUARDS[i].frame = 7;
                // Decrement the guardian's x-coordinate (moving it left across a cell boundary).
                lsb--;
                HGUARDS[i].attributeAddress = build_address(msb, lsb);
            }
        } else if (HGUARDS[i].frame > 4) {
            // the animation frame is 5, 6 or 7.

            // Decrement the animation frame (this guardian is moving left).
            HGUARDS[i].frame--;
        } else {
            // Increment the animation frame (this guardian is moving right).
            HGUARDS[i].frame++;
        }
    }
}

// Draw the horizontal guardians in the current cavern,
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool GuardianHorizontal_draw() {
    uint8_t msb, lsb;
    uint16_t addr;

    // Point IY at the first byte of the first horizontal guardian definition at HGUARDS.

    // The guardian-drawing loop begins here.
    for (int i = 0; i < 4; i++) {
        // Have we dealt with all the guardians yet?
        if (HGUARDS[i].speedColour == 255) {
            return false;
        }

        // Is this guardian definition blank? If so, skip it and consider the next one.
        if (HGUARDS[i].speedColour == 0) {
            continue;
        }

        // Point HL at the address of the guardian's location in the attribute buffer at 23552,
        addr = HGUARDS[i].attributeAddress;

        // Reset bit 7 (which specifies the animation speed) of the attribute byte, ensuring no FLASH,
        uint8_t attr = (uint8_t) (HGUARDS[i].speedColour & 127);

        // Set the attribute bytes for the guardian in the buffer at 23552,
        Speccy_write(addr, attr);
        addr++;
        Speccy_write(addr, attr);
        addr += 31;
        Speccy_write(addr, attr);
        addr++;
        Speccy_write(addr, attr);

        // Pick up the animation frame (0-7),
        uint8_t anim_frame = (uint8_t) HGUARDS[i].frame;
        // Multiply it by 32,
        anim_frame = rotr(anim_frame, 3);

        // If we are not in one of the first seven caverns, The Endorian Forest, or The Sixteenth Cavern...
        if (cavern.SHEET >= 7 && cavern.SHEET != 9 && cavern.SHEET != 15) {
            // Add 128 to E (the horizontal guardians in this cavern use frames 4-7 only),
            anim_frame |= (1 << 7);
        }

        // Point DE at the graphic data for the appropriate guardian sprite (at GGDATA+E),
        // Point HL at the address of the guardian's location in the screen buffer at 24576,
        split_address(HGUARDS[i].attributeAddress, &msb, &lsb);
        addr = build_address((uint8_t) HGUARDS[i].addressMSB, lsb);

        // Draw the guardian to the screen buffer at 24576,
        bool kill_willy = DRWFIX(&HGUARDS[i].GGDATA[anim_frame], addr, 1);

        // Kill Willy if the guardian collided with him,
        if (kill_willy) {
            Willy_kill();
            return true;
        }
    }

    return false; // IMPORTANT Willy has not died! -MRC-
}