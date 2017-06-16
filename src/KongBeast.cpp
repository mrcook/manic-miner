// Manic Miner C Port Copyright 2017 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.

#include "Headers.h"
#include "Data.h"
#include "Globals.h"
#include "KongBeast.h"
#include "GuardianSpecial.h"

// Move and draw the Kong Beast in the current cavern.
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool KONGBEAST(void) {
    // Check the left-hand switch.
    // Flip the left-hand switch at (0,6) if Willy is touching it.
    CHKSWITCH(23558);

    // Pick up the Kong Beast's status from EUGDIR.
    // Is the Kong Beast already dead?
    if (EUGDIR == 2) {
        return false; // NOTE: willy is not dead!
    }

    // Pick up the sixth pixel row of the left-hand switch from the screen buffer at 28672.
    // Has the switch been flipped?
    if (speccy.readMemory(29958) == 16) {
        return animateKongBeast(); // return dead-ness state of Willy! -MRC-
    }
    // Okay, the left-hand switch has been flipped...continue.

    // Deal with opening up the wall
    Kong_openWall();

    // Now check the right-hand switch.
    // Flip the right-hand switch at (0,18) if Willy is touching it (and it hasn't already been flipped).
    // Jump if the switch was not flipped.
    if (CHKSWITCH(23570)) {
        Kong_removeBeastFloor();
    }

    // Pick up the Kong Beast's status from EUGDIR.
    // Is the Kong Beast still on the ledge?
    if (EUGDIR == 0) {
        return animateKongBeast();  // return dead-ness state of Willy! -MRC-
    }

    // The Kong Beast is falling.

    // Pick up the Kong Beast's pixel y-coordinate from EUGHGT.
    // Has he fallen into the portal yet?
    if (EUGHGT < 100) {
        Kong_beastFalls();
        return false;
    }

    // The Kong Beast has fallen into the portal.
    // Set the Kong Beast's status at EUGDIR to 2: he is dead.
    EUGDIR = 2;

    return false; // return dead-ness state of Willy! -MRC-
}

// Deal with opening up the wall if that is still in progress.
void Kong_openWall(void) {
    // Pick up the attribute byte of the tile at (11,17) in the buffer at 24064.
    // Has the wall there been removed yet?
    if (speccy.readMemory(24433) == 0) {
        return;
    }

    // Point HL at the bottom row of pixels of the wall tile at (11,17) in the screen buffer at 28672.
    uint16_t addr = 32625;

    uint8_t msb, lsb;
    while (true) {
        Speccy::splitAddress(addr, &msb, &lsb);

        // Pick up a pixel row. Is it blank yet?
        if (speccy.readMemory(addr) != 0) {
            // Clear a pixel row of the wall tile at (11,17) in the screen buffer at 28672.
            speccy.writeMemory(addr, 0);

            // Point HL at the opposite pixel row of the wall tile one cell down at (12,17).
            lsb = 145;
            msb ^= 7;
            addr = Speccy::buildAddress(msb, lsb);

            // Clear that pixel row as well.
            speccy.writeMemory(addr, 0);

            break;
        }

        // Point HL at the next pixel row up.
        msb--;

        // Have we checked all 8 pixel rows yet?
        // If not, jump back to check the next one.
        if (msb == 119) {
            // Change the attributes at (11,17) and (12,17) in the buffer at 24064.
            // to match the background tile (the wall there is now gone).
            speccy.writeMemory(24433, cavern.BACKGROUND.id);
            speccy.writeMemory(24465, cavern.BACKGROUND.id);

            // Update the seventh byte of the guardian definition at HGUARD2 so
            // that the guardian moves through the opening in the wall.
            HGUARDS[1].addressRightLSB = 114;

            break;
        }

        addr = Speccy::buildAddress(msb, lsb);
    }
}

void Kong_removeBeastFloor(void) {
    uint16_t addr;
    uint8_t msb, lsb;

    // Initialise the Kong Beast's pixel y-coordinate at EUGHGT to 0.
    EUGHGT = 0;

    // Update the Kong Beast's status at EUGDIR to 1: he is falling.
    EUGDIR = 1;

    // Change the attributes of the floor beneath the Kong Beast in the
    // buffer at 24064 to match that of the background tile.
    speccy.writeMemory(24143, cavern.BACKGROUND.id);
    speccy.writeMemory(24144, cavern.BACKGROUND.id);

    // Point HL at (2,15) in the screen buffer at 28672.
    addr = 28751;

    // Clear the cells at (2,15) and (2,16), removing the floor beneath the Kong Beast.
    for (int i = 8; i > 0; i--) {
        speccy.writeMemory(addr, 0);
        speccy.writeMemory(addr + 1, 0);

        Speccy::splitAddress(addr, &msb, &lsb);
        msb++;
        addr = Speccy::buildAddress(msb, lsb);
    }
}

void Kong_beastFalls(void) {
    uint16_t addr;
    uint8_t msb, lsb;

    // Add 4 to the Kong Beast's pixel y-coordinate at EUGHGT (moving him downwards).
    EUGHGT += 4;

    // Copy the pixel y-coordinate to C; this value determines the pitch of the sound effect.

    // D=16; this value determines the duration of the sound effect.
    // Pick up the border colour for the current cavern from BORDER.
    speccy.setBorderColour(cavern.BORDER);

    // Make a falling sound effect.
    speccy.beep(EUGHGT, 16, 5);

    // Point DE at the entry in the screen buffer address lookup table at
    // SBUFADDRS that corresponds to the Kong Beast's pixel y-coordinate.
    // Point HL at the address of the Kong Beast's location in the screen buffer at 24576.
    uint8_t pixelY = Speccy::rotL(EUGHGT, 1);

    lsb = (uint8_t) (SBUFADDRS[pixelY / 2] | 15);
    msb = Speccy::getAddressMSB(SBUFADDRS[(pixelY + 1) / 2]);
    addr = Speccy::buildAddress(msb, lsb);

    // Use bit 5 of the value of the game clock at CLOCK (which is toggled
    // once every eight passes through the main loop) to point DE at the
    // graphic data for the appropriate Kong Beast sprite.
    uint16_t spriteID = (uint16_t) ((cavern.CLOCK & 32) | 64);

    // Draw the Kong Beast to the screen buffer at 24576.
    DRWFIX(&GGDATA[spriteID], addr, 0);

    // Add 100 to the score.
    Game_scoreAdd(100);

    // Pick up the Kong Beast's pixel y-coordinate from EUGHGT.
    // Point HL at the address of the Kong Beast's location in the attribute buffer at 23552.
    lsb = (uint8_t) (EUGHGT & 120);
    msb = 23;
    addr = Speccy::buildAddress(msb, lsb);

    // NOTE: `ADD HL,HL` is this really correct? What is happening here?
    addr += addr;
    addr += addr;

    msb = Speccy::getAddressMSB(addr);
    lsb = Speccy::getAddressLSB(addr);
    lsb |= 15;
    addr = Speccy::buildAddress(msb, lsb);

    // The Kong Beast is drawn with yellow INK.
    // Set the attribute bytes for the Kong Beast.
    UpdateGuardianColourAttributes(addr, 6);
}

// The Kong Beast is still on the ledge.
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool animateKongBeast(void) {
    // Use bit 5 of this value (which is toggled once every eight passes
    // through the main loop) to point DE at the graphic data for the
    // appropriate Kong Beast sprite.
    uint8_t sprite_id = (uint8_t) (cavern.CLOCK & 32);

    // Draw the Kong Beast at (0,15) in the screen buffer at 24576
    bool kill_willy = DRWFIX(&GGDATA[sprite_id], 24591, 1);

    // Kill Willy if he collided with the Kong Beast.
    if (kill_willy) {
        return true;
    }

    // A=68 (INK 4: PAPER 0: BRIGHT 1).
    // Set the attribute bytes for the Kong Beast in the buffer at 23552.
    speccy.writeMemory(23599, 68);
    speccy.writeMemory(23600, 68);
    speccy.writeMemory(23567, 68);
    speccy.writeMemory(23568, 68);

    return false; // NOTE: willy is not dead.
}

// Flip a switch in a Kong Beast cavern if Willy is touching it
// Returns with the zero flag set if Willy flips the switch.
// HL Address of the switch's location in the attribute buffer at 23552.
bool CHKSWITCH(uint16_t switchAddress) {
    uint8_t switchMSB = Speccy::getAddressMSB(switchAddress);
    uint8_t switchLSB = Speccy::getAddressLSB(switchAddress);

    // Pick up the LSB of the address of Willy's location in the attribute buffer at 23552 from LOCATION.
    uint8_t lsb = Speccy::getAddressLSB(willy.LOCATION);

    // Is it equal to or one less than the LSB of the address of the switch's location?
    // Return (with the zero flag reset) if not.
    lsb++;
    lsb &= 254;
    if (lsb != switchLSB) {
        return false;
    }

    // Pick up the MSB of the address of Willy's location in the attribute buffer at 23552 from 32877.
    uint8_t msb = Speccy::getAddressMSB(willy.LOCATION);

    // Does it match the MSB of the address of the switch's location?
    // Return (with the zero flag reset) if not.
    if (msb != switchMSB) {
        return false;
    }

    // Has the switch already been flipped?
    // Return (with the zero flag reset) if so.
    if (Kong_switchFlipped(switchAddress)) {
        return false;
    }

    Kong_flipSwitch(switchAddress);

    // Return true: Willy has flipped the switch.
    return true;
}

bool Kong_switchFlipped(uint16_t switchAddress) {
    // Pick up the sixth byte of the graphic data for the switch tile from 32869.
    uint8_t sixthByte = cavern.EXTRA.sprite[5];

    // Point HL at the sixth row of pixels of the switch tile in the screen buffer at 28672.
    uint8_t switchMSB = Speccy::getAddressMSB(switchAddress);
    uint8_t switchLSB = Speccy::getAddressLSB(switchAddress);
    switchMSB += 25; // 92 + 25 = 117 .... but why 25, and why 117?
    switchAddress = Speccy::buildAddress(switchMSB, switchLSB);

    return speccy.readMemory(switchAddress) != sixthByte;
}

// Willy is flipping the switch.
void Kong_flipSwitch(uint16_t switchAddress) {
    // Point HL at the sixth row of pixels of the switch tile in the screen buffer at 28672.
    uint8_t switchMSB = Speccy::getAddressMSB(switchAddress);
    uint8_t switchLSB = Speccy::getAddressLSB(switchAddress);
    switchMSB += 25; // 92 + 25 = 117 .... but why 25, and why 117?
    switchAddress = Speccy::buildAddress(switchMSB, switchLSB);

    // Update the sixth, seventh and eighth rows of pixels of the switch tile
    // in the screen buffer at 28672 to make it appear flipped.
    speccy.writeMemory(switchAddress, 8);

    switchMSB++;
    switchAddress = Speccy::buildAddress(switchMSB, switchLSB);
    speccy.writeMemory(switchAddress, 6);

    switchMSB++;
    switchAddress = Speccy::buildAddress(switchMSB, switchLSB);
    speccy.writeMemory(switchAddress, 6);
}
