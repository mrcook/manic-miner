// Manic Miner C/C++ port Copyright (c) 2016-2021 Michael R. Cook
// Manic Miner Copyright (c) 1983 Bug-Byte Ltd.

#pragma once

#include "headers.h"

/*
    Horizontal guardians

    There are four slots, each one seven bytes long, used to hold the state of
    the horizontal guardians in the current cavern.

    For each horizontal guardian, the seven bytes are used as follows:
    +------+--------------------------------------------------------------------+
    | Byte | Contents                                                           |
    +------+--------------------------------------------------------------------+
    | 0    | Bit 7: animation speed (0=normal, 1=slow)                          |
    |      | Bits 0-6: attribute (BRIGHT, PAPER and INK)                        |
    | 1,2  | Address of the guardian's location in the attribute buffer at      |
    |      | 23552                                                              |
    | 3    | MSB of the address of the guardian's location in the screen buffer |
    |      | at 24576                                                           |
    | 4    | Animation frame                                                    |
    | 5    | LSB of the address of the leftmost point of the guardian's path in |
    |      | the attribute buffer                                               |
    | 6    | LSB of the address of the rightmost point of the guardian's path   |
    |      | in the attribute buffer                                            |
    +------+--------------------------------------------------------------------+
*/

typedef struct guardianHorizontal_ {
public:
    // animation speed (0=normal, 1=slow)
    // Colour attribute (BRIGHT, PAPER and INK)
    uint8_t speedColour;

    // Address of the guardian's location in the attribute buffer at 23552
    uint16_t attributeAddress;
    // MSB of the address of the guardian's location in the screen buffer at 24576
    uint8_t addressMSB;
    // LSB of the address of the leftmost point of the guardian's path in the attribute buffer

    uint8_t frame;

    uint8_t addressLeftLSB;
    // LSB of the address of the rightmost point of the guardian's path in the attribute buffer
    uint8_t addressRightLSB;

    // Guardian graphic data (yes, it uses more memory, but gives greater flexibility)
    uint8_t GGDATA[256];
} GuardianHorizontal;

// MOVEHG
// returning a true value stops iterating on any further guardians
bool GuardianH_update(GuardianHorizontal &guardian);

// DRAWHG
// returning a true value indicated Willy has been killed
bool GuardianH_draw(GuardianHorizontal &guardian);
