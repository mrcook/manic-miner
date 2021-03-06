// Manic Miner C/C++ port Copyright (c) 2016-2021 Michael R. Cook
// Manic Miner Copyright (c) 1983 Bug-Byte Ltd.

#include "headers.h"
#include "data.h"
#include "globals.h"

void Willy_loadSprites() {
    for (int i = 0; i < 256; i++) {
        willy.sprites[i] = WILLYDATA[i];
    }
}

void Willy_reset(int lives) {
    willy.NOMEN = lives;
    willy.PIXEL_Y = 208;
    willy.FRAME = 0;
    willy.DMFLAGS = 0;
    willy.AIRBORNE = 0;
    willy.LOCATION = 23970;
    willy.JUMPING = 0;
}

// Reset bit 1 at DMFLAGS: Willy is not moving left or right.
void Willy_setToUnmoving() {
    willy.DMFLAGS &= ~(1 << 1);
}

// Willy is currently jumping, update his state.
// Return value of `true` indicates that Willy has finished jumping
bool Willy_updateJumpingState() {
    // Pick up the jumping animation counter (0-17).
    uint8_t jump = willy.JUMPING;

    // Now -8<=A<=8 (and A is even).
    jump &= ~(1 << 0); // RES 0,A
    jump -= 8;         // SUB 8

    // Adjust Willy's pixel y-coordinate at PIXEL_Y depending on where Willy is in the jump.
    willy.PIXEL_Y += jump;

    // Adjust Willy's attribute buffer location at LOCATION depending on his pixel y-coordinate.
    uint16_t location = Willy_adjustAttributes(willy.PIXEL_Y);

    // Is the top-left or top-right cell of Willy's sprite overlapping a wall tile?
    if (Speccy_readMemory(location) == cavern.WALL.id || Speccy_readMemory(location + 1) == cavern.WALL.id) {
        Willy_hitsWall();
        return true;
    }

    // Increment the jumping animation counter.
    willy.JUMPING++;

    Willy_triggerJumpingSound(willy.JUMPING);

    // Pick up the border colour for the current cavern.
    Speccy_setBorderColour(cavern.BORDER);

    return Willy_hasFinishedJumping();
}

void Willy_triggerJumpingSound(uint8_t jumpHeight) {
    uint8_t pitch = jumpHeight;

    // SUB 8
    // A=J-8, where J (1-18) is the new value of the jumping animation counter.
    pitch -= 8;

    // Jump if J>=8, else....
    if (jumpHeight < 8) {
        // NEG pitch
        // A=8-J (1<=J<=7, 1<=A<=7)
        pitch *= -1;
    }

    // A=1+ABS(J-8)
    pitch++;

    // D=8*(1+ABS(J-8)); this value determines the pitch of the jumping
    // sound effect (rising as Willy rises, falling as Willy falls).
    pitch = Speccy_rotL(pitch, 3);

    // C=32; this value determines the duration of the jumping sound effect.
    Speccy_beep(pitch, 32, 5);
}

bool Willy_hasFinishedJumping() {
    // Jumping animation counter will have a value of 1-18.
    // Has Willy reached the end of the jump?
    if (willy.JUMPING == 18) {
        // Willy has just finished a jump.
        // Set the airborne status indicator at AIRBORNE to 6: Willy will
        // continue to fall unless he's landed on a wall or floor block.
        willy.AIRBORNE = 6;
        return true;
    }

    // Is the jumping animation counter now 16 or 13?
    if (willy.JUMPING == 16) {
        return false;
    }

    if (willy.JUMPING != 13) {
        Willy_moveInDirectionFacing();
        return true;
    }

    // Willy is still jumping!
    return false;
}

// Adjust Willy's attribute buffer location at LOCATION to
// account for this new pixel y-coordinate.
uint16_t Willy_adjustAttributes(uint8_t y_coord) {
    uint8_t msb, lsb;

    // L=16*Y, where Y is Willy's screen y-coordinate (0-14).
    lsb = uint8_t(y_coord & 240);

    // ...the Carry flag was reset here in the Z80 code.

    // Now L=32*(Y-8*INT(Y/8)), and the carry flag is set if Willy is in the lower half of the cavern (Y>=8).
    // If LSB bit 7 is set the `RL L` will set the Carry, which means we need to add one to the MSB.
    // H=92 or 93 (MSB of the address of Willy's location in the attribute buffer)
    msb = 92;
    // Pickup bit-7 of lsb. If it's set we need to also set the Carry flag.
    if (((lsb >> 7) & 1) == 1) {
        msb = 93;
    }
    lsb = Speccy_rotL(lsb, 1); // RL lsb
    lsb &= ~(1 << 0);   // set bit `0` to the Carry, which was set to `0`.

    // Pick up Willy's screen x-coordinate (1-29) from bits 0-4 at LOCATION.
    uint8_t msb_dummy, x_lsb;
    Speccy_splitAddress((uint16_t) (willy.LOCATION & 31), msb_dummy, x_lsb);

    // Now L holds the LSB of Willy's attribute buffer address.
    x_lsb |= lsb;

    // Store Willy's updated attribute buffer location at LOCATION.
    willy.LOCATION = Speccy_buildAddress(msb, x_lsb);

    return willy.LOCATION;
}

// The top-left or top-right cell of Willy's sprite is overlapping a wall tile.
void Willy_hitsWall() {
    // Adjust Willy's pixel y-coordinate at PIXEL_Y so that the top
    // row of cells of his sprite is just below the wall tile.
    willy.PIXEL_Y += 16;
    willy.PIXEL_Y &= 240;

    // Adjust Willy's attribute buffer location to account for this new pixel y-coordinate.
    // NOTE: willy.LOCATION is set with the new value
    Willy_adjustAttributes(willy.PIXEL_Y);

    // Set the airborne status indicator to 2: Willy has started falling.
    willy.AIRBORNE = 2;

    // Reset bit 1 at DMFLAGS: Willy is not moving left or right
    Willy_setToUnmoving();
}

// Move Willy (2)
// This routine checks the keyboard and joystick, and moves Willy left or right if necessary.
// HL Attribute buffer address of the left-hand cell below Willy's sprite.
bool Willy_getUserInputAndMove(int keyInput, uint16_t addr) {
    // Has Willy just landed after falling from too great a height? If so, kill him!
    if (willy.AIRBORNE >= 12) { // FIXME: should this be `== 12` ?
        Willy_kill();
        return true;
    }

    // Reset the airborne status indicator at AIRBORNE (Willy has landed safely)
    willy.AIRBORNE = 0;

    uint8_t conveyor_dir = 255;
    // Does the attribute byte of the left or right hand cell below Willy's sprite match that of the conveyor tile?
    if (Speccy_readMemory(addr) == cavern.CONVEYOR.id || Speccy_readMemory(addr + 1) == cavern.CONVEYOR.id) {
        // Pick up the direction byte of the conveyor definition from CONVDIR (0=left, 1=right)
        conveyor_dir = cavern.CONVEYOR.CONVDIR;
    }

    uint8_t movement = 0; // no movement
    bool keyLeft = keyInput == Keyboard::MM_KEY_LEFT || keyInput == Keyboard::MM_KEY_LEFT_JUMP;
    bool keyRight = keyInput == Keyboard::MM_KEY_RIGHT || keyInput == Keyboard::MM_KEY_RIGHT_JUMP;

    if (keyLeft || conveyor_dir == 0) {
        movement |= 4;
    }
    if (keyRight || conveyor_dir == 1) {
        movement |= 8;
    }

    // Point HL at the entry in the left-right movement table at LRMOVEMENT
    // that corresponds to the direction Willy is facing, and the direction
    // in which he is being moved or trying to move.
    movement = willy.DMFLAGS + movement;
    // Update Willy's direction and movement flags at DMFLAGS with the
    // entry from the left-right movement table.
    willy.DMFLAGS = LRMOVEMENT[movement];

    // That is left-right movement taken care of. Now check the jump keys.

    // Is jump being pressed?
    if (keyInput == Keyboard::MM_KEY_JUMP || keyInput == Keyboard::MM_KEY_LEFT_JUMP || keyInput == Keyboard::MM_KEY_RIGHT_JUMP) {
        // Time to make Willy jump. Initialise the jumping animation counter.
        willy.JUMPING = 0;

        // Set the airborne status indicator to 1: Willy is jumping.
        willy.AIRBORNE = 1;
    }

    Willy_moveInDirectionFacing();

    return false; // willy is not dead. -MRC-
}

// Check Willy's movement/direction, and update the animation frame accordingly.
// This entry point is used by the routine at MOVEWILLY.
void Willy_moveInDirectionFacing() {
    // Is Willy moving? Return if not.
    if ((willy.DMFLAGS & 2) == 0) {
        return;
    }

    // Is Willy facing right? (0 = right, 1 = left). Return if not.
    if ((willy.DMFLAGS & 1) == 0) {
        Willy_moveRight();
        return;
    }

    // Willy is moving left.

    // Pick up Willy's animation frame. Is it 0?
    // If so move Willy's sprite left across a cell boundary.
    if (willy.FRAME == 0) {
        Willy_moveWillyLeft();
        return;
    }

    // Decrement Willy's animation frame.
    willy.FRAME--;
}

// Willy's sprite is moving left across a cell boundary.
// In the comments that follow, (x,y) refers to the coordinates
// of the top-left cell currently occupied by Willy's sprite.
void Willy_moveWillyLeft() {
    // Collect Willy's attribute buffer coordinates from LOCATION.
    // Point to the cell at (x-1,y+1).
    uint16_t addr = uint16_t(willy.LOCATION - 1);

    addr += 32;

    // Pick up the attribute byte of the wall tile for the current cavern.
    // Is there a wall tile in the cell pointed to?
    // Return if so, without moving Willy (his path is blocked).
    if (Speccy_readMemory(addr) == cavern.WALL.id) {
        return;
    }

    // Does Willy's sprite currently occupy six cells (two rows) of cells?
    if (willy.PIXEL_Y & 15) {
        // Pick up the attribute byte of the wall tile for the current cavern.
        // Point to the cell at (x-1,y+2).
        // Is there a wall tile in the cell pointed to?
        // Return if so without moving Willy (his path is blocked).
        if (Speccy_readMemory(addr + 32) == cavern.WALL.id) {
            return;
        }
    }

    // Pick up the attribute byte of the wall tile for the current cavern.
    // Clear the carry flag for subtraction.
    // Point to the cell at (x-1,y).
    addr -= 32;

    // Is there a wall tile in the cell pointed to?
    // Return if so without moving Willy (his path is blocked).
    if (Speccy_readMemory(addr) == cavern.WALL.id) {
        return;
    }

    // Save Willy's new attribute buffer coordinates to LOCATION.
    willy.LOCATION = addr;

    // Change Willy's animation frame from 0 to 3.
    willy.FRAME = 3;
}

// Willy is moving right.
void Willy_moveRight() {
    // If animation frame is not 3 then he is not moving across a cell boundary.
    if (willy.FRAME != 3) {
        // Increment Willy's animation frame.
        willy.FRAME++;
        return;
    }

    // Willy's sprite is moving right across a cell boundary.
    // In the comments that follow, (x,y) refers to the coordinates
    // of the top-left cell currently occupied by Willy's sprite.

    // Collect Willy's attribute buffer coordinates from LOCATION.
    // Point HL at the cell at (x+2,y).
    uint16_t addr = uint16_t(willy.LOCATION + 2);

    // Pick up the attribute byte of the wall tile for the current cavern from WALL.
    // Point HL at the cell at (x+2,y+1).
    addr += 32;

    // Is there a wall tile in the cell pointed to by HL?
    // Return if so without moving Willy (his path is blocked).
    if (Speccy_readMemory(addr) == cavern.WALL.id) {
        return;
    }

    // Does Willy's sprite currently occupy six cells (two rows) of cells?
    if ((willy.PIXEL_Y & 15)) {
        // Pick up the attribute byte of the wall tile for the current cavern from WALL.
        // Point HL at the cell at (x+2,y+2).
        // Is there a wall tile in the cell pointed to by HL?
        // Return if so without moving Willy (his path is blocked).
        if (Speccy_readMemory(addr + 32) == cavern.WALL.id) {
            return;
        }
    }

    addr -= 32;

    // Is there a wall tile in the cell pointed to by HL?
    // Return if so without moving Willy (his path is blocked).
    if (Speccy_readMemory(addr) == cavern.WALL.id) {
        return;
    }

    // Point HL at the cell at (x+1,y).
    addr--;

    // Save Willy's new attribute buffer coordinates (in HL) at LOCATION.
    willy.LOCATION = addr;

    // Change Willy's animation frame at FRAME from 3 to 0.
    willy.FRAME = 0;
}

// Kill Willy when:
//   * Willy hits a nasty
//   * Willy lands after falling from too great a height
//   * Willy collides with a horizontal guardian)
//   * Willy collides with Eugene)
//   * Willy collides with a vertical guardian)
//   * Willy collides with the Kong Beast
void Willy_kill() {
    willy.AIRBORNE = 255;
}

// Check and set the attribute bytes for Willy's sprite in the buffer at 23552.
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool Willy_checkAttrs() {
    bool kill_willy = false;

    // Pick up the address of Willy's location in the attribute buffer at 23552 from LOCATION.
    uint16_t addr = willy.LOCATION;

    // Set C=15 (`pix_y`) for the top two rows of cells
    // (to make the routine at Willy_setAttrByte force white INK).
    uint8_t pix_y = 15;

    // Check and set the attribute byte for the top-left cell.
    if (Willy_setAttrByte(addr, pix_y)) {
        kill_willy = true;
    }

    // Move HL to the next cell to the right.
    addr++;

    // Check and set the attribute byte for the top-right cell.
    if (Willy_setAttrByte(addr, pix_y)) {
        kill_willy = true;
    }

    // Move down a row and back one cell to the left.
    addr += 31;

    // Check and set the attribute byte for the mid-left cell.
    if (Willy_setAttrByte(addr, pix_y)) {
        kill_willy = true;
    }

    // Move to the next cell to the right.
    addr++;

    // Check and set the attribute byte for the mid-right cell.
    if (Willy_setAttrByte(addr, pix_y)) {
        kill_willy = true;
    }

    // Pick up Willy's pixel y-coordinate from PIXEL_Y. Copy it to pix_y.
    pix_y = willy.PIXEL_Y;

    // Move down a row and back one cell to the left.
    addr += 31;

    // Check and set the attribute byte for the bottom-left cell.
    if (Willy_setAttrByte(addr, pix_y)) {
        kill_willy = true;
    }

    // Move to the next cell to the right.
    addr++;

    // Check and set the attribute byte for the bottom-right cell.
    if (Willy_setAttrByte(addr, pix_y)) {
        kill_willy = true;
    }

    // Draw Willy to the screen buffer at 24576.
    Willy_draw();

    // Is Willy dead?
    return kill_willy;
}

// Check and set the attribute byte for a cell occupied by Willy's sprite.
//
// C=15 or Willy's pixel y-coordinate.
// HL Address of the cell in the attribute buffer at 23552.
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool Willy_setAttrByte(uint16_t addr, uint8_t pix_y) {
    // Does this cell contain a background tile?
    if (Speccy_readMemory(addr) == cavern.BACKGROUND.id) {
        // Set the zero flag if we are going to retain the INK colour in this cell;
        // this happens only if the cell is in the bottom row and Willy's sprite
        // is confined to the top two rows.

        // Jump if we are going to retain the current INK colour in this cell.
        if (pix_y & 15) {
            // Pick up the attribute byte of the BACKGROUND tile.
            // Set bits 0-2, making the INK white.
            // Set the attribute byte for this cell in the buffer at 23552.
            Speccy_writeMemory(addr, (uint8_t) (cavern.BACKGROUND.id | 7));
        }
    }

    // Pick up the attribute byte of the nasty tiles for the current cavern.
    // Has Willy hit a NASTY1 or NASTY2 kind? Kill Willy if so.
    if (Speccy_readMemory(addr) == cavern.NASTY1.id || Speccy_readMemory(addr) == cavern.NASTY2.id) {
        Willy_kill();
        return true;
    }

    return false;
}

// Draw Willy to the screen buffer at 24576.
void Willy_draw() {
    // Pick up Willy's pixel y-coordinate from PIXEL_Y so we can
    // point to the entry in SBUFADDRS (the screen buffer address
    // lookup table) that corresponds to Willy's y-coordinate.
    uint8_t pix_y = uint8_t(willy.PIXEL_Y / 2);

    // Pick up Willy's direction and movement flags from DMFLAGS.
    // Now 0 if Willy is facing right, or 128 if he's facing left.
    uint8_t frame = Speccy_rotR((uint8_t) (willy.DMFLAGS & 1), 1);

    // Pick up Willy's current animation frame (0-3) (see MANDAT).
    frame = Speccy_rotR(uint8_t(willy.FRAME & 3), 3) | frame;

    // Pick up Willy's screen x-coordinate (0-31) from LOCATION.
    uint8_t msb_dummy, pix_x;
    Speccy_splitAddress(uint16_t(willy.LOCATION & 31), msb_dummy, pix_x);

    // There are 16 rows of pixels in a sprite.
    uint8_t h, l;
    for (int i = 0; i < 16; i++) {
        // Set to the address in the screen buffer at 24576 that corresponds
        // to where we are going to draw the next pixel row of the sprite graphic.
        Speccy_splitAddress(SBUFADDRS[pix_y], h, l);
        uint16_t addr = Speccy_buildAddress(h, l | pix_x);

        // Merge the sprite byte with the background.
        Speccy_writeMemory(addr, willy.sprites[frame] | Speccy_readMemory(addr));

        // Move HL along to the next cell to the right.
        addr++;

        // Point to the next sprite byte.
        frame++;

        // Merge the sprite byte with the background.
        Speccy_writeMemory(addr, willy.sprites[frame] | Speccy_readMemory(addr));

        // Point to the next entry in the screen buffer address lookup table at SBUFADDRS.
        pix_y += 1;

        // Point to the next sprite byte.
        frame++;
    }
}

// Left-right movement table
//
// Used by the routine at Willy_getUserInputAndMove. The entries in this table are used to map
// the existing value (V) of Willy's direction and movement flags at DMFLAGS to
// a new value (V'), depending on the direction Willy is facing and how he is
// moving or being moved (by 'left' and 'right' keypresses and joystick input,
// or by a conveyor).
//
uint8_t LRMOVEMENT[16] = {
        // One of the first four entries is used when Willy is not moving.
        0,                  // V=0 (facing right, no movement) + no movement: V'=0 (no change)
        1,                  // V=1 (facing left, no movement) + no movement: V'=1 (no change)
        0,                  // V=2 (facing right, moving) + no movement: V'=0 (facing right, no movement) (i.e. stop)
        1,                  // V=3 (facing left, moving) + no movement: V'=1 (facing left, no movement) (i.e. stop)

        // One of the next four entries is used when Willy is moving left.
        1,                  // V=0 (facing right, no movement) + move left: V'=1 (facing left, no movement) (i.e. turn around)
        3,                  // V=1 (facing left, no movement) + move left: V'=3 (facing left, moving)
        1,                  // V=2 (facing right, moving) + move left: V'=1 (facing left, no movement) (i.e. turn around)
        3,                  // V=3 (facing left, moving) + move left: V'=3 (no change)

        // One of the next four entries is used when Willy is moving right.
        2,                  // V=0 (facing right, no movement) + move right: V'=2 (facing right, moving)
        0,                  // V=1 (facing left, no movement) + move right: V'=0 (facing right, no movement) (i.e. turn around)
        2,                  // V=2 (facing right, moving) + move right: V'=2 (no change)
        0,                  // V=3 (facing left, moving) + move right: V'=0 (facing right, no movement) (i.e. turn around)

        // One of the final four entries is used when Willy is being pulled both left
        // and right; each entry leaves the flags at DMFLAGS unchanged (so Willy carries
        // on moving in the direction he's already moving, or remains stationary).
        0,                  // V=V'=0 (facing right, no movement)
        1,                  // V=V'=1 (facing left, no movement)
        2,                  // V=V'=2 (facing right, moving)
        3,                  // V=V'=3 (facing left, moving)
};
