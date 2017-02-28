// Manic Miner C Port Copyright 2017 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.

#ifndef MANIC_MINER_WILLY_H
#define MANIC_MINER_WILLY_H


#include "headers.h"

extern uint8_t LRMOVEMENT[16];

enum WillyFacing {
    WillyRight,
    WillyLeft,
};

typedef struct Willy_ {
    // Number of Lives
    int NOMEN;

    // Direction Willy is facing (reset=right, set=left) (DMFLAGS)
    // enum WillyFacing direction;
    // Willy's movement flag (set=moving)
    // bool moving;
    uint8_t DMFLAGS;

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
    // FIXME: another part of the program says, Possible values: 1-18.
    uint8_t JUMPING;

    // Willy sprite graphic data (NOTE: combines the original MANDAT, WILLYR2, WILLYR3)
    //
    // 32 bytes per frame (16x16 pixels)
    // Each pair of bytes is a row of pixels.
    //
    // MANDAT  ( 0-31): Walk Right frame 1
    // MANDAT  (32-63): Walk Right frame 2
    // WILLYR2 ( 0-31): Walk Right frame 3
    // WILLYR3 ( 0-31): Walk Right frame 4
    // WILLYR3 (32...): Walk Left  frames 1 - 4
    uint8_t sprites[256];
} Willy;

void Willy_initialize(int lives);


#endif //MANIC_MINER_WILLY_H
