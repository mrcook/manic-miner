// Manic Miner C Port Copyright 2017 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.

#include "Headers.h"
#include "Globals.h"
#include "Data.h"

// Move and draw the vertical guardians in the current cavern.
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool GuardianVertical_updateDraw() {
    uint8_t msb, lsb;
    uint8_t msb_bak, lsb_bak;

    // The guardian-moving loop begins here.
    for (int i = 0; i < 4; i++) {
        // Have we dealt with all the guardians yet?
        if (VGUARDS[i].attribute == 255) {
            return false;
        }

        // Increment the guardian's animation frame, or reset to 0 if it overflowed to 4.
        if (VGUARDS[i].frame < 3) {
            VGUARDS[i].frame++;
        } else {
            VGUARDS[i].frame = 0;
        }

        // Pick up the guardian's pixel y-coordinate.
        // Add the current y-coordinate increment.
        uint8_t y_coord = (uint8_t) (VGUARDS[i].yCoord + VGUARDS[i].yPixelIncrement);

        // Has the guardian reached the lowest/highest point of its path (minimum y-coordinate)?
        // If so, change its direction of movement
        if (y_coord < VGUARDS[i].yCoordMinimum || y_coord >= VGUARDS[i].yCoordMaximum) {
            // Negate the y-coordinate increment; this changes the guardian's direction of movement.
            VGUARDS[i].yPixelIncrement = (uint8_t) (-VGUARDS[i].yPixelIncrement);
        } else {
            // Update the guardian's pixel y-coordinate.
            VGUARDS[i].yCoord = y_coord;
        }

        // Now that the guardian's movement has been dealt with, time to draw it.

        // Pick up the guardian's pixel y-coordinate.
        // Point DE at the entry in the screen buffer address lookup table at
        // SBUFADDRS that corresponds to the guardian's pixel y-coordinate.
        y_coord = rotL((uint8_t) (VGUARDS[i].yCoord & 127), 1);
        uint16_t addr = SBUFADDRS[y_coord / 2];

        // Point HL at the address of the guardian's location in the screen buffer at 24576.
        splitAddress(addr, &msb, &lsb);
        lsb_bak = lsb | VGUARDS[i].xCoord;
        y_coord++;
        addr = SBUFADDRS[y_coord / 2];
        splitAddress(addr, &msb, &lsb);
        addr = buildAddress(msb, lsb_bak);

        // Pick up the guardian's animation frame (0-3). Multiply it by 32.
        uint8_t anim_frame = rotR(VGUARDS[i].frame, 3);

        // Draw the guardian to the screen buffer at 24576
        bool kill_willy = DRWFIX(&VGUARDS[i].GGDATA[anim_frame], addr, 1);

        // Kill Willy if the guardian collided with him.
        if (kill_willy) {
            Willy_kill();
            return true;
        }

        // Pick up the guardian's pixel y-coordinate.
        // Point HL at the address of the guardian's location in the attribute buffer at 23552.
        addr = (uint16_t) (rotL((uint8_t) (VGUARDS[i].yCoord & 64), 2) + 92);
        splitAddress(addr, &msb, &lsb);
        msb_bak = msb;

        addr = (uint16_t) (rotL(VGUARDS[i].yCoord, 2) & 224);
        splitAddress(addr, &msb, &lsb);
        lsb |= VGUARDS[i].xCoord;
        addr = buildAddress(msb_bak, lsb);

        // Set the attribute bytes for the guardian.
        EUGENE_3(addr, VGUARDS[i].attribute);

        // The current guardian definition has been dealt with. Time for the next one.
    }

    return false; // Willy is not dead.
}
