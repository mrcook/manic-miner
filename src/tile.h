// Manic Miner C/C++ port Copyright (c) 2016-2021 Michael R. Cook
// Manic Miner Copyright (c) 1983 Bug-Byte Ltd.

#pragma once

#include "headers.h"

typedef struct tile_ {
    // Attribute byte is used both to match the appropriate location in the cavern mapping,
    // and set the sprite Attributes (colours)....I think!
    uint8_t id;

    // The actual tile sprite
    uint8_t sprite[8];
} Tile;
