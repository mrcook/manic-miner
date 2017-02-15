// Manic Miner C Port Copyright 2017 Michael R. Cook

#pragma once

#include "headers.h"

enum WillyFacing {
    WillyRight,
    WillyLeft,
};

typedef struct Willy_ {
    // Number of Lives
    int NOMEN;

    // Direction Willy is facing (reset=right, set=left) (DMFLAGS)
    enum WillyFacing direction;

    // Willy's movement flag (set=moving)
    bool moving;

    // Willy's pixel y-coordinate (x2)
    // Holds the LSB of the address of the entry in the screen buffer address
    // lookup table that corresponds to Willy's pixel y-coordinate;
    // in practice, this is twice Willy's actual pixel y-coordinate.
    uint8_t PIXEL_Y;

    // Willy's current animation frame. Possible values are 0, 1, 2 and 3.
    uint8_t FRAME;

    // Airborne status indicator. Possible values are:
    //      0   Willy is neither falling nor jumping
    //      1   Willy is jumping
    //   2-11   Willy is falling, and can land safely
    //     12+  Willy is falling, and has fallen too far to land safely
    //    255   Willy has collided with a nasty or a guardian
    uint8_t AIRBORNE;

    // Address of Willy's current location in the attribute buffer at 23552
    uint16_t LOCATION;

    // Current Jumping animation counter. Possible values: 0-17.
    uint8_t JUMPING;

    // Left-right movement table (LRMOVEMENT)
    //
    // The entries in this table are used to map the existing value (V) of
    // Willy's direction and movement flags at DMFLAGS to a new value (V'),
    // depending on the direction Willy is facing and how he is moving or
    // being moved (by 'left' and 'right' keypress and joystick input,
    // or by a conveyor).
    //
    // One of the first four entries is used when Willy is not moving.
    // V=0 (facing right, no movement) + no movement: V'=0 (no change)
    // V=1 (facing left, no movement) + no movement: V'=1 (no change)
    // V=2 (facing right, moving) + no movement: V'=0 (facing right, no movement) (i.e. stop)
    // V=3 (facing left, moving) + no movement: V'=1 (facing left, no movement) (i.e. stop)

    // One of the next four entries is used when Willy is moving left.
    // V=0 (facing right, no movement) + move left: V'=1 (facing left, no movement) (i.e. turn around)
    // V=1 (facing left, no movement) + move left: V'=3 (facing left, moving)
    // V=2 (facing right, moving) + move left: V'=1 (facing left, no movement) (i.e. turn around)
    // V=3 (facing left, moving) + move left: V'=3 (no change)

    // One of the next four entries is used when Willy is moving right.
    // V=0 (facing right, no movement) + move right: V'=2 (facing right, moving)
    // V=1 (facing left, no movement) + move right: V'=0 (facing right, no movement) (i.e. turn around)
    // V=2 (facing right, moving) + move right: V'=2 (no change)
    // V=3 (facing left, moving) + move right: V'=0 (facing right, no movement) (i.e. turn around)

    // One of the final four entries is used when Willy is being pulled both left
    // and right; each entry leaves the flags at DMFLAGS unchanged (so Willy carries
    // on moving in the direction he's already moving, or remains stationary).
    // V=V'=0 (facing right, no movement)
    // V=V'=1 (facing left, no movement)
    // V=V'=2 (facing right, moving)
    // V=V'=3 (facing left, moving)
    uint8_t LRMOVEMENT[16];
} Willy;
