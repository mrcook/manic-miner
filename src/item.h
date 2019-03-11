// Manic Miner C/C++ port Copyright (c) 2016-2019 Michael R. Cook
// Manic Miner Copyright (c) 1983 Bug-Byte Ltd.

#pragma once

#include "headers.h"

/*
    Item definition

    +---------+-----------------------------------------------------------------+
    | Byte(s) | Contents                                                        |
    +---------+-----------------------------------------------------------------+
    | 0       | Current attribute                                               |
    | 1,2     | Address of the item's location in the attribute buffer at 23552 |
    | 3       | MSB of the address of the item's location in the screen buffer  |
    |         | at 24576                                                        |
    | 4       | Unused (always 255)                                             |
    +---------+-----------------------------------------------------------------+
*/

typedef struct item_ {
    uint8_t attribute;
    uint16_t address;
    uint8_t addressMSB;

    uint8_t tile[8];
} Item;
