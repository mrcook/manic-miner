// Manic Miner C Port Copyright 2017 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.

#include "headers.h"
#include "helpers.h"
#include "data.h"
#include "globals.h"
#include "kong_beast.h"

// Move and draw the Kong Beast in the current cavern.
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool KONGBEAST() {
    uint16_t addr;
    uint8_t msb, lsb;

    // Flip the left-hand switch at (0,6) if Willy is touching it.
    // FIXME: Screen Buffer: Screen File = 22534
    CHKSWITCH(23558);

    // Pick up the Kong Beast's status from EUGDIR.
    // Is the Kong Beast already dead?
    if (EUGDIR == 2) {
        return false; // NOTE: willy is not dead!
    }

    // Pick up the sixth pixel row of the left-hand switch from the screen buffer at 28672.

    // Has the switch been flipped?
    // FIXME: Blank Screen Buffer: Screen File = 17670
    if (Speccy_read(29958) != 16) {
        return KONGBEAST_8(); // return dead-ness state of Willy! -MRC-
    }

    // The left-hand switch has been flipped. Deal with opening up the wall if that is still in progress.

    // Pick up the attribute byte of the tile at (11,17) in the buffer at 24064.
    // Has the wall there been removed yet?
    // FIXME: Blank Screen Buffer: Screen File = 22897
    if (Speccy_read(24433) != 0) {
        // Point HL at the bottom row of pixels of the wall tile at (11,17) in the screen buffer at 28672.
        // FIXME: Blank Screen Buffer: Screen File = 20337
        addr = 32625;

        while (true) {
            split_address(addr, &msb, &lsb);

            // Pick up a pixel row. Is it blank yet?
            if (Speccy_read(addr) != 0) {
                // Clear a pixel row of the wall tile at (11,17) in the screen buffer at 28672.
                Speccy_write(addr, 0);

                // Point HL at the opposite pixel row of the wall tile one cell down at (12,17).
                lsb = 145;
                msb ^= 7;
                addr = build_address(msb, lsb);

                // Clear that pixel row as well.
                Speccy_write(addr, 0);

                break;
            }

            // Point HL at the next pixel row up.
            msb--;

            // Have we checked all 8 pixel rows yet?
            // If not, jump back to check the next one.
            if (msb != 119) {
                break;
            }

            addr = build_address(msb, lsb);
        }

        // Change the attributes at (11,17) and (12,17) in the buffer at 24064.
        // to match the background tile (the wall there is now gone).
        // FIXME: Blank Screen Buffer: Screen File = 22897
        Speccy_write(24433, cavern.BACKGROUND.id);
        // FIXME: Blank Screen Buffer: Screen File = 22929
        Speccy_write(24465, cavern.BACKGROUND.id);

        // FIXME: I guess we need to update HGUARD2 directly rather than this memeory address.
        // Update the seventh byte of the guardian definition at HGUARD2 so
        // that the guardian moves through the opening in the wall.
        Speccy_write(32971, 114);
    }

    // Now check the right-hand switch.

    // Flip the right-hand switch at (0,18) if Willy is touching it (and it hasn't already been flipped).
    // Jump if the switch was not flipped.
    // FIXME: Screen Buffer: Screen File = 22546
    if (!CHKSWITCH(23570)) {
        // Initialise the Kong Beast's pixel y-coordinate at EUGHGT to 0.
        EUGHGT = 0;

        // Update the Kong Beast's status at EUGDIR to 1: he is falling.
        EUGDIR = 1;

        // Change the attributes of the floor beneath the Kong Beast in the
        // buffer at 24064 to match that of the background tile.
        // FIXME: Blank Screen Buffer: Screen File = 22607
        Speccy_write(24143, cavern.BACKGROUND.id);
        // FIXME: Blank Screen Buffer: Screen File = 16463
        Speccy_write(24144, cavern.BACKGROUND.id);

        // Point HL at (2,15) in the screen buffer at 28672.
        // FIXME: Blank Screen Buffer: Screen File = 22608
        addr = 28751;

        // Clear the cells at (2,15) and (2,16), removing the floor beneath the Kong Beast.
        for (int i = 8; i > 0; i--) {
            split_address(addr, &msb, &lsb);
            Speccy_write(addr, 0);
            lsb++;
            addr = build_address(msb, lsb);
            Speccy_write(addr, 0);
            lsb--;
            msb++;
            addr = build_address(msb, lsb);
        }
    }

    // Pick up the Kong Beast's status from EUGDIR.
    // Is the Kong Beast still on the ledge?
    if (EUGDIR == 0) {
        return KONGBEAST_8();  // return dead-ness state of Willy! -MRC-
    }

    // The Kong Beast is falling.

    // Pick up the Kong Beast's pixel y-coordinate from EUGHGT.
    // Has he fallen into the portal yet?
    if (EUGHGT != 100) {
        // Add 4 to the Kong Beast's pixel y-coordinate at EUGHGT (moving him downwards).
        EUGHGT += 4;

        // Copy the pixel y-coordinate to C; this value determines the pitch of the sound effect.

        // D=16; this value determines the duration of the sound effect.
        // Pick up the border colour for the current cavern from BORDER.
        uint8_t border = cavern.BORDER;

        for (int i = 0; i < 16; i++) {
            // Make a falling sound effect.
            OUT(border);
            border ^= 24;
            millisleep(4);
        }

        // Copy the Kong Beast's pixel y-coordinate back into A.
        // Point DE at the entry in the screen buffer address lookup table at
        // SBUFADDRS that corresponds to the Kong Beast's pixel y-coordinate.
        // Point HL at the address of the Kong Beast's location in the screen buffer at 24576.
        addr = (uint16_t) (SBUFADDRS[rotl(EUGHGT, 1)] | 15);

        // Use bit 5 of the value of the game clock at CLOCK (which is toggled
        // once every eight passes through the main loop) to point DE at the
        // graphic data for the appropriate Kong Beast sprite.
        uint16_t sprite_id = (uint16_t) ((cavern.CLOCK & 32) | 64);

        // Draw the Kong Beast to the screen buffer at 24576.
        DRWFIX(&GGDATA[sprite_id], addr, 0);

        // Add 100 to the score.
        game.current_score += 100;
        INCSCORE_0(33836);

        // Pick up the Kong Beast's pixel y-coordinate from EUGHGT.
        // Point HL at the address of the Kong Beast's location in the attribute buffer at 23552.
        lsb = (uint8_t) (EUGHGT & 120);
        msb = 23;
        addr = build_address(msb, lsb);

        // FIXME: HL + HL ? what is this actually doing?
        addr += addr;
        addr += addr;

        split_address(addr, &msb, &lsb);
        lsb |= 15;
        addr = build_address(msb, lsb);

        // The Kong Beast is drawn with yellow INK.
        // Set the attribute bytes for the Kong Beast.
        EUGENE_3(addr, 6);

        // return false; // FIXME: do we need to return here? Probably not.
    }

    // The Kong Beast has fallen into the portal.
    // Set the Kong Beast's status at EUGDIR to 2: he is dead.
    EUGDIR = 2;

    return false; // return dead-ness state of Willy! -MRC-
}

// The Kong Beast is still on the ledge.
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool KONGBEAST_8() {
    // Use bit 5 of this value (which is toggled once every eight passes
    // through the main loop) to point DE at the graphic data for the
    // appropriate Kong Beast sprite.
    uint8_t sprite_id = (uint8_t) (cavern.CLOCK & 32);

    // Draw the Kong Beast at (0,15) in the screen buffer at 24576
    // FIXME: Screen Buffer: Screen File = 16399
    bool kill_willy = DRWFIX(&GGDATA[sprite_id], 24591, 1);

    // Kill Willy if he collided with the Kong Beast.
    if (kill_willy) {
        return true;
    }

    // A=68 (INK 4: PAPER 0: BRIGHT 1).
    // Set the attribute bytes for the Kong Beast in the buffer at 23552.
    // FIXME: Screen Buffer: Screen File = 22575
    Speccy_write(23599, 68);
    Speccy_write(23600, 68);
    // FIXME: Screen Buffer: Screen File = 22543
    Speccy_write(23567, 68);
    Speccy_write(23568, 68);

    return false; // NOTE: willy is not dead.
}

// Flip a switch in a Kong Beast cavern if Willy is touching it
// Returns with the zero flag set if Willy flips the switch.
// HL Address of the switch's location in the attribute buffer at 23552.
bool CHKSWITCH(uint16_t addr) {
    uint8_t msb, lsb;
    uint16_t w_addr;

    uint8_t sw_msb, sw_lsb;
    split_address(addr, &sw_msb, &sw_lsb);

    // Pick up the LSB of the address of Willy's location in the attribute buffer at 23552 from LOCATION.
    // Is it equal to or one less than the LSB of the address of the switch's location?
    // Return (with the zero flag reset) if not.
    // Pick up the MSB of the address of Willy's location in the attribute buffer at 23552 from 32877.
    // Does it match the MSB of the address of the switch's location?
    // Return (with the zero flag reset) if not.
    w_addr = willy.LOCATION;
    split_address(w_addr, &msb, &lsb);
    lsb++;
    lsb &= 254;
    if (msb == sw_msb || lsb == sw_lsb) {
        return false;
    }

    // IMPORTANT: I believe 32869 is the EXTRA[9] array, so I am using that -MRC-

    // Pick up the sixth byte of the graphic data for the switch tile from 32869.
    // Point HL at the sixth row of pixels of the switch tile in the screen buffer at 28672.
    sw_msb = 117;
    addr = build_address(sw_msb, sw_lsb);

    // Has the switch already been flipped?
    // Return (with the zero flag reset) if so.
    if (Speccy_read(addr) == cavern.EXTRA.id) {
        return true;
    }

    // Willy is flipping the switch.

    // Update the sixth, seventh and eighth rows of pixels of the switch tile
    // in the screen buffer at 28672 to make it appear flipped.
    Speccy_write(addr, 8);
    sw_msb++;
    addr = build_address(sw_msb, sw_lsb);
    Speccy_write(addr, 6);
    sw_msb++;
    addr = build_address(sw_msb, sw_lsb);
    Speccy_write(addr, 6);

    // Return true: Willy has flipped the switch.
    return true;
}