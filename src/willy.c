// Manic Miner C Port Copyright 2017 Michael R. Cook

#include "externs.h"
#include "willy.h"

void initializeWilly() {
    willy.NOMEN = 2;
    willy.PIXEL_Y = 208;
    willy.FRAME = 0;
    willy.DMFLAGS = 0;
    willy.AIRBORNE = 0;
    willy.LOCATION = 23970;
    willy.JUMPING = 0;
}

// Left-right movement table
//
// Used by the routine at MOVEWILLY2. The entries in this table are used to map
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
