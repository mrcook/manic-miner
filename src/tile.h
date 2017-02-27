// Manic Miner C Port Copyright 2017 Michael R. Cook

#pragma once

#include "headers.h"

typedef struct Tile_ {
    // Attribute byte is used both to match the appropriate location in the cavern mapping,
    // and set the sprite Attributes (colours)....I think!
    uint8_t id;

    // The actual tile sprite
    uint8_t sprite[8];
} Tile;
