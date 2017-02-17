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
    uint8_t JUMPING;
} Willy;

void Willy_initialize();