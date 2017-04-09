// Manic Miner C Port Copyright 2017 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.

#include "Headers.h"
#include "Globals.h"
#include "Data.h"
#include "GuardianSpecial.h"

// Move and draw the vertical guardians in the current cavern.
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool GuardianVertical::updateAndDraw() {
    // Have we dealt with this guardians already?
    if (attribute == 255) {
        return false;
    }

    // Increment the guardian's animation frame, or reset to 0 if it overflowed to 4.
    if (frame < 3) {
        frame++;
    } else {
        frame = 0;
    }

    // Pick up the guardian's pixel y-coordinate.
    // Add the current y-coordinate increment.
    uint8_t y_coord = (uint8_t) (yCoord + yPixelIncrement);

    // Has the guardian reached the lowest/highest point of its path (minimum y-coordinate)?
    // If so, change its direction of movement
    if (y_coord < yCoordMinimum || y_coord >= yCoordMaximum) {
        // Negate the y-coordinate increment; this changes the guardian's direction of movement.
        yPixelIncrement = (uint8_t) (-yPixelIncrement);
    } else {
        // Update the guardian's pixel y-coordinate.
        yCoord = y_coord;
    }

    // Now that the guardian's movement has been dealt with, time to draw it.

    // Pick up the guardian's pixel y-coordinate.
    // Point DE at the entry in the screen buffer address lookup table at
    // SBUFADDRS that corresponds to the guardian's pixel y-coordinate.
    y_coord = Speccy::rotL((uint8_t) (yCoord & 127), 1);
    uint16_t addr = SBUFADDRS[y_coord / 2];

    uint8_t msb, lsb;
    uint8_t msb_bak, lsb_bak;

    // Point HL at the address of the guardian's location in the screen buffer at 24576.
    Speccy::splitAddress(addr, &msb, &lsb);
    lsb_bak = lsb | xCoord;
    y_coord++;
    addr = SBUFADDRS[y_coord / 2];
    Speccy::splitAddress(addr, &msb, &lsb);
    addr = Speccy::buildAddress(msb, lsb_bak);

    // Pick up the guardian's animation frame (0-3). Multiply it by 32.
    uint8_t anim_frame = Speccy::rotR(frame, 3);

    // Draw the guardian to the screen buffer at 24576
    bool kill_willy = DRWFIX(&GGDATA[anim_frame], addr, 1);

    // Kill Willy if the guardian collided with him.
    if (kill_willy) {
        Willy_kill();
        return true;
    }

    // Pick up the guardian's pixel y-coordinate.
    // Point HL at the address of the guardian's location in the attribute buffer at 23552.
    addr = (uint16_t) (Speccy::rotL((uint8_t) (yCoord & 64), 2) + 92);
    Speccy::splitAddress(addr, &msb, &lsb);
    msb_bak = lsb;
    addr = (uint16_t) (Speccy::rotL(yCoord, 2) & 224);
    Speccy::splitAddress(addr, &msb, &lsb);
    addr = Speccy::buildAddress(msb_bak, lsb | xCoord);

    // Set the attribute bytes for the guardian.
    UpdateGuardianColourAttributes(addr, attribute);

    // Great, Willy is not dead!
    return false;
}
