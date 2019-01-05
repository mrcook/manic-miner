// Manic Miner C++ port Copyright (c) 2016-2018 Michael R. Cook
// Manic Miner Copyright (c) 1983 Bug-Byte Ltd.

#include "headers.h"
#include "globals.h"
#include "data.h"
#include "guardian_special.h"

// Move and GuardianH_draw the vertical guardians in the current cavern.
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool GuardianV_updateAndDraw(GuardianVertical &guardian) {
    // Have we dealt with this guardians already?
    if (guardian.attribute == 255) {
        return false;
    }

    // Increment the guardian's animation frame, or reset to 0 if it overflowed to 4.
    if (guardian.frame < 3) {
        guardian.frame++;
    } else {
        guardian.frame = 0;
    }

    // Pick up the guardian's pixel y-coordinate.
    // Add the current y-coordinate increment.
    auto y_coord = uint8_t(guardian.yCoord + guardian.yPixelIncrement);

    // Has the guardian reached the lowest/highest point of its path (minimum y-coordinate)?
    // If so, change its direction of movement
    if (y_coord < guardian.yCoordMinimum || y_coord >= guardian.yCoordMaximum) {
        // Negate the y-coordinate increment; this changes the guardian's direction of movement.
        guardian.yPixelIncrement = uint8_t(-guardian.yPixelIncrement);
    } else {
        // Update the guardian's pixel y-coordinate.
        guardian.yCoord = y_coord;
    }

    // Now that the guardian's movement has been dealt with, time to GuardianH_draw it.

    // Pick up the guardian's pixel y-coordinate.
    // Point DE at the entry in the screen buffer address lookup table at
    // SBUFADDRS that corresponds to the guardian's pixel y-coordinate.
    y_coord = Speccy::rotL(uint8_t(guardian.yCoord & 127), 1);
    uint16_t addr = SBUFADDRS[y_coord / 2];

    uint8_t msb, lsb;
    uint8_t msb_bak, lsb_bak;

    // Point HL at the address of the guardian's location in the screen buffer at 24576.
    Speccy::splitAddress(addr, msb, lsb);
    lsb_bak = lsb | guardian.xCoord;
    y_coord++;
    addr = SBUFADDRS[y_coord / 2];
    Speccy::splitAddress(addr, msb, lsb);
    addr = Speccy::buildAddress(msb, lsb_bak);

    // Pick up the guardian's animation frame (0-3). Multiply it by 32.
    uint8_t anim_frame = Speccy::rotR(guardian.frame, 3);

    // Draw the guardian to the screen buffer at 24576
    bool kill_willy = DRWFIX(&GGDATA[anim_frame], addr, 1);

    // Kill Willy if the guardian collided with him.
    if (kill_willy) {
        Willy_kill();
        return true;
    }

    // Pick up the guardian's pixel y-coordinate.
    // Point HL at the address of the guardian's location in the attribute buffer at 23552.
    addr = uint16_t(Speccy::rotL(uint8_t(guardian.yCoord & 64), 2) + 92);
    Speccy::splitAddress(addr, msb, lsb);
    msb_bak = lsb;
    addr = uint16_t(Speccy::rotL(guardian.yCoord, 2) & 224);
    Speccy::splitAddress(addr, msb, lsb);
    addr = Speccy::buildAddress(msb_bak, lsb | guardian.xCoord);

    // Set the attribute bytes for the guardian.
    UpdateGuardianColourAttributes(addr, guardian.attribute);

    // Great, Willy is not dead!
    return false;
}
